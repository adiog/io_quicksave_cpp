// This file is a part of quicksave project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#pragma once

#include <stdexcept>


namespace qs {
namespace server {

class ParseError : public std::runtime_error
{
public:
    using std::runtime_error::runtime_error;
};

struct HttpStatus
{
    int statusCode;
};
}
}
