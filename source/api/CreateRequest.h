// This file is a part of quicksave project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#pragma once

#include <PluginEngine.h>
#include <bean/CreateRequestBean.h>
#include <bean/CreateResponseBean.h>
#include <databaseBean/DatabaseBeanMeta.h>
#include <databaseBean/DatabaseBeanTag.h>
#include <folly/io/IOBuf.h>


class CreateRequest : public CreateRequestBean
{
public:
    using CreateRequestBean::CreateRequestBean;

    template <typename CTX>
    std::unique_ptr<folly::IOBuf> handle(CTX* ctx)
    {
        CreateResponseBean createResponseBean;

        meta.user_hash = *ctx->userBean.user_hash;
        meta.meta_hash = DatabaseBean<MetaBean>::insert(ctx->db.get(), meta);

        auto itemBean = PluginEngine::process(meta);

        DatabaseBean<MetaBean>::update(ctx->db.get(), itemBean.meta);

        for(auto& tag : itemBean.tags)
        {
            tag.meta_hash = meta.meta_hash;
            tag.user_hash = meta.user_hash;
            DatabaseBean<TagBean>::insert(ctx->db.get(), tag);
        }

        createResponseBean.item = itemBean;
        return createResponseBean;
    }
};
