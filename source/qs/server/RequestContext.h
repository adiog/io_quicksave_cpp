// This file is a part of quicksave project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#pragma once

#include <string>

#include <sqlpp11/sqlite3/connection.h>

#include <qsgen/abi/UserBean.h>

namespace qs {

struct RequestContext
{
    UserBean userBean;
    std::string session_hash;
    sqlpp::connection &databaseTransaction;
};
}