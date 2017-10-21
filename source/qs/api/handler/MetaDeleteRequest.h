// This file is a part of quicksave project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#pragma once

#include <folly/io/IOBuf.h>

#include <qs/storage/StorageFactory.h>
#include <qsgen/bean/MessageBean.h>
#include <qsgen/bean/MetaDeleteRequestBean.h>

#include <qsgen/orm/sqlppWrappers.h>

class MetaDeleteRequest : public MetaDeleteRequestBean
{
public:
    using MetaDeleteRequestBean::MetaDeleteRequestBean;

    std::unique_ptr<folly::IOBuf> handle(RequestContext& ctx)
    {
        MessageBean messageBean;

        std::unique_ptr<storage::Storage> storage = storage::StorageFactory::create(ctx, ctx.userBean.storageConnectionString);
        storage->remove(meta_hash);

        qsgen::orm::ORM<TagBean>::removeBy(ctx.databaseTransaction, qsgen::orm::Tag{}.metaHash, meta_hash);
        qsgen::orm::ORM<ActionBean>::removeBy(ctx.databaseTransaction, qsgen::orm::Action{}.metaHash, meta_hash);
        qsgen::orm::ORM<FileBean>::removeBy(ctx.databaseTransaction, qsgen::orm::File{}.metaHash, meta_hash);
        qsgen::orm::ORM<MetaBean>::remove(ctx.databaseTransaction, meta_hash);

        messageBean.message = "OK";

        return messageBean;
    }
};
