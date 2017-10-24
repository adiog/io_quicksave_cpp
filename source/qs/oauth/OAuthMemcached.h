// This file is a part of quicksave project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#pragma once

#include <env.h>

#include <string>

#include <libmemcached/memcached.hpp>

namespace qs {

class OAuthMemcached
{
    static const uint32_t getFlags()
    {
        return 0;
    }

public:
    static std::string get(const std::string& key)
    {
        std::vector<char> value;
        memcache::Memcache mem(FLAGS_IO_QUICKSAVE_MEMCACHED_CONNECTION_STRING);
        if (!mem.get(key, value))
        {
            return "";
        }
        return std::string(value.data(), value.size());
    }

    static bool set(const std::string& key, const std::string& value, int expireTime = FLAGS_IO_QUICKSAVE_OAUTH_TOKEN_EXPIRE_TIME)
    {
        // TODO: memcache connection handle per thread!!
        memcache::Memcache mem(FLAGS_IO_QUICKSAVE_MEMCACHED_CONNECTION_STRING);
        return mem.set(key, value.c_str(), value.size(), expireTime, getFlags());
    }

    static bool remove(const std::string& key)
    {
        memcache::Memcache mem(FLAGS_IO_QUICKSAVE_MEMCACHED_CONNECTION_STRING);
        return mem.remove(key);
    }

    static bool touch(const std::string& key, int expireTime = FLAGS_IO_QUICKSAVE_OAUTH_TOKEN_EXPIRE_TIME)
    {
        memcache::Memcache mem(FLAGS_IO_QUICKSAVE_MEMCACHED_CONNECTION_STRING);
        return mem.touch(key, expireTime);
    }
};
}