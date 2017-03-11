// This file is a part of quicksave project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#ifndef QUICKSAVE_USER_H
#define QUICKSAVE_USER_H

#include <string>

struct User
{
    int user_id;
    std::string username;
/*
    User() = default;
    User(int id, std::string username) : user_id(id), username(username) {}
*/
    bool isAuthorized()
    {
        return true;
    }
};

#endif
