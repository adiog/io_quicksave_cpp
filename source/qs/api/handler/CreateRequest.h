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
#include <qsgen/databaseBean/DatabaseBeans.h>


class CreateRequest : public CreateRequestBean
{
public:
    using CreateRequestBean::CreateRequestBean;

    std::unique_ptr<folly::IOBuf> handle(RequestContext& ctx)
    {
        CreateResponseBean createResponseBean;

        LOG(INFO) << "create";

        meta.user_hash = *ctx.userBean.user_hash;
        meta.meta_hash = database::Action::insert<MetaBean>(ctx.databaseTransaction, meta);
        meta.created_at = qs::util::Timestamp::timestamp();

        LOG(INFO) << "create";
        InternalCreateRequestBean internalCreateRequestBean;

        LOG(INFO) << "create";
        internalCreateRequestBean.createRequest = *this;
        internalCreateRequestBean.storageConnectionString = ctx.userBean.storageConnectionString;
        internalCreateRequestBean.databaseConnectionString = ctx.userBean.databaseConnectionString;
        internalCreateRequestBean.keys = database::Action::get_by<KeyBean>(ctx.databaseTransaction, "user_hash", *ctx.userBean.user_hash);

        LOG(INFO) << "create - before python";
        auto itemBean = PluginEngine::process(internalCreateRequestBean);
        LOG(INFO) << "create - after python";

        database::Action::update<MetaBean>(ctx.databaseTransaction, itemBean.meta);

        LOG(INFO) << "create";
        for (auto& tag : itemBean.tags)
        {
            tag.meta_hash = meta.meta_hash;
            tag.user_hash = meta.user_hash;
            database::Action::insert<TagBean>(ctx.databaseTransaction, tag);
        }
        LOG(INFO) << "before storage";

        std::unique_ptr<storage::Storage> storage = storage::StorageFactory::create(ctx, ctx.userBean.storageConnectionString);
        LOG(INFO) << "after storage";
        storage->init(*meta.meta_hash);
        createResponseBean.item = itemBean;
        LOG(INFO) << "create";
        return createResponseBean;
    }
};
