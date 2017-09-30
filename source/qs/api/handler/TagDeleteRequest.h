// This file is a part of quicksave project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#pragma once

#include <qsgen/bean/TagDeleteRequestBean.h>
#include <qsgen/bean/MessageBean.h>
#include <qsgen/databaseBean/DatabaseBeans.h>
#include <folly/io/IOBuf.h>

class TagDeleteRequest : public TagDeleteRequestBean
{
public:
    using TagDeleteRequestBean::TagDeleteRequestBean;

    std::unique_ptr<folly::IOBuf> handle(RequestContext& ctx)
    {
        MessageBean messageBean;

        database::Action::remove<TagBean>(ctx.databaseTransaction, tag_hash);
        messageBean.message = "OK";

        return messageBean;
    }
};
