// This file is a part of quicksave project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#ifndef QUICKSAVE_QUICKSAVEHANDLER_H
#define QUICKSAVE_QUICKSAVEHANDLER_H

#include "ProxygenHandler.h"
#include "RequestContext.h"
#include <SQLiteCpp/Database.h>
#include <auth/Authentication.h>
#include <bean/MessageBean.h>
#include <http/Exception.h>
#include <proxygen/httpserver/ResponseBuilder.h>
#include <rapidjson/document.h>
#include <databaseBean/DatabaseBeanUser.h>
#include <util/format.h>
#include <util/logger.h>
#include <uuid>
#include <bean/SessionBean.h>
#include <databaseBean/DatabaseBeanSession.h>
#include "QuicksaveHandler.h"
#include "rapidjson/document.h"
#include <folly/io/IOBuf.h>
#include <proxygen/httpserver/ResponseBuilder.h>
#include <proxygen/lib/http/HTTPMessage.h>

#include <server/RequestContext.h>
#include <api/CreateRequest.h>
#include <api/GenericRequest.h>
#include <api/RetrieveRequest.h>
#include <api/TagCreateRequest.h>
#include <api/TagDeleteRequest.h>
#include <api/ItemDeleteRequest.h>
#include <api/ItemUpdateRequest.h>
#include <api/TagUpdateRequest.h>
#include <auth/Authentication.h>
#include <http/Exception.h>


class QuicksaveHandler : public ProxygenHandler
{
public:
    void handle() override;

    std::string formatCookie(const std::string &sessionid)
    {
        if (sessionid == "")
        {
            return "sessionid=; HttpOnly; Path=/; expires=Thu, 01 Jan 1970 00:00:00 GMT";
        }
        else
        {
            return Format::format("sessionid=%s; HttpOnly; Path=/", sessionid);
        }
    }

    void reply_options()
    {
        this->response = folly::IOBuf::copyBuffer("");

        std::string buffer{reinterpret_cast<const char *>(response->data()), response->length()};
        Logger::log(Logger::format("> %lu %s [%luB]", this->statusCode, buffer.c_str(), response->length()));


        proxygen::ResponseBuilder(downstream_)
                .status(200, "OK")
                .header("Vary", "Accept-Encoding, Origin")
                .header("Keep-Alive", "timeout=5, max=100")
                .header("Connection", "Keep-Alive")
                .header("Content-Type", "text/plain")
                .body(std::move(this->response))
                .sendWithEOM();
    }

    void reply()
    {
        std::string cookie = formatCookie(requestContext.session_hash);

        std::string buffer{reinterpret_cast<const char *>(response->data()), response->length()};
        Logger::log(Logger::format("> %lu %s [%luB]", this->statusCode, buffer.c_str(), response->length()));

        proxygen::ResponseBuilder(downstream_)
            .status(this->statusCode, "OK")
            .header("Content-Type", "application/json")
            .header("Vary", "Cookie")
            .header("Set-Cookie", cookie.c_str())
            .body(std::move(this->response))
            .sendWithEOM();
    }

    void reply(const char *message)
    {
        this->statusCode = 200;
        this->response = folly::IOBuf::copyBuffer(message);

        reply();
    }

    void reply(int statusCode)
    {
        static std::unordered_map<int, const char *> messageBody = {
            {400, "{\"message\": \"bad request\"}"},
            {401, "{\"message\": \"not authenticated\"}"},
            {403, "{\"message\": \"not authorized\"}"},
            {404, "{\"message\": \"not found\"}"},
            {415, "{\"message\": \"unsupported media type\"}"},
            {500, "{\"message\": \"internal server error\"}"},
            {500, "{\"message\": \"not implemented\"}"}};

        this->statusCode = statusCode;

        if (statusCode != 200)
        {
            this->response = folly::IOBuf::copyBuffer(messageBody[statusCode]);
        }

        reply();
    }

    rapidjson::Document parse(const folly::IOBuf *body_)
    {
        rapidjson::Document document;

        std::string buffer{reinterpret_cast<const char *>(body_->data()), body_->length()};

        document.Parse(buffer.c_str());
        if (document.HasParseError())
        {
            throw ParseError("");
        }

        return document;
    }

    bool authenticateWithPassword(std::pair<std::string, std::string> credentials)
    {
        List<UserBean> userBeanList = DatabaseBean<UserBean>::get_by(requestContext.db.get(), "name", credentials.first);

        if ((userBeanList.size() == 1) && (credentials.second == userBeanList[0].pass))
        {
            Logger::log("Authenticated with BasicAuth");
            std::string session_hash = UUID::get();

            Logger::log(session_hash);

            DatabaseBean<SessionBean>::insert_with_pk(requestContext.db.get(), SessionBean(session_hash, *userBeanList[0].user_id));

            requestContext.userBean = userBeanList[0];
            requestContext.session_hash = session_hash;

            MessageBean messageBean;
            messageBean.message = "logged";
            response = messageBean;

            return true;
        }
        else
        {
            Logger::log("Not Authenticated with BasicAuth");
            return false;
        }
    }

    bool authenticateWithCookie()
    {
        std::string cookie = headers_->getCookie("sessionid").toString();
        if (cookie != "")
        {
            List<SessionBean> sessionBeanList = DatabaseBean<SessionBean>::get_by(requestContext.db.get(), "session_hash", cookie);
            if (sessionBeanList.empty())
            {
                Logger::log(Logger::format("Not Authenticated with Cookie: %s", cookie.c_str()));
            }
            else
            {
                Logger::log(Logger::format("Authenticated with Cookie: %s", cookie.c_str()));

                requestContext.userBean = *DatabaseBean<UserBean>::get(requestContext.db.get(), sessionBeanList[0].user_id);
                requestContext.session_hash = cookie;
                return true;

            }
        }
        else
        {
            Logger::log("Not Authenticated with Cookie");
            return false;
        }

    }

    void logout()
    {
        std::string cookie = headers_->getCookie("sessionid").toString();
        if (cookie != "")
        {
            DatabaseBean<SessionBean>::remove_by(requestContext.db.get(), "session_hash", cookie);
            requestContext.session_hash = "";
        }

        return;
    }

private:
    RequestContext<SQLite::Database> requestContext;
    rapidjson::Document document;
};

void QuicksaveHandler::handle()
{
    const std::string path = headers_->getPath();

    requestContext.db = std::make_unique<SQLite::Database>("/io.quicksave.db/db.sqlite3", SQLite::OPEN_READWRITE);

    if (path == "/logout/")
    {
        logout();
        response = MessageBean(std::string("Logged out..."));
        return reply(200);
    }

    if (path == "/login/")
    {
        if (!authenticateWithPassword(Authentication::dispatchBasicAuth(headers_.get())))
        {
            return reply(401);
        } else
        {
            response = MessageBean(std::string(requestContext.userBean.name));
            return reply(200);
        }
    }
    else {
        if (!authenticateWithCookie())
        {
            return reply(401);
        }
    }

    try
    {
        document = parse(body_.get());
    }
    catch (ParseError& parseError)
    {
        return reply(400);
    }

    try
    {
        if (path == "/status/")
        {
            response = MessageBean(std::string(requestContext.userBean.name));
        }
        else if (path == "/create/")
        {
            response = GenericRequest<CreateRequest>::handle(&requestContext, document);
        }
        else if (path == "/retrieve/")
        {
            response = GenericRequest<RetrieveRequest>::handle(&requestContext, document);
        }
        else if (path == "/tag/create/")
        {
            response = GenericRequest<TagCreateRequest>::handle(&requestContext, document);
        }
        else if (path == "/tag/update/")
        {
            response = GenericRequest<TagUpdateRequest>::handle(&requestContext, document);
        }
        else if (path == "/tag/delete/")
        {
            response = GenericRequest<TagDeleteRequest>::handle(&requestContext, document);
        }
        else if (path == "/item/delete/")
        {
            response = GenericRequest<ItemDeleteRequest>::handle(&requestContext, document);
        }
        else if (path == "/item/update/")
        {
            response = GenericRequest<ItemUpdateRequest>::handle(&requestContext, document);
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

    return reply(200);
}

#endif
