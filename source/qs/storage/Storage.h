// This file is a part of quicksave project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#pragma once

#include <string>
#include <style>
#include <util/base64.h>

namespace storage {

class Storage
{
public:
    virtual ~Storage() = default;
    virtual void init(const std::string& metaHash) abstract;
    virtual void save(const std::string& metaHash, const std::string& filename, const std::string& file) abstract;
    virtual std::string read(const FileBean& fileBean) const abstract;
    virtual void remove(const FileBean& fileBean) abstract;
    virtual void remove(const std::string& metaHash) abstract;
};
}
