// This file is a part of quicksave project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#pragma once

#include <bean/PerspectiveDeleteRequestBean.h>
#include <bean/MessageBean.h>
#include <databaseBean/DatabaseBeans.h>
#include <folly/io/IOBuf.h>

class PerspectiveDeleteRequest : public PerspectiveDeleteRequestBean
{
public:
    using PerspectiveDeleteRequestBean::PerspectiveDeleteRequestBean;

    std::unique_ptr<folly::IOBuf> handle(RequestContext& ctx)
    {
        MessageBean messageBean;

        database::Action::remove<PerspectiveBean>(ctx.databaseTransaction, perspective_hash);
        messageBean.message = "OK";

        return messageBean;
    }
};
