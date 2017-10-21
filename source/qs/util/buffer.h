// This file is a part of quicksave project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#pragma once

#include <iostream>
#include <bits/unique_ptr.h>
#include <folly/io/IOBuf.h>


namespace qs {
namespace util {

class Buffer
{
public:
    static std::string to_string(folly::IOBuf &iobuf)
    {
        std::string targetString;
        targetString.resize(iobuf.computeChainDataLength());

        int offset = 0;
        folly::IOBuf *buffer = &iobuf;
        folly::IOBuf *current = buffer;
        do
        {
            std::memcpy(&targetString[offset], current->data(), current->length());
            offset += current->length();
            current = current->next();
        } while (current != buffer);


        return targetString;
    }
};
}
}
