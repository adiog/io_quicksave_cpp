// This file is a part of quicksave project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#pragma once

#include <folly/io/IOBuf.h>

#include <qs/useCase/RetrieveUseCase.h>

#include <qsgen/abi/PerspectiveRetrieveRequestBean.h>
#include <qsgen/abi/PerspectiveRetrieveResponseBean.h>

#include <qsgen/ORM.h>


namespace qs {

class PerspectiveRetrieveRequest : public PerspectiveRetrieveRequestBean
{
public:
    using PerspectiveRetrieveRequestBean::PerspectiveRetrieveRequestBean;

    std::unique_ptr<folly::IOBuf> handle(RequestContext &ctx)
    {
        PerspectiveRetrieveResponseBean responseBean;
        responseBean.perspectives = ORM<PerspectiveBean>::getBy(ctx.databaseTransaction,
                                                                orm::Perspective{}.userHash,
                                                                *ctx.userBean.user_hash);
        return responseBean;
    }
};
}