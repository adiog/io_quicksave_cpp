// This file is a part of quicksave project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#ifndef QUICKSAVE_OAUTHHANDLER_H
#define QUICKSAVE_OAUTHHANDLER_H


#include "OAuthHelper.h"
#include "ProxygenHandler.h"
#include <OAuthAPI.h>
#include <OAuthMasterDatabase.h>
#include <uuid>
#include <SQLiteCpp/Database.h>
#include <bean/SessionBean.h>
#include <bean/TokenBean.h>
#include <bean/TokenRequestBean.h>
#include <databaseBean/DatabaseBeanUser.h>
#include <folly/io/IOBuf.h>
#include <http/Exception.h>
#include <proxygen/httpserver/ResponseBuilder.h>
#include <proxygen/lib/http/HTTPMessage.h>
#include <util/format.h>
#include <util/logger.h>
#include <util/split>
#include <server/RequestContext.h>

#include <bean/MetaBean.h>
#include <databaseBean/DatabaseBeanMeta.h>
#include <bean/FileBean.h>
#include <databaseBean/DatabaseBeanFile.h>
#include <fstream>
#include <database/ProviderFactory.h>
#include <storage/StorageFactory.h>

class ContentHandler : public ProxygenHandler
{
public:
    void handle_get() override
    {
        const std::string& path = headers_->getPath();
        const std::string& token = headers_->getCookie("token").toString();

        if (path == "/session/check")
        {
            if (OAuthAPI::check_session(token))
            {
                return reply_cookie(200, "token", token);
            }
            else
            {
                return reply(204);
            }
        }
        else if (OAuthAPI::check_session(token))
        {
            SessionBean sessionBean = OAuthAPI::get_session(token);

            RequestContext requestContext;

            auto databaseConnection = database::ProviderFactory::create(sessionBean.user.databaseConnectionString);
            auto transaction = databaseConnection->getTransaction();
            requestContext.databaseTransaction = transaction.get();
            requestContext.userBean = sessionBean.user;

            auto path_split = split(path, '/');

            if (path_split.size() < 4) {
                return reply(400);
            }

            auto user_hash = path_split[1];
            auto meta_hash = path_split[2];
            auto file_hash = path_split[3];

            if (requestContext.userBean.user_hash != user_hash)
            {
                return reply(403);
            }

            auto meta = DatabaseBean<MetaBean>::get(requestContext.databaseTransaction, meta_hash);

            if (!meta)
            {
                return reply(400);
            }
            else if (meta->user_hash != user_hash)
            {
                return reply(401);
            }

            auto file = DatabaseBean<FileBean>::get(requestContext.databaseTransaction, file_hash);

            if (!file)
            {
                return reply(404);
            }
            else if (file->meta_hash != meta_hash)
            {
                return reply(400);
            }

            std::unique_ptr<storage::Storage> storage = storage::StorageFactory::create(requestContext, requestContext.userBean.storageConnectionString);

            std::string filebody = storage->read(*file);

            proxygen::ResponseBuilder(downstream_)
                    .status(200, "OK")
                    .header("Content-Type", file->mimetype)
                    .body(filebody)
                    .sendWithEOM();
        }
        else
        {
            return reply(400);
        }
    }

    void handle_post() override
    {
        const std::string path = headers_->getPath();
        const std::string contiguousBody = Buffer::to_string(body_);

        Logger::log("< [%luB] %s", contiguousBody.length(), contiguousBody.c_str());

        if (path == "/token/put")
        {
            const TokenBean tokenBean{contiguousBody.c_str()};
            const std::string token = tokenBean.token;

            if (OAuthAPI::check_session(token))
            {
                return reply_cookie(200, "token", token);
            }
            else
            {
                return reply_cookie(400, "token", "");
            }
        }
        else
        {
            return reply(400);
        }
    }
};

#endif
