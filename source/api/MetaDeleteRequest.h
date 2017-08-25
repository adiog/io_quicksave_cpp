// This file is a part of quicksave project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#pragma once

#include <bean/MetaDeleteRequestBean.h>
#include <bean/MessageBean.h>
#include <databaseBean/DatabaseBeans.h>
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

        database::Action::remove_by<TagBean>(ctx.databaseTransaction, "meta_hash", meta_hash);
        database::Action::remove_by<ActionBean>(ctx.databaseTransaction, "meta_hash", meta_hash);
        database::Action::remove_by<FileBean>(ctx.databaseTransaction, "meta_hash", meta_hash);
        database::Action::remove<MetaBean>(ctx.databaseTransaction, meta_hash);

        messageBean.message = "OK";

        return messageBean;
    }
};
