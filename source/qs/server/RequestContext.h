// This file is a part of quicksave project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#pragma once

#include <string>

#include <qs/database/Transaction.h>
#include <qsgen/bean/UserBean.h>


struct RequestContext
{
    UserBean userBean;
    std::string session_hash;
    database::Transaction *databaseTransaction;
};
