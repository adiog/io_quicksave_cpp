// This file is a part of quicksave project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#pragma once

namespace qs {
namespace server {

class Flags
{
public:
    static void InitFlags()
    {
        FLAGS_IO_QUICKSAVE_MEMCACHED_CONNECTION_STRING =
            std::__cxx11::string("--SERVER=") + FLAGS_IO_QUICKSAVE_MEMCACHED_HOST + std::__cxx11::string(":") + FLAGS_IO_QUICKSAVE_MEMCACHED_PORT;

        DLOG(INFO) << "api:\t" << FLAGS_IO_QUICKSAVE_API_HOST << ":" << FLAGS_IO_QUICKSAVE_API_PORT
                   << std::endl;
        DLOG(INFO) << "master:\t" << FLAGS_IO_QUICKSAVE_MASTER_DATABASE_CONNECTION_STRING << std::endl;
        DLOG(INFO) << "mem:\t" << FLAGS_IO_QUICKSAVE_MEMCACHED_HOST << ":" << FLAGS_IO_QUICKSAVE_MEMCACHED_PORT
                   << std::endl;
        DLOG(INFO) << "mq:\t" << FLAGS_IO_QUICKSAVE_MQ_HOST << ":" << FLAGS_IO_QUICKSAVE_MQ_PORT << std::endl;
    }
};
}
}