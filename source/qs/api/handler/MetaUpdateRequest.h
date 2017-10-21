// This file is a part of quicksave project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#pragma once

#include <qsgen/bean/MetaUpdateRequestBean.h>
#include <qsgen/bean/MessageBean.h>
#include <folly/io/IOBuf.h>

class MetaUpdateRequest : public MetaUpdateRequestBean
{
public:
    using MetaUpdateRequestBean::MetaUpdateRequestBean;

    std::unique_ptr<folly::IOBuf> handle(RequestContext& ctx)
    {
        MessageBean messageBean;

        auto updated_meta = qsgen::orm::ORM<MetaBean>::get(ctx.databaseTransaction, *meta.meta_hash);

        if (updated_meta) {
            updated_meta->update(meta);
            qsgen::orm::ORM<MetaBean>::update(ctx.databaseTransaction, *updated_meta);
            messageBean.message = "OK";
        } else {
            messageBean.message = "Not found";
        }
        return messageBean;
    }
};

