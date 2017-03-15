// This file is a part of quicksave project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#ifndef QUICKSAVE_BASE64_H
#define QUICKSAVE_BASE64_H

#include <string>

#include <proxygen/lib/utils/Base64.h>

namespace qs {
class Base64
{
public:
    static std::string decode(std::string based)
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

#endif  //QUICKSAVE_BASE64_H
