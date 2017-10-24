// This file is a part of quicksave project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#pragma once

#include <folly/io/IOBuf.h>

#include <qs/storage/StorageFactory.h>
#include <qsgen/abi/MessageBean.h>
#include <qsgen/abi/MetaDeleteRequestBean.h>

#include <qsgen/ORM.h>


namespace qs {

class MetaDeleteRequest : public MetaDeleteRequestBean
{
public:
    using MetaDeleteRequestBean::MetaDeleteRequestBean;

    std::unique_ptr<folly::IOBuf> handle(RequestContext& ctx)
    {
        MessageBean messageBean;

        std::unique_ptr<storage::Storage> storage = storage::StorageFactory::create(ctx, ctx.userBean.storageConnectionString);
        storage->remove(meta_hash);

        ORM<TagBean>::removeBy(ctx.databaseTransaction, orm::Tag{}.metaHash, meta_hash);
        ORM<ActionBean>::removeBy(ctx.databaseTransaction, orm::Action{}.metaHash, meta_hash);
        ORM<FileBean>::removeBy(ctx.databaseTransaction, orm::File{}.metaHash, meta_hash);
        ORM<MetaBean>::remove(ctx.databaseTransaction, meta_hash);

        messageBean.message = "OK";

        return messageBean;
    }
};
}
