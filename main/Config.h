// This file is a part of quicksave project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#ifndef QUICKSAVE_CONFIG_H
#define QUICKSAVE_CONFIG_H

#include <gflags/gflags.h>

#define IO_QUICKSAVE_DB "/io.quicksave.db/db.sqlite3"

DEFINE_int32(http_port, 11000, "Port to listen on with HTTP protocol");
DEFINE_int32(spdy_port, 11001, "Port to listen on with SPDY protocol");
DEFINE_int32(h2_port, 11002, "Port to listen on with HTTP/2 protocol");
DEFINE_string(ip, "localhost", "IP/Hostname to bind to");
DEFINE_int32(threads, 0, "Number of threads to listen on. Numbers <= 0 "
"will use the number of cores on this machine.");


#endif //QUICKSAVE_CONFIG_H
