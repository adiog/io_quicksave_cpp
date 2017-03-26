// This file is a part of quicksave project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#ifndef QUICKSAVE_OAUTHMEM_H
#define QUICKSAVE_OAUTHMEM_H

#include <string>
#include <libmemcached/memcached.hpp>
#include <env.h>

class OAuthMemcached
{

    static const int getFlags()
    {
        return 0;
    }

public:
    static std::string get(std::string key)
    {
        std::vector<char> value;
        memcache::Memcache mem(FLAGS_IO_QUICKSAVE_MEMCACHED_CONNECTION_STRING.c_str());
        if (!mem.get(key, value))
        {
            return "";
        }
        return std::string(value.data(), value.size());
    }

    static bool set(std::string key, std::string value, int expireTime=FLAGS_IO_QUICKSAVE_OAUTH_TOKEN_EXPIRE_TIME)
    {
        memcache::Memcache mem(FLAGS_IO_QUICKSAVE_MEMCACHED_CONNECTION_STRING.c_str());
        return mem.set(key, value.c_str(), value.size(), expireTime, getFlags());
    }

    static bool remove(std::string key)
    {
        memcache::Memcache mem(FLAGS_IO_QUICKSAVE_MEMCACHED_CONNECTION_STRING.c_str());
        return mem.remove(key);
    }
};

#endif
