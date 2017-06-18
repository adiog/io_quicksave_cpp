// This file is a part of quicksave project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#pragma once

#include <bean/TagUpdateRequestBean.h>
#include <bean/MessageBean.h>
#include <databaseBean/DatabaseBeanTag.h>
#include <folly/io/IOBuf.h>

class TagUpdateRequest : public TagUpdateRequestBean
{
public:
    using TagUpdateRequestBean::TagUpdateRequestBean;

    std::unique_ptr<folly::IOBuf> handle(RequestContext& ctx)
    {
        MessageBean messageBean;

        auto updated_tag = DatabaseBean<TagBean>::get(ctx.databaseTransaction, *tag.tag_hash);

        if (updated_tag) {
            updated_tag->update(tag);
            DatabaseBean<TagBean>::update(ctx.databaseTransaction, *updated_tag);
            messageBean.message = "OK";
        } else {
            messageBean.message = "Not found";
        }
        return messageBean;
    }
};
