// This file is a part of quicksave project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#pragma once

#include <bean/PerspectiveUpdateRequestBean.h>
#include <bean/MessageBean.h>
#include <databaseBean/DatabaseBeans.h>
#include <folly/io/IOBuf.h>

class PerspectiveUpdateRequest : public PerspectiveUpdateRequestBean
{
public:
    using PerspectiveUpdateRequestBean::PerspectiveUpdateRequestBean;

    std::unique_ptr<folly::IOBuf> handle(RequestContext& ctx)
    {
        MessageBean messageBean;

        std::cout << ::serialize(perspective) << std::endl;

        auto updated_perspective = database::Action::get<PerspectiveBean>(ctx.databaseTransaction, *perspective.perspective_hash);

        //std::cout << ::serialize(*updated_perspective) << std::endl;
/*
        if (updated_perspective) {
            //updated_perspective->update(perspective);
            database::Action::update<PerspectiveBean>(ctx.databaseTransaction, *updated_perspective);
            messageBean.message = "OK";
        } else {
            messageBean.message = "Not found";
        }
  */      return messageBean;
    }
};
