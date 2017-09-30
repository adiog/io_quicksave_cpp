// This file is a part of quicksave project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#pragma once

#include <absl/types/optional.h>

#include <SQLiteCpp/Database.h>
#include <folly/io/IOBuf.h>
#include <proxygen/httpserver/ResponseBuilder.h>
#include <proxygen/lib/http/HTTPMessage.h>

#include <ProxygenHandler.h>
#include <uuid>
#include <qs/http/Exception.h>
#include <qs/oauth/OAuthAPI.h>
#include <qs/oauth/OAuthHelper.h>
#include <qs/oauth/OAuthMasterDatabase.h>
#include <qsgen/bean/SessionBean.h>
#include <qsgen/bean/TokenBean.h>
#include <qsgen/bean/TokenRequestBean.h>
#include <qsgen/databaseBean/DatabaseBeans.h>
#include <util/format.h>


namespace qs {
namespace server {

class OAuthServer : public ProxygenHandler
{
public:
    void handle_get() override
    {
        return reply(400);
    }

    void handle_post() override
    {
        const std::string path = headers_->getPath();
        if (path == "/token/get")
        {
            const std::string body = Buffer::to_string(body_);
            const TokenRequestBean tokenRequest{body.c_str()};
            absl::optional<UserBean> userBean = OAuthMasterDatabase::authenticateWithPassword(
                OAuthHelper::dispatchBasicAuth(headers_.get()));

            if (userBean)
            {
                const std::string token = OAuthAPI::create_session(*userBean, tokenRequest.expireTime);
                return reply_token(token);
            }
            else
            {
                return reply(204);
            }
        }
        else if (path == "/token/delete")
        {
            const std::string body = Buffer::to_string(body_);
            const TokenBean tokenBean{body.c_str()};
            const std::string token = tokenBean.token;

            if (OAuthAPI::destroy_session(token))
            {
                return reply(200);
            }
            else
            {
                return reply(401);
            }
        }
        else if (path == "/token/check")
        {
            const std::string body = Buffer::to_string(body_);
            const TokenBean tokenBean{body.c_str()};
            const std::string token = tokenBean.token;

            if (OAuthAPI::check_session(token))
            {
                return reply(200);
            }
            else
            {
                return reply(204);
            }
        }
        {
            return reply(404);
        }
    }

    void reply_token(const std::string &token)
    {
        TokenBean tokenBean(token);

        proxygen::ResponseBuilder(downstream_)
            .status(200, "OK")
            .header("Content-Type", "application/json")
            .body(tokenBean.to_string())
            .sendWithEOM();
    }
};
}
}
