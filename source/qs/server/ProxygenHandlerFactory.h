// This file is a part of quicksave project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#pragma once

#include <proxygen/httpserver/RequestHandlerFactory.h>

namespace qs {

template <typename ProxygenHandle>
class ProxygenHandlerFactory : public proxygen::RequestHandlerFactory
{
public:
    void onServerStart(folly::EventBase *evb) noexcept override
    {
    }

    void onServerStop() noexcept override
    {
    }

    proxygen::RequestHandler *onRequest(proxygen::RequestHandler *, proxygen::HTTPMessage *) noexcept override
    {
        return new ProxygenHandle();
    }
};
}
