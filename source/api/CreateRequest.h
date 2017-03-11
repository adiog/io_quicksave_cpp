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

        auto richItemBean = PluginEngine::process(item);

        std::cout << "############### " << *richItemBean.item.url << std::endl;

        int id = DatabaseBean<ItemBean>::insert(ctx->db.get(), richItemBean.item);
        for(auto& tag : richItemBean.tags)
        {
            tag.item_id = id;
            DatabaseBean<TagBean>::insert(ctx->db.get(), tag);
        }
        std::cout << *(richItemBean.tags[0].name) << std::endl;
        item.item_id = id;
        createResponseBean.item = item;
        return createResponseBean;
    }
};

#endif
