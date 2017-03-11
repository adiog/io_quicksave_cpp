// This file is a part of quicksave project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#ifndef QUICKSAVE_TAGCREATEREQUEST_H
#define QUICKSAVE_TAGCREATEREQUEST_H

#include <bean/TagCreateRequestBean.h>
#include <bean/MessageWithIdBean.h>
#include <databaseBean/DatabaseBeanTag.h>
#include <folly/io/IOBuf.h>

class TagCreateRequest : public TagCreateRequestBean
{
public:
    using TagCreateRequestBean::TagCreateRequestBean;

    template<typename CTX>
    std::unique_ptr<folly::IOBuf> handle(CTX*ctx)
    {
        tag.user_id = std::make_optional<int>(*(ctx->userBean.user_id));

        int tag_id = DatabaseBean<TagBean>::insert(ctx->db.get(), tag);

        MessageWithIdBean messageBean;
        messageBean.id = tag_id;
        messageBean.message = "OK";
        return messageBean;
    }
};

#endif
