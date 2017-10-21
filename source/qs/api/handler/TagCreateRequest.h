// This file is a part of quicksave project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#pragma once

#include <folly/io/IOBuf.h>

#include <qsgen/bean/TagCreateRequestBean.h>
#include <qsgen/bean/MessageWithHashBean.h>


class TagCreateRequest : public TagCreateRequestBean
{
public:
    using TagCreateRequestBean::TagCreateRequestBean;

    std::unique_ptr<folly::IOBuf> handle(RequestContext& ctx)
    {
        tag.user_hash = *(ctx.userBean.user_hash);

        std::string tag_hash = qsgen::orm::ORM<TagBean>::insert(ctx.databaseTransaction, tag);

        MessageWithHashBean messageBean;
        messageBean.hash = tag_hash;
        messageBean.message = "OK";
        return messageBean;
    }
};
