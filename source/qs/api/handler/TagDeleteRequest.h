// This file is a part of quicksave project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#pragma once

#include <folly/io/IOBuf.h>

#include <qsgen/abi/MessageBean.h>
#include <qsgen/abi/TagDeleteRequestBean.h>


namespace qs {

class TagDeleteRequest : public TagDeleteRequestBean
{
public:
    using TagDeleteRequestBean::TagDeleteRequestBean;

    std::unique_ptr<folly::IOBuf> handle(RequestContext &ctx)
    {
        MessageBean messageBean;

        ORM<TagBean>::remove(ctx.databaseTransaction, tag_hash);
        messageBean.message = "OK";

        return messageBean;
    }
};
}