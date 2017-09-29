// This file is a part of quicksave project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#include <env.h>

#include <folly/io/async/EventBaseManager.h>
#include <proxygen/httpserver/HTTPServer.h>

#include <ProxygenHandlerFactory.h>
#include <server/QuicksaveHandler.h>


int main(int argc, char *argv[])
{
    gflags::ParseCommandLineFlags(&argc, &argv, true);

    google::InitGoogleLogging(argv[0]);
    google::InstallFailureSignalHandler();

    FLAGS_IO_QUICKSAVE_MEMCACHED_CONNECTION_STRING =
            std::string("--SERVER=") + FLAGS_IO_QUICKSAVE_MEMCACHED_HOST + std::string(":") + FLAGS_IO_QUICKSAVE_MEMCACHED_PORT;

    DLOG(INFO) << "api:\t" << FLAGS_IO_QUICKSAVE_API_HOST << ":" << FLAGS_IO_QUICKSAVE_API_PORT << std::endl;
    DLOG(INFO) << "master:\t" << FLAGS_IO_QUICKSAVE_MASTER_DATABASE_CONNECTION_STRING << std::endl;
    DLOG(INFO) << "mem:\t" << FLAGS_IO_QUICKSAVE_MEMCACHED_HOST << ":" << FLAGS_IO_QUICKSAVE_MEMCACHED_PORT << std::endl;
    DLOG(INFO) << "mq:\t" << FLAGS_IO_QUICKSAVE_MQ_HOST << ":" << FLAGS_IO_QUICKSAVE_MQ_PORT << std::endl;

    std::vector<proxygen::HTTPServer::IPConfig> IPs = {
        {folly::SocketAddress(FLAGS_IO_QUICKSAVE_API_HOST, FLAGS_IO_QUICKSAVE_API_PORT, true), proxygen::HTTPServer::Protocol::HTTP}};

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
                                   .addThen<ProxygenHandlerFactory<QuicksaveHandler> >()
                                   .build();
    options.h2cEnabled = true;

    proxygen::HTTPServer server(std::move(options));
    server.bind(IPs);

    std::thread t([&]() {
        server.start();
    });

    t.join();

    return 0;
}
