// This file is a part of quicksave project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#pragma once

#include <folly/io/IOBuf.h>

#include <qsgen/abi/MessageBean.h>
#include <qsgen/abi/TagUpdateRequestBean.h>


namespace qs {

class TagUpdateRequest : public TagUpdateRequestBean
{
public:
    using TagUpdateRequestBean::TagUpdateRequestBean;

    std::unique_ptr<folly::IOBuf> handle(RequestContext &ctx)
    {
        MessageBean messageBean;

        auto updated_tag = ORM<TagBean>::get(ctx.databaseTransaction, *tag.tag_hash);

        if (updated_tag)
        {
            updated_tag->update(tag);
            ORM<TagBean>::update(ctx.databaseTransaction, *updated_tag);
            messageBean.message = "OK";
        }
        else
        {
            messageBean.message = "Not found";
        }
        return messageBean;
    }
};
}