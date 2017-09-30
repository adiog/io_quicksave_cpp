// This file is a part of quicksave project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#include <env.h>

#include <proxygen/httpserver/HTTPServer.h>

#include <qs/server/Config.h>
#include <qs/server/Flags.h>
#include <qs/server/OAuthServer.h>


int main(int argc, char* argv[])
{
    gflags::ParseCommandLineFlags(&argc, &argv, true);

    google::InitGoogleLogging(argv[0]);
    google::InstallFailureSignalHandler();

    qs::server::Flags::InitFlags();

    auto serverIPs = qs::server::Config::getHttpIp(FLAGS_IO_QUICKSAVE_OAUTH_HOST, static_cast<uint16_t>(FLAGS_IO_QUICKSAVE_OAUTH_PORT));
    auto serverThreads = qs::server::Config::getThreads(FLAGS_oauth_threads);
    auto serverOptions = qs::server::Config::getOptions<qs::server::OAuthServer>(serverThreads);

    proxygen::HTTPServer server(std::move(serverOptions));
    server.bind(serverIPs);

    std::thread t([&]() {
        server.start();
    });

    t.join();

    return 0;
}
