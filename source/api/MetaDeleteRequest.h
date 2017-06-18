// This file is a part of quicksave project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#pragma once

#include <bean/MetaDeleteRequestBean.h>
#include <bean/MessageBean.h>
#include <databaseBean/DatabaseBeanMeta.h>
#include <databaseBean/DatabaseBeanTag.h>
#include <databaseBean/DatabaseBeanFile.h>
#include <databaseBean/DatabaseBeanAction.h>
#include <folly/io/IOBuf.h>
#include <storage/StorageFactory.h>

class MetaDeleteRequest : public MetaDeleteRequestBean
{
public:
    using MetaDeleteRequestBean::MetaDeleteRequestBean;

    std::unique_ptr<folly::IOBuf> handle(RequestContext& ctx)
    {
        MessageBean messageBean;

        std::unique_ptr<storage::Storage> storage = storage::StorageFactory::create(ctx, ctx.userBean.storageConnectionString);
        storage->remove(meta_hash);

        DatabaseBean<TagBean>::remove_by(ctx.databaseTransaction, "meta_hash", meta_hash);
        DatabaseBean<ActionBean>::remove_by(ctx.databaseTransaction, "meta_hash", meta_hash);
        DatabaseBean<FileBean>::remove_by(ctx.databaseTransaction, "meta_hash", meta_hash);
        DatabaseBean<MetaBean>::remove(ctx.databaseTransaction, meta_hash);

        messageBean.message = "OK";

        return messageBean;
    }
};
