// This file is a part of quicksave project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#ifndef QUICKSAVE_EXCEPTION_H
#define QUICKSAVE_EXCEPTION_H

class ParseError : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};

#endif
