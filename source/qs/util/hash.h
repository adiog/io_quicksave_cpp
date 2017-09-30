// This file is a part of quicksave project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#pragma once

#include <chrono>
#include <sha1.h>


namespace qs {
namespace util {

struct Hash
{
    static std::string get_sha1(const std::string &string)
    {
        std::string s;
        LIBSHA _checksum{};
        _checksum.update(string);
        return _checksum.final();
    }

    static std::string get()
    {
        auto now = std::chrono::high_resolution_clock::now();
        return Hash::get_sha1(std::to_string(
            (std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch())).count()));
    }
};
}
}