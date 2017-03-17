// This file is a part of quicksave project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#pragma once

#include <folly/Memory.h>
#include <proxygen/httpserver/RequestHandler.h>
#include <util/buffer.h>

class ProxygenHandler : public proxygen::RequestHandler
{
public:
    void onRequest(std::unique_ptr<proxygen::HTTPMessage> headers) noexcept override;

    void onBody(std::unique_ptr<folly::IOBuf> body) noexcept override;

    void onEOM() noexcept override;

    void onUpgrade(proxygen::UpgradeProtocol proto) noexcept override;

    void requestComplete() noexcept override;

    void onError(proxygen::ProxygenError err) noexcept override;

    virtual void handle() = 0;
    virtual void reply(int statusCode) = 0;
    virtual void reply(const char *message) = 0;
    virtual void reply_options() = 0;

    bool isAuthenticationRequest()
    {
        std::string path = headers_->getPath();
        return ((path == "/login/") || (path == "/logout") || (path == "/status/"));
    }

protected:
    std::unique_ptr<proxygen::HTTPMessage> headers_;
    std::unique_ptr<folly::IOBuf> body_;
    std::unique_ptr<folly::IOBuf> response;
    int statusCode;
    std::string contiguousBody;
};

#include <proxygen/httpserver/RequestHandler.h>

#include <timer>
#include <proxygen/httpserver/ResponseBuilder.h>
#include <util/format.h>
#include <util/logger.h>

void ProxygenHandler::onRequest(std::unique_ptr<proxygen::HTTPMessage> headers) noexcept
{
    TIC;
    headers_ = std::move(headers);
    Logger::log(Logger::format("%s %s", headers_->getMethodString().c_str(), headers_->getPath().c_str()));
}

void ProxygenHandler::onBody(std::unique_ptr<folly::IOBuf> body) noexcept
{
    if (body_)
    {
        body_->prependChain(std::move(body));
    }
    else
    {
        body_ = std::move(body);
    }
}

void ProxygenHandler::onEOM() noexcept
{
    if (body_)
    {
        contiguousBody = Buffer::to_string(body_);
        /*
        if (body_->computeChainDataLength() != body_->length())
        {
            std::cout << "CHUJOWY BUFOR " << body_->computeChainDataLength() << " " << body_->length() << std::endl;
            std::string dobry_bufor = Buffer::to_string(body_);
            std::cout << "DOBRY BUFOR " << dobry_bufor << " " << dobry_bufor.length() << std::endl;
            return reply(400);
        }
        else {
          */
        Logger::log(Logger::format("< %s [%luB]", contiguousBody.c_str(), contiguousBody.length()));

        handle();
        TAC;
    }
    else if (headers_->getMethodString() == "GET")
    {
        contiguousBody = headers_->getDecodedQueryParam("body");
        Logger::log(Logger::format("< %s [%luB]", contiguousBody.c_str(), contiguousBody.length()));

        handle();
        TAC;
    }
    else if (headers_->getMethodString() == "OPTIONS")
    {
        return reply_options();
    }
    else if (isAuthenticationRequest())
    {
        handle();
        TAC;
    }
    else
    {
        TAC;
        return reply(400);
    }
}

void ProxygenHandler::onUpgrade(proxygen::UpgradeProtocol protocol) noexcept
{
    // handler doesn"t support upgrades
}

void ProxygenHandler::requestComplete() noexcept
{
    delete this;
}

void ProxygenHandler::onError(proxygen::ProxygenError err) noexcept
{
    delete this;
}
