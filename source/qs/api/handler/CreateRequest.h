// This file is a part of quicksave project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#pragma once

#include <folly/io/IOBuf.h>

#include <PluginEngine.h>
#include <timestamp.h>
#include <qs/storage/StorageFactory.h>
#include <qsgen/bean/CreateRequestBean.h>
#include <qsgen/bean/CreateResponseBean.h>
#include <qsgen/bean/InternalCreateRequestBean.h>
#include <qsgen/databaseBean/DatabaseBeans.h>


class CreateRequest : public CreateRequestBean
{
public:
    using CreateRequestBean::CreateRequestBean;

    std::unique_ptr<folly::IOBuf> handle(RequestContext& ctx)
    {
        CreateResponseBean createResponseBean;

        meta.user_hash = *ctx.userBean.user_hash;
        meta.meta_hash = database::Action::insert<MetaBean>(ctx.databaseTransaction, meta);
        meta.created_at = timestamp();

        InternalCreateRequestBean internalCreateRequestBean;

        internalCreateRequestBean.createRequest = *this;
        internalCreateRequestBean.storageConnectionString = ctx.userBean.storageConnectionString;
        internalCreateRequestBean.databaseConnectionString = ctx.userBean.databaseConnectionString;
        internalCreateRequestBean.keys = database::Action::get_by<KeyBean>(ctx.databaseTransaction, "user_hash", *ctx.userBean.user_hash);

        auto itemBean = PluginEngine::process(internalCreateRequestBean);

        database::Action::update<MetaBean>(ctx.databaseTransaction, itemBean.meta);

        for (auto& tag : itemBean.tags)
        {
            tag.meta_hash = meta.meta_hash;
            tag.user_hash = meta.user_hash;
            database::Action::insert<TagBean>(ctx.databaseTransaction, tag);
        }

        std::unique_ptr<storage::Storage> storage = storage::StorageFactory::create(ctx, ctx.userBean.storageConnectionString);
        storage->init(*meta.meta_hash);
        createResponseBean.item = itemBean;
        return createResponseBean;
    }
};
