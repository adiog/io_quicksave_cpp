// This file is a part of quicksave project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#pragma once

#include <bean/RetrieveRequestBean.h>
#include <bean/RetrieveResponseBean.h>
#include <folly/io/IOBuf.h>
#include <qsql/qsqlQuery.h>
#include <databaseBean/DatabaseBeans.h>
#include <useCase/RetrieveUseCase.h>

class RetrieveRequest : public RetrieveRequestBean
{
public:
    using RetrieveRequestBean::RetrieveRequestBean;

    std::unique_ptr<folly::IOBuf> handle(RequestContext& ctx)
    {
        return useCase::Retrieve::getBean(ctx.databaseTransaction, query, *ctx.userBean.user_hash);
    }
};
