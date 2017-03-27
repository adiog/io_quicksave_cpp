// This file is a part of quicksave project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#pragma once

#include <bean/MetaDeleteRequestBean.h>
#include <bean/MessageBean.h>
#include <databaseBean/DatabaseBeanMeta.h>
#include <databaseBean/DatabaseBeanTag.h>
#include <folly/io/IOBuf.h>

class MetaDeleteRequest : public MetaDeleteRequestBean
{
public:
    using MetaDeleteRequestBean::MetaDeleteRequestBean;

    template<typename CTX>
    std::unique_ptr<folly::IOBuf> handle(CTX*ctx)
    {
        MessageBean messageBean;

        DatabaseBean<TagBean>::remove_by(ctx->db.get(), "meta_hash", meta_hash);
        DatabaseBean<MetaBean>::remove(ctx->db.get(), meta_hash);
        messageBean.message = "OK";

        return messageBean;
    }
};
