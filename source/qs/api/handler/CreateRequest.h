// This file is a part of quicksave project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#pragma once

#include <folly/io/IOBuf.h>

#include <qs/api-ext/PluginEngine.h>
#include <qs/storage/StorageFactory.h>
#include <qs/util/timestamp.h>
#include <qsgen/ORM.h>
#include <qsgen/abi/CreateRequestBean.h>
#include <qsgen/abi/CreateResponseBean.h>
#include <qsgen/abi/InternalCreateRequestBean.h>


namespace qs {

class CreateRequest : public CreateRequestBean
{
public:
    using CreateRequestBean::CreateRequestBean;

    std::unique_ptr<folly::IOBuf> handle(RequestContext &ctx)
    {
        CreateResponseBean createResponseBean;

        meta.user_hash = *ctx.userBean.user_hash;
        meta.meta_hash = ORM<MetaBean>::insert(ctx.databaseTransaction, meta);
        meta.created_at = qs::util::Timestamp::timestamp();

        InternalCreateRequestBean internalCreateRequestBean;

        internalCreateRequestBean.createRequest = *static_cast<CreateRequestBean *>(this);
        internalCreateRequestBean.storageConnectionString = ctx.userBean.storageConnectionString;
        internalCreateRequestBean.databaseConnectionString = ctx.userBean.databaseConnectionString;
        internalCreateRequestBean.keys = ORM<KeyBean>::getBy(ctx.databaseTransaction, qs::orm::Key{}.userHash, *ctx.userBean.user_hash);

        auto itemBean = PluginEngine::process(internalCreateRequestBean);

        ORM<MetaBean>::update(ctx.databaseTransaction, itemBean.meta);

        for (auto &tag : itemBean.tags)
        {
            tag.meta_hash = meta.meta_hash;
            tag.user_hash = meta.user_hash;
            ORM<TagBean>::insert(ctx.databaseTransaction, tag);
        }

        std::unique_ptr<storage::Storage> storage = storage::StorageFactory::create(ctx,
                                                                                    ctx.userBean.storageConnectionString);
        storage->init(*meta.meta_hash);
        createResponseBean.item = itemBean;

        return createResponseBean;
    }
};
}