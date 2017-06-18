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
#include <OAuthHandler.h>
#include <ProxygenHandlerFactory.h>

using folly::EventBase;
using folly::EventBaseManager;
using folly::SocketAddress;

using Protocol = proxygen::HTTPServer::Protocol;


int main(int argc, char* argv[])
{
    gflags::ParseCommandLineFlags(&argc, &argv, true);

    std::cout << "oauth:\t" << FLAGS_IO_QUICKSAVE_OAUTH_HOST << ":" << FLAGS_IO_QUICKSAVE_OAUTH_PORT << std::endl;
    std::cout << "master:\t" << FLAGS_IO_QUICKSAVE_MASTER_DATABASE_CONNECTION_STRING << std::endl;
    std::cout << "mem:\t" << FLAGS_IO_QUICKSAVE_MEMCACHED_HOST << ":" << FLAGS_IO_QUICKSAVE_MEMCACHED_PORT << std::endl;
    std::cout << "mq:\t" << FLAGS_IO_QUICKSAVE_MQ_HOST << ":" << FLAGS_IO_QUICKSAVE_MQ_PORT << std::endl;

    FLAGS_IO_QUICKSAVE_MEMCACHED_CONNECTION_STRING = std::string("--SERVER=") + FLAGS_IO_QUICKSAVE_MEMCACHED_HOST + std::string(":") + FLAGS_IO_QUICKSAVE_MEMCACHED_PORT;

    google::InitGoogleLogging(argv[0]);
    google::InstallFailureSignalHandler();

    std::vector<proxygen::HTTPServer::IPConfig> IPs = {
        {SocketAddress(FLAGS_IO_QUICKSAVE_OAUTH_HOST, static_cast<uint16_t >(FLAGS_IO_QUICKSAVE_OAUTH_PORT), true), Protocol::HTTP}
    };

    long int threads = FLAGS_api_threads;
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
            .addThen<ProxygenHandlerFactory<OAuthHandler>>()
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
