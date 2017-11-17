// This file is a part of quicksave project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#pragma once

#include <cstdlib>
#include <string>
#include <uuid/uuid.h>

//*Adapted from https://gist.github.com/ne-sachirou/882192
//*std::rand() can be replaced with other algorithms as Xorshift for better perfomance
//*Random seed must be initialized by user

class UUID
{
public:
    static std::string get()
    {
        uuid_t id;
        uuid_generate(id);

        char str[37];
        uuid_unparse(id, str);

        return std::string(str, 36);
    }
};

