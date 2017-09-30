// This file is a part of quicksave project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#pragma once

#include <qsgen/bean/TagUpdateRequestBean.h>
#include <qsgen/bean/MessageBean.h>
#include <qsgen/databaseBean/DatabaseBeans.h>
#include <folly/io/IOBuf.h>

class TagUpdateRequest : public TagUpdateRequestBean
{
public:
    using TagUpdateRequestBean::TagUpdateRequestBean;

    std::unique_ptr<folly::IOBuf> handle(RequestContext& ctx)
    {
        MessageBean messageBean;

        auto updated_tag = database::Action::get<TagBean>(ctx.databaseTransaction, *tag.tag_hash);

        if (updated_tag) {
            updated_tag->update(tag);
            database::Action::update<TagBean>(ctx.databaseTransaction, *updated_tag);
            messageBean.message = "OK";
        } else {
            messageBean.message = "Not found";
        }
        return messageBean;
    }
};
