// This file is a part of quicksave project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#pragma once

#include <bean/TagDeleteRequestBean.h>
#include <bean/MessageBean.h>
#include <databaseBean/DatabaseBeanTag.h>
#include <folly/io/IOBuf.h>

class TagDeleteRequest : public TagDeleteRequestBean
{
public:
    using TagDeleteRequestBean::TagDeleteRequestBean;

    std::unique_ptr<folly::IOBuf> handle(RequestContext& ctx)
    {
        MessageBean messageBean;

        DatabaseBean<TagBean>::remove(ctx.databaseTransaction, tag_hash);
        messageBean.message = "OK";

        return messageBean;
    }
};
