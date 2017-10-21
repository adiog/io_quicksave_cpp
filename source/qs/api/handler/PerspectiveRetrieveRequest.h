// This file is a part of quicksave project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#pragma once

#include <folly/io/IOBuf.h>

#include <qs/useCase/RetrieveUseCase.h>
#include <qsgen/bean/PerspectiveRetrieveRequestBean.h>
#include <qsgen/bean/PerspectiveRetrieveResponseBean.h>
#include <qsgen/orm/sqlppWrappers.h>

class PerspectiveRetrieveRequest : public PerspectiveRetrieveRequestBean
{
public:
    using PerspectiveRetrieveRequestBean::PerspectiveRetrieveRequestBean;

    std::unique_ptr<folly::IOBuf> handle(RequestContext& ctx)
    {
        PerspectiveRetrieveResponseBean responseBean;
        responseBean.perspectives = qsgen::orm::ORM<PerspectiveBean>::getBy(ctx.databaseTransaction, qsgen::orm::Perspective{}.userHash, *ctx.userBean.user_hash);
        return responseBean;
    }
};
