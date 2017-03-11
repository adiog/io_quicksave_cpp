// This file is a part of quicksave project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#ifndef QUICKSAVE_TAGUPDATEREQUEST_H
#define QUICKSAVE_TAGUPDATEREQUEST_H

#include <bean/TagUpdateRequestBean.h>
#include <bean/MessageBean.h>
#include <databaseBean/DatabaseBeanTag.h>
#include <folly/io/IOBuf.h>

class TagUpdateRequest : public TagUpdateRequestBean
{
public:
    using TagUpdateRequestBean::TagUpdateRequestBean;

    template<typename CTX>
    std::unique_ptr<folly::IOBuf> handle(CTX*ctx)
    {
        MessageBean messageBean;

        auto updated_tag = DatabaseBean<TagBean>::get(ctx->db.get(), *tag.tag_id);

        if (updated_tag) {
            updated_tag->update(tag);
            DatabaseBean<TagBean>::update(ctx->db.get(), *updated_tag);
            messageBean.message = "OK";
        } else {
            messageBean.message = "Not found";
        }
        return messageBean;
    }
};

#endif
