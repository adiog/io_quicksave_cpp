// This file is a part of quicksave project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#ifndef QUICKSAVE_REQUESTCONTEXT_H
#define QUICKSAVE_REQUESTCONTEXT_H

#include <bean/UserBean.h>
#include <bits/unique_ptr.h>
#include <database/Transaction.h>

struct RequestContext
{
    UserBean userBean;
    std::string session_hash;
    database::Transaction *databaseTransaction;
};


#endif  //QUICKSAVE_REQUESTCONTEXT_H
