// This file is a part of quicksave project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#ifndef QUICKSAVE_TAGDELETEREQUEST_H
#define QUICKSAVE_TAGDELETEREQUEST_H

#include <bean/TagDeleteRequestBean.h>
#include <bean/MessageBean.h>
#include <databaseBean/DatabaseBeanTag.h>
#include <folly/io/IOBuf.h>

class TagDeleteRequest : public TagDeleteRequestBean
{
public:
    using TagDeleteRequestBean::TagDeleteRequestBean;

    template<typename CTX>
    std::unique_ptr<folly::IOBuf> handle(CTX*ctx)
    {
        MessageBean messageBean;

        DatabaseBean<TagBean>::remove(ctx->db.get(), tag_id);
        messageBean.message = "OK";

        return messageBean;
    }
};

#endif
