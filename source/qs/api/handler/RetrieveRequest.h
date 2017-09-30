// This file is a part of quicksave project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#pragma once

#include <folly/io/IOBuf.h>

#include <qs/useCase/RetrieveUseCase.h>
#include <qsgen/bean/RetrieveRequestBean.h>
#include <qsgen/bean/RetrieveResponseBean.h>
#include <qsgen/databaseBean/DatabaseBeans.h>
#include <qsql/qsqlQuery.h>


class RetrieveRequest : public RetrieveRequestBean
{
public:
    using RetrieveRequestBean::RetrieveRequestBean;

    std::unique_ptr<folly::IOBuf> handle(RequestContext& ctx)
    {
        return useCase::Retrieve::getBean(ctx.databaseTransaction, query, *ctx.userBean.user_hash);
    }
};
