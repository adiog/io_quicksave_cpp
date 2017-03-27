// This file is a part of quicksave project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#pragma once

#include <bean/TagCreateRequestBean.h>
#include <bean/MessageWithHashBean.h>
#include <databaseBean/DatabaseBeanTag.h>
#include <folly/io/IOBuf.h>

class TagCreateRequest : public TagCreateRequestBean
{
public:
    using TagCreateRequestBean::TagCreateRequestBean;

    template<typename CTX>
    std::unique_ptr<folly::IOBuf> handle(CTX*ctx)
    {
        tag.user_hash = *(ctx->userBean.user_hash);

        std::string tag_hash = DatabaseBean<TagBean>::insert(ctx->db.get(), tag);

        MessageWithHashBean messageBean;
        messageBean.hash = tag_hash;
        messageBean.message = "OK";
        return messageBean;
    }
};
