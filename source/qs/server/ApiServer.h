// This file is a part of quicksave project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#pragma once

#include <folly/io/IOBuf.h>
#include <proxygen/httpserver/ResponseBuilder.h>
#include <proxygen/lib/http/HTTPMessage.h>
#include <rapidjson/document.h>

#include <qs/api/handler/CreateRequest.h>
#include <qs/api/handler/GenericRequest.h>
#include <qs/api/handler/MetaDeleteRequest.h>
#include <qs/api/handler/MetaUpdateRequest.h>
#include <qs/api/handler/PerspectiveCreateRequest.h>
#include <qs/api/handler/PerspectiveRetrieveRequest.h>
#include <qs/api/handler/PerspectiveUpdateRequest.h>
#include <qs/api/handler/PerspectiveDeleteRequest.h>
#include <qs/api/handler/RetrieveByPerspectiveRequest.h>
#include <qs/api/handler/RetrieveByQueryRequest.h>
#include <qs/api/handler/TagCreateRequest.h>
#include <qs/api/handler/TagDeleteRequest.h>
#include <qs/api/handler/TagUpdateRequest.h>
#include <qs/api/handler/UploadRequest.h>
#include <qs/database/ProviderFactory.h>
#include <qs/http/Exception.h>
#include <qs/oauth/OAuthAPI.h>
#include <qs/oauth/OAuthHelper.h>
#include <qs/server/ProxygenHandler.h>
#include <qs/server/RequestContext.h>
#include <qs/util/format.h>
#include <qs/util/uuid.h>
#include <qsgen/bean/MessageBean.h>
#include <qsgen/bean/SessionBean.h>
#include <qsgen/bean/TokenBean.h>
#include <qsgen/databaseBean/DatabaseBeans.h>


namespace qs {
namespace server {

class ApiServer : public ProxygenHandler
{
public:
    void handle_get() override;

    void handle_post() override;

    rapidjson::Document parse(const std::string &contiguousBody)
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

void ApiServer::handle_get()
{
    const std::string &path = headers_->getPath();

    if (path == "/session/check")
    {
        const std::string &token = headers_->getCookie("token").toString();
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
        const std::string &token = headers_->getCookie("token").toString();
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

void ApiServer::handle_post()
{
    const std::string path = headers_->getPath();
    const std::string contiguousBody = qs::util::Buffer::to_string(body_);

    int limit = std::min(static_cast<int>(contiguousBody.length()), 512);
    LOG(INFO) << Format::format("< [%luB] %s", contiguousBody.length(), std::string(&contiguousBody[0], limit).c_str());

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

    const std::string &token = headers_->getCookie("token").toString();

    if (!OAuthAPI::check_session(token))
    {
        return reply_cookie(401, "token", "");
    }

    try
    {
        document = parse(contiguousBody);
    }
    catch (ParseError &parseError)
    {
        return reply(400);
    }

    SessionBean sessionBean = OAuthAPI::get_session(token);

    std::unique_ptr<database::Connection> databaseConnection = database::ProviderFactory::create(
        sessionBean.user.databaseConnectionString);
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
            response = GenericRequest<RetrieveByQueryRequest>::handle(requestContext, document);
        }
        else if (path == "/retrieve_by_perspective")
        {
            response = GenericRequest<RetrieveByPerspectiveRequest>::handle(requestContext, document);
        }
        else if (path == "/perspective/create")
        {
            response = GenericRequest<PerspectiveCreateRequest>::handle(requestContext, document);
        }
        else if (path == "/perspective/retrieve")
        {
            response = GenericRequest<PerspectiveRetrieveRequest>::handle(requestContext, document);
        }
        else if (path == "/perspective/update")
        {
            response = GenericRequest<PerspectiveUpdateRequest>::handle(requestContext, document);
        }
        else if (path == "/perspective/delete")
        {
            response = GenericRequest<PerspectiveDeleteRequest>::handle(requestContext, document);
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
    catch (ParseError &parseError)
    {
        return reply(400);
    }

    requestContext.databaseTransaction->commit();
    return reply_response(response);
}
}
}