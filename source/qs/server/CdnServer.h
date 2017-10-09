// This file is a part of quicksave project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#pragma once

#include <absl/strings/str_split.h>

#include <folly/Format.h>

#include <SQLiteCpp/Database.h>
#include <folly/io/IOBuf.h>
#include <proxygen/httpserver/ResponseBuilder.h>
#include <proxygen/lib/http/HTTPMessage.h>

#include <qs/database/ProviderFactory.h>
#include <qs/http/Exception.h>
#include <qs/oauth/OAuthAPI.h>
#include <qs/oauth/OAuthHelper.h>
#include <qs/oauth/OAuthMasterDatabase.h>
#include <qs/server/ProxygenHandler.h>
#include <qs/server/RequestContext.h>
#include <qs/storage/StorageFactory.h>
#include <qs/util/uuid.h>
#include <qsgen/bean/FileBean.h>
#include <qsgen/bean/MetaBean.h>
#include <qsgen/bean/SessionBean.h>
#include <qsgen/bean/TokenBean.h>
#include <qsgen/bean/TokenRequestBean.h>
#include <qsgen/databaseBean/DatabaseBeans.h>


namespace qs {
namespace server {

class CdnServer : public ProxygenHandler
{
public:
    void handle_get() override
    {
        const std::string &path = headers_->getPath();
        const std::string &token = headers_->getCookie("token").toString();

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

            auto databaseConnection = database::ProviderFactory::create(
                sessionBean.user.databaseConnectionString);
            auto transaction = databaseConnection->getTransaction();
            requestContext.databaseTransaction = transaction.get();
            requestContext.userBean = sessionBean.user;

            std::vector<std::string> path_split = absl::StrSplit(path, '/');

            if (path_split.size() < 4)
            {
                return reply(400);
            }

            auto user_hash = path_split[1];
            auto meta_hash = path_split[2];
            auto file_hash = path_split[3];

            if (requestContext.userBean.user_hash != user_hash)
            {
                return reply(403);
            }

            auto meta = database::Action::get<MetaBean>(requestContext.databaseTransaction, meta_hash);

            if (!meta)
            {
                return reply(400);
            }
            else if (meta->user_hash != user_hash)
            {
                return reply(401);
            }

            auto file = database::Action::get<FileBean>(requestContext.databaseTransaction, file_hash);

            if (!file)
            {
                return reply(404);
            }
            else if (file->meta_hash != meta_hash)
            {
                return reply(400);
            }

            std::unique_ptr<storage::Storage> storage = storage::StorageFactory::create(requestContext,
                                                                                        requestContext.userBean.storageConnectionString);

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
        const std::string contiguousBody = qs::util::Buffer::to_string(body_);

        LOG(INFO) << folly::format("< [{}B] {}", contiguousBody.length(), contiguousBody.c_str());

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
}
}