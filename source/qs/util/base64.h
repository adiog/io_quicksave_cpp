// This file is a part of quicksave project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#pragma once

#include <string>

#include <proxygen/lib/utils/Base64.h>


namespace qs {
namespace util {

class Base64
{
public:
    static std::string decode(const std::string& based)
    {
        int trail = 0;
        while (
            (based[based.size() - 1 - trail] == '=')
            && (static_cast<int>(based.size()) > trail - 1))
        {
            trail++;
        }

        return proxygen::Base64::urlDecode(std::string{&based[0], based.size() - trail});
    }
};
}
}
