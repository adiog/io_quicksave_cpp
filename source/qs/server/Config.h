// This file is a part of quicksave project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#pragma once

#include <vector>

#include <proxygen/httpserver/HTTPServer.h>
#include <proxygen/httpserver/HTTPServerOptions.h>

#include <qs/server/ProxygenHandlerFactory.h>


namespace qs {
namespace server {

class Config
{
public:
    static long getThreads(long threads)
    {
        if (threads <= 0)
        {
            threads = sysconf(_SC_NPROCESSORS_ONLN);
            CHECK(threads > 0);
        }
        return threads;
    }

    template <typename ServerHandler>
    static typename proxygen::HTTPServerOptions getOptions(long threads)
    {
        proxygen::HTTPServerOptions options;
        options.threads = static_cast<size_t>(threads);
        options.idleTimeout = std::chrono::milliseconds(60000);
        options.shutdownOn = {SIGINT, SIGTERM};
        options.enableContentCompression = false;
        options.handlerFactories = proxygen::RequestHandlerChain()
                                       .addThen<ProxygenHandlerFactory<ServerHandler> >()
                                       .build();
        return options;
    }

    static std::vector<proxygen::HTTPServer::IPConfig> getHttpIp(const std::string &host, uint16_t port)
    {
        return {{folly::SocketAddress(host, port, true), proxygen::HTTPServer::Protocol::HTTP}};
    }
};
}
}
