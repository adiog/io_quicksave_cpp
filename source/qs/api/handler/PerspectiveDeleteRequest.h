// This file is a part of quicksave project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#pragma once

#include <folly/io/IOBuf.h>
#include <qsgen/abi/MessageBean.h>
#include <qsgen/abi/PerspectiveDeleteRequestBean.h>


namespace qs {

class PerspectiveDeleteRequest : public PerspectiveDeleteRequestBean
{
public:
    using PerspectiveDeleteRequestBean::PerspectiveDeleteRequestBean;

    std::unique_ptr<folly::IOBuf> handle(RequestContext &ctx)
    {
        MessageBean messageBean;

        ORM<PerspectiveBean>::remove(ctx.databaseTransaction, perspective_hash);
        messageBean.message = "OK";

        return messageBean;
    }
};
}