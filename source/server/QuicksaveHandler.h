// This file is a part of quicksave project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#ifndef QUICKSAVE_QUICKSAVEHANDLER_H
#define QUICKSAVE_QUICKSAVEHANDLER_H

#include "../../oauth/OAuthHelper.h"
#include "ProxygenHandler.h"
#include "QuicksaveHandler.h"
#include "RequestContext.h"
#include "rapidjson/document.h"
#include <uuid>
#include <SQLiteCpp/Database.h>
#include <bean/MessageBean.h>
#include <bean/SessionBean.h>
#include <bean/TokenBean.h>
#include <databaseBean/DatabaseBeans.h>
#include <folly/io/IOBuf.h>
#include <http/Exception.h>
#include <proxygen/httpserver/ResponseBuilder.h>
#include <proxygen/lib/http/HTTPMessage.h>
#include <rapidjson/document.h>
#include <util/format.h>
#include <util/logger.h>

#include "../../oauth/OAuthAPI.h"
#include "../../oauth/OAuthHelper.h"
#include <api/CreateRequest.h>
#include <api/GenericRequest.h>
#include <api/MetaDeleteRequest.h>
#include <api/MetaUpdateRequest.h>
#include <api/RetrieveRequest.h>
#include <api/TagCreateRequest.h>
#include <api/TagDeleteRequest.h>
#include <api/TagUpdateRequest.h>
#include <api/UploadRequest.h>
#include <http/Exception.h>
#include <server/RequestContext.h>
#include <database/ProviderFactory.h>

class QuicksaveHandler : public ProxygenHandler
{
public:
    void handle_get() override;
    void handle_post() override;

    rapidjson::Document parse(const std::string& contiguousBody)
    {
        rapidjson::Document document;

        document.Parse(contiguousBody.c_str());
        if (document.HasParseError())
        {
            throw ParseError("");
        }

        return document;
    }


private:
    RequestContext requestContext;
    rapidjson::Document document;

    std::unique_ptr<folly::IOBuf> response;
    std::string token;
    int statusCode;
    std::string contiguousBody;
};

void QuicksaveHandler::handle_get()
{
    const std::string& path = headers_->getPath();

    if (path == "/session/check")
    {
        const std::string& token = headers_->getCookie("token").toString();
        if (OAuthAPI::check_session(token))
        {
            return reply_cookie(200, "token", token);
        }
        else
        {
            return reply(204);
        }
    }
    else if (path == "/session/delete")
    {
        const std::string& token = headers_->getCookie("token").toString();
        if (OAuthAPI::check_session(token))
        {
            if (OAuthAPI::destroy_session(token))
            {
                return reply_cookie(200, "token", "");
            }
            else
            {
                return reply(500);
            }
        }
        else
        {
            return reply(204);
        }
    }
    else
    {
        return reply(400);
    }
}

void QuicksaveHandler::handle_post()
{
    const std::string path = headers_->getPath();
    const std::string contiguousBody = Buffer::to_string(body_);

    int limit = std::min(static_cast<int>(contiguousBody.length()), 512);
    Logger::log("< [%luB] %s", contiguousBody.length(), std::string(&contiguousBody[0], limit).c_str());

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

    const std::string& token = headers_->getCookie("token").toString();

    if (!OAuthAPI::check_session(token))
    {
        return reply_cookie(401, "token", "");
    }

    try
    {
        document = parse(contiguousBody);
    }
    catch (ParseError& parseError)
    {
        return reply(400);
    }

    SessionBean sessionBean = OAuthAPI::get_session(token);

    std::unique_ptr<database::Connection> databaseConnection = database::ProviderFactory::create(sessionBean.user.databaseConnectionString);
    std::unique_ptr<database::Transaction> databaseTransaction = databaseConnection->getTransaction();
    requestContext.databaseTransaction = databaseTransaction.get();
    requestContext.userBean = sessionBean.user;

    try
    {
        if (path == "/create")
        {
            response = GenericRequest<CreateRequest>::handle(requestContext, document);
        }
        else if (path == "/retrieve")
        {
            response = GenericRequest<RetrieveRequest>::handle(requestContext, document);
        }
        else if (path == "/tag/create")
        {
            response = GenericRequest<TagCreateRequest>::handle(requestContext, document);
        }
        else if (path == "/tag/update")
        {
            response = GenericRequest<TagUpdateRequest>::handle(requestContext, document);
        }
        else if (path == "/tag/delete")
        {
            response = GenericRequest<TagDeleteRequest>::handle(requestContext, document);
        }
        else if (path == "/meta/delete")
        {
            response = GenericRequest<MetaDeleteRequest>::handle(requestContext, document);
        }
        else if (path == "/meta/update")
        {
            response = GenericRequest<MetaUpdateRequest>::handle(requestContext, document);
        }
        else if (path == "/upload")
        {
            response = GenericRequest<UploadRequest>::handle(requestContext, document);
        }
        else
        {
            return reply(404);
        }
    }
    catch (ParseError& parseError)
    {
        return reply(400);
    }

    requestContext.databaseTransaction->commit();
    return reply_response(response);
}

#endif
