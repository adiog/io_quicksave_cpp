// This file is a part of quicksave project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#pragma once

#include <PluginEngine.h>
#include <bean/CreateRequestBean.h>
#include <bean/CreateResponseBean.h>
#include <bean/InternalCreateRequestBean.h>
#include <databaseBean/DatabaseBeanMeta.h>
#include <databaseBean/DatabaseBeanTag.h>
#include <folly/io/IOBuf.h>
#include <storage/StorageFactory.h>

class CreateRequest : public CreateRequestBean
{
public:
    using CreateRequestBean::CreateRequestBean;

    std::unique_ptr<folly::IOBuf> handle(RequestContext& ctx)
    {
        CreateResponseBean createResponseBean;

        meta.user_hash = *ctx.userBean.user_hash;
        meta.meta_hash = DatabaseBean<MetaBean>::insert(ctx.databaseTransaction, meta);

        InternalCreateRequestBean internalCreateRequestBean;

        internalCreateRequestBean.createRequest = *this;
        internalCreateRequestBean.storageConnectionString = ctx.userBean.storageConnectionString;
        internalCreateRequestBean.databaseConnectionString = ctx.userBean.databaseConnectionString;
        internalCreateRequestBean.keys = DatabaseBean<KeyBean>::get_by(ctx.databaseTransaction, "user_hash", *ctx.userBean.user_hash);

        auto itemBean = PluginEngine::process(internalCreateRequestBean);

        DatabaseBean<MetaBean>::update(ctx.databaseTransaction, itemBean.meta);

        for(auto& tag : itemBean.tags)
        {
            tag.meta_hash = meta.meta_hash;
            tag.user_hash = meta.user_hash;
            DatabaseBean<TagBean>::insert(ctx.databaseTransaction, tag);
        }

        std::unique_ptr<storage::Storage> storage = storage::StorageFactory::create(ctx, ctx.userBean.storageConnectionString);
        storage->init(*meta.meta_hash);
        createResponseBean.item = itemBean;
        return createResponseBean;
    }
};
