// This file is a part of quicksave project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#pragma once

#include <folly/io/IOBuf.h>
#include <qsgen/abi/MessageBean.h>
#include <qsgen/abi/MetaUpdateRequestBean.h>


namespace qs {

class MetaUpdateRequest : public MetaUpdateRequestBean
{
public:
    using MetaUpdateRequestBean::MetaUpdateRequestBean;

    std::unique_ptr<folly::IOBuf> handle(RequestContext &ctx)
    {
        MessageBean messageBean;

        auto updated_meta = ORM<MetaBean>::get(ctx.databaseTransaction, *meta.meta_hash);

        if (updated_meta)
        {
            updated_meta->update(meta);
            ORM<MetaBean>::update(ctx.databaseTransaction, *updated_meta);
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