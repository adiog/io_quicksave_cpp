// This file is a part of quicksave project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#ifndef QUICKSAVE_ITEMDELETEREQUEST_H
#define QUICKSAVE_ITEMDELETEREQUEST_H

#include <bean/ItemDeleteRequestBean.h>
#include <bean/MessageBean.h>
#include <databaseBean/DatabaseBeanItem.h>
#include <databaseBean/DatabaseBeanTag.h>
#include <folly/io/IOBuf.h>

class ItemDeleteRequest : public ItemDeleteRequestBean
{
public:
    using ItemDeleteRequestBean::ItemDeleteRequestBean;

    template<typename CTX>
    std::unique_ptr<folly::IOBuf> handle(CTX*ctx)
    {
        MessageBean messageBean;

        DatabaseBean<TagBean>::remove_by(ctx->db.get(), "item_id", item_id);
        DatabaseBean<ItemBean>::remove(ctx->db.get(), item_id);
        messageBean.message = "OK";

        return messageBean;
    }
};

#endif
