// This file is a part of quicksave project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#pragma once

#include <folly/io/IOBuf.h>

#include <qs/api-ext/PluginEngine.h>
#include <qs/util/timestamp.h>
#include <qs/storage/StorageFactory.h>
#include <qsgen/bean/CreateRequestBean.h>
#include <qsgen/bean/CreateResponseBean.h>
#include <qsgen/bean/InternalCreateRequestBean.h>
#include <qsgen/orm/sqlppWrappers.h>


class CreateRequest : public CreateRequestBean
{
public:
    using CreateRequestBean::CreateRequestBean;

    std::unique_ptr<folly::IOBuf> handle(RequestContext& ctx)
    {
        CreateResponseBean createResponseBean;

        meta.user_hash = *ctx.userBean.user_hash;
        meta.meta_hash = qsgen::orm::ORM<MetaBean>::insert(ctx.databaseTransaction, meta);
        meta.created_at = qs::util::Timestamp::timestamp();

        InternalCreateRequestBean internalCreateRequestBean;

        internalCreateRequestBean.createRequest = *this;
        internalCreateRequestBean.storageConnectionString = ctx.userBean.storageConnectionString;
        internalCreateRequestBean.databaseConnectionString = ctx.userBean.databaseConnectionString;
        internalCreateRequestBean.keys = qsgen::orm::ORM<KeyBean>::getBy(ctx.databaseTransaction, qsgen::orm::Key{}.userHash, *ctx.userBean.user_hash);

        auto itemBean = PluginEngine::process(internalCreateRequestBean);

        qsgen::orm::ORM<MetaBean>::update(ctx.databaseTransaction, itemBean.meta);

        for (auto& tag : itemBean.tags)
        {
            tag.meta_hash = meta.meta_hash;
            tag.user_hash = meta.user_hash;
            qsgen::orm::ORM<TagBean>::insert(ctx.databaseTransaction, tag);
        }

        std::unique_ptr<storage::Storage> storage = storage::StorageFactory::create(ctx, ctx.userBean.storageConnectionString);
        storage->init(*meta.meta_hash);
        createResponseBean.item = itemBean;

        return createResponseBean;
    }
};
