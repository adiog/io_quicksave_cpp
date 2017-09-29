// This file is a part of quicksave project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#pragma once

#include <bean/PerspectiveCreateRequestBean.h>
#include <bean/MessageWithHashBean.h>
#include <databaseBean/DatabaseBeans.h>
#include <folly/io/IOBuf.h>

class PerspectiveCreateRequest : public PerspectiveCreateRequestBean
{
public:
    using PerspectiveCreateRequestBean::PerspectiveCreateRequestBean;

    std::unique_ptr<folly::IOBuf> handle(RequestContext& ctx)
    {
        perspective.user_hash = *(ctx.userBean.user_hash);

        std::string perspective_hash = database::Action::insert<PerspectiveBean>(ctx.databaseTransaction, perspective);

        MessageWithHashBean messageBean;
        messageBean.hash = perspective_hash;
        messageBean.message = "OK";
        return messageBean;
    }
};
