// This file is a part of quicksave project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#ifndef QUICKSAVE_FORMAT_H
#define QUICKSAVE_FORMAT_H

#include <string>
#include <vector>

struct Format
{
    template <typename... Args>
    static std::string format(std::string fmt, Args... args)
    {
        std::vector<char>::size_type size = 256;
        std::vector<char> buf;

        do
        {
            // use snprintf instead of sprintf (to avoid buffer overflows)
            // snprintf returns the required size (without terminating null)
            // if buffer is too small initially: loop should run at most twice
            buf.resize(size + 1);
            size = std::snprintf(
                &buf[0], buf.size(), fmt.c_str(), std::string(args).c_str()...);
        } while (size + 1 > buf.size());

        std::string str = &buf[0];

        return str;
    }
};

#endif
