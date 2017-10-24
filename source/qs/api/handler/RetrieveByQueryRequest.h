// This file is a part of quicksave project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#pragma once

#include <folly/io/IOBuf.h>

#include <qs/server/RequestContext.h>
#include <qs/useCase/RetrieveUseCase.h>
#include <qs/util/integer.h>

#include <qsgen/abi/RetrieveByQueryRequestBean.h>


namespace qs {

class RetrieveByQueryRequest : public RetrieveByQueryRequestBean
{
public:
    using RetrieveByQueryRequestBean::RetrieveByQueryRequestBean;

    std::unique_ptr<folly::IOBuf> handle(RequestContext &ctx)
    {
        int safe_limit = (limit) ? qs::util::Integer::safe_bounds(*limit, 0, 10) : 10;
        int safe_offset = (offset) ? qs::util::Integer::safe_bounds(*offset, 0, 10) : 0;
        return useCase::Retrieve::getBean(ctx.databaseTransaction, query, *ctx.userBean.user_hash, safe_limit, safe_offset);
    }
};
}