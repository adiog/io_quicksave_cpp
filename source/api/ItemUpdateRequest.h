// This file is a part of quicksave project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#ifndef QUICKSAVE_ITEMUPDATEREQUEST_H
#define QUICKSAVE_ITEMUPDATEREQUEST_H

#include <bean/ItemUpdateRequestBean.h>
#include <bean/MessageBean.h>
#include <databaseBean/DatabaseBeanItem.h>
#include <folly/io/IOBuf.h>

class ItemUpdateRequest : public ItemUpdateRequestBean
{
public:
    using ItemUpdateRequestBean::ItemUpdateRequestBean;

    template<typename CTX>
    std::unique_ptr<folly::IOBuf> handle(CTX*ctx)
    {
        MessageBean messageBean;

        auto updated_item = DatabaseBean<ItemBean>::get(ctx->db.get(), *item.item_id);

        if (updated_item) {
            updated_item->update(item);
            DatabaseBean<ItemBean>::update(ctx->db.get(), *updated_item);
            messageBean.message = "OK";
        } else {
            messageBean.message = "Not found";
        }
        return messageBean;
    }
};

#endif
