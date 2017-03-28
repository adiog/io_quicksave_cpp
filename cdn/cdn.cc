// This file is a part of quicksave project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.


#include <env.h>
#include <unistd.h>
#include <folly/Memory.h>
#include <folly/io/async/EventBaseManager.h>
#include <gflags/gflags.h>
#include <mq/queue.h>
#include <proxygen/httpserver/HTTPServer.h>
#include <proxygen/httpserver/RequestHandlerFactory.h>
#include <ContentHandler.h>
#include <ProxygenHandlerFactory.h>

using folly::EventBase;
using folly::EventBaseManager;
using folly::SocketAddress;

using Protocol = proxygen::HTTPServer::Protocol;


int main(int argc, char* argv[])
{
    gflags::ParseCommandLineFlags(&argc, &argv, true);
    google::InitGoogleLogging(argv[0]);
    google::InstallFailureSignalHandler();

    std::cout << FLAGS_IO_QUICKSAVE_CDN_PORT << std::endl;

    std::vector<proxygen::HTTPServer::IPConfig> IPs = {
        {SocketAddress(FLAGS_IO_QUICKSAVE_CDN_HOST, static_cast<uint16_t >(FLAGS_IO_QUICKSAVE_CDN_PORT), true), Protocol::HTTP}
    };

    long int threads = FLAGS_cdn_threads;
    if (threads <= 0)
    {
        threads = sysconf(_SC_NPROCESSORS_ONLN);
        CHECK(threads > 0);
    }

    proxygen::HTTPServerOptions options;
    options.threads = static_cast<size_t>(threads);
    options.idleTimeout = std::chrono::milliseconds(60000);
    options.shutdownOn = {SIGINT, SIGTERM};
    options.enableContentCompression = false;
    options.handlerFactories = proxygen::RequestHandlerChain()
            .addThen<ProxygenHandlerFactory<ContentHandler>>()
            .build();
    options.h2cEnabled = true;

    proxygen::HTTPServer server(std::move(options));
    server.bind(IPs);

    // Start HTTPServer mainloop in a separate thread
    std::thread t([&]() {
        server.start();
    });

    t.join();
    return 0;
}
