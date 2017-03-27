// This file is a part of quicksave project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#pragma once

#include <bean/MetaUpdateRequestBean.h>
#include <bean/MessageBean.h>
#include <databaseBean/DatabaseBeanMeta.h>
#include <folly/io/IOBuf.h>

class MetaUpdateRequest : public MetaUpdateRequestBean
{
public:
    using MetaUpdateRequestBean::MetaUpdateRequestBean;

    template<typename CTX>
    std::unique_ptr<folly::IOBuf> handle(CTX*ctx)
    {
        MessageBean messageBean;

        auto updated_meta = DatabaseBean<MetaBean>::get(ctx->db.get(), *meta.meta_hash);

        if (updated_meta) {
            updated_meta->update(meta);
            DatabaseBean<MetaBean>::update(ctx->db.get(), *updated_meta);
            messageBean.message = "OK";
        } else {
            messageBean.message = "Not found";
        }
        return messageBean;
    }
};

