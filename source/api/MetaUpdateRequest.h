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

    std::unique_ptr<folly::IOBuf> handle(RequestContext& ctx)
    {
        MessageBean messageBean;

        auto updated_meta = DatabaseBean<MetaBean>::get(ctx.databaseTransaction, *meta.meta_hash);

        if (updated_meta) {
            updated_meta->update(meta);
            DatabaseBean<MetaBean>::update(ctx.databaseTransaction, *updated_meta);
            messageBean.message = "OK";
        } else {
            messageBean.message = "Not found";
        }
        return messageBean;
    }
};

