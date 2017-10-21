// This file is a part of quicksave project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#pragma once

#include <reference_cast>

#include <absl/strings/str_split.h>

#include <folly/Format.h>

#include <folly/io/IOBuf.h>
#include <proxygen/httpserver/ResponseBuilder.h>
#include <proxygen/lib/http/HTTPMessage.h>

#include <qs/database/ProviderFactory.h>
#include <qs/server/Exception.h>
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


namespace qs {
namespace server {

class CdnServer : public ProxygenHandler
{
public:
    FileBean getFileBean(RequestContext &requestContext, const std::string &path) {
        std::vector<std::string> path_split = absl::StrSplit(path, '/');

        if (path_split.size() < 4) {
            throw HttpStatus{400};
        }

        auto user_hash = path_split[1];
        auto meta_hash = path_split[2];
        auto file_hash = path_split[3];

        if (requestContext.userBean.user_hash != user_hash) {
            throw HttpStatus{403};
        }

        auto meta = qsgen::orm::ORM<MetaBean>::get(requestContext.databaseTransaction, meta_hash);

        if (!meta) {
            throw HttpStatus{400};
        } else if (meta->user_hash != user_hash) {
            throw HttpStatus{401};
        }

        auto file = qsgen::orm::ORM<FileBean>::get(requestContext.databaseTransaction, file_hash);

        if (!file) {
            throw HttpStatus{404};
        } else if (file->meta_hash != meta_hash) {
            throw HttpStatus{400};
        }

        return std::move(*file);
    }

    std::string readFileBody(RequestContext &requestContext, const FileBean& fileBean)
    {
        std::unique_ptr<storage::Storage> storage = storage::StorageFactory::create(requestContext,
                                                                                    requestContext.userBean.storageConnectionString);

        return storage->read(fileBean);
    }

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

            auto databaseConnectionOwner = database::ProviderFactory::create(sessionBean.user.databaseConnectionString);
            auto& databaseConnection = reference_cast(databaseConnectionOwner);

            RequestContext requestContext{
                sessionBean.user,
                sessionBean.token,
                databaseConnection
            };

            try
            {
                auto fileBean = getFileBean(requestContext, path);

                std::string fileBody = readFileBody(requestContext, fileBean);

                proxygen::ResponseBuilder(downstream_)
                        .status(200, "OK")
                        .header("Content-Type", fileBean.mimetype)
                        .body(move(fileBody))
                        .sendWithEOM();
            }
            catch (HttpStatus httpStatus) {
                return reply(httpStatus.statusCode);
            }
        }
        else
        {
            return reply(400);
        }
    }

    void handle_post() override
    {
        const std::string path = headers_->getPath();
        const std::string contiguousBody = qs::util::Buffer::to_string(reference_cast(body_));

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