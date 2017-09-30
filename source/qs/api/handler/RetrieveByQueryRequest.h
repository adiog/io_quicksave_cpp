// This file is a part of quicksave project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#pragma once

#include <folly/io/IOBuf.h>

#include <qs/server/RequestContext.h>
#include <qs/useCase/RetrieveUseCase.h>
#include <qsgen/bean/RetrieveByQueryRequestBean.h>


class RetrieveByQueryRequest : public RetrieveByQueryRequestBean
{
public:
    using RetrieveByQueryRequestBean::RetrieveByQueryRequestBean;

    std::unique_ptr<folly::IOBuf> handle(RequestContext& ctx)
    {
        return useCase::Retrieve::getBean(ctx.databaseTransaction, query, *ctx.userBean.user_hash);
    }
};
