// This file is a part of quicksave project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#ifndef QUICKSAVE_CREATEREQUEST_H
#define QUICKSAVE_CREATEREQUEST_H

#include <PluginEngine.h>
#include <bean/CreateRequestBean.h>
#include <bean/CreateResponseBean.h>
#include <databaseBean/DatabaseBeanItem.h>
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

        item.item_id = DatabaseBean<ItemBean>::insert(ctx->db.get(), item);

        auto richItemBean = PluginEngine::process(item);

        DatabaseBean<ItemBean>::update(ctx->db.get(), richItemBean.item);

        for(auto& tag : richItemBean.tags)
        {
            tag.item_id = item.item_id;
            DatabaseBean<TagBean>::insert(ctx->db.get(), tag);
        }

        createResponseBean.item = item;
        return createResponseBean;
    }
};

#endif
