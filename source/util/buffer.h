//
// Created by adiog on 15.03.17.
//

#ifndef QUICKSAVE_BUFFER_H
#define QUICKSAVE_BUFFER_H

#include <iostream>
#include <bits/unique_ptr.h>
#include <folly/io/IOBuf.h>

class Buffer
{
public:
    static std::string to_string(std::unique_ptr<folly::IOBuf> &iobuf)
    {
        std::string targetString;
        targetString.resize(iobuf->computeChainDataLength());
        int offset = 0;
        folly::IOBuf *buffer = iobuf.get();
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

#endif  //QUICKSAVE_BUFFER_H
