// This file is a part of quicksave project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#pragma once

#include <string>
#include <util/base64.h>

class Provider
{
public:
    int accept_base(std::string& meta_hash, std::string & file_hash, std::string& filename, std::string& filebase)
    {
        return this->accept(meta_hash, file_hash, filename, qs::Base64::decode(filebase));
    }

    virtual int accept(std::string& meta_hash, std::string & file_hash, std::string& filename, std::string file) const = 0;
};

