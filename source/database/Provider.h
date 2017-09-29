// This file is a part of quicksave project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#pragma once

#include <style>
#include <util/base64.h>
#include <database/Connection.h>


namespace database {

class Provider
{
public:
    virtual ~Provider() noexcept = default;
    virtual bool validate(const std::string& connectionString) const abstract;
    virtual std::unique_ptr<database::Connection> accept(const std::string& connectionString) const abstract;
};
}
