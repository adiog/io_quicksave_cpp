// This file is a part of quicksave project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#pragma once

#include <absl/strings/str_join.h>

#include <folly/io/IOBuf.h>

#include <qsgen/bean/MessageBean.h>
#include <qsgen/bean/PerspectiveUpdateRequestBean.h>

class PerspectiveUpdateRequest : public PerspectiveUpdateRequestBean
{
public:
    using PerspectiveUpdateRequestBean::PerspectiveUpdateRequestBean;

    std::unique_ptr<folly::IOBuf> handle(RequestContext& ctx)
    {
        MessageBean messageBean;

        std::cout << ::serialize(perspective) << std::endl;

        if (ordering)
        {
            std::string orderingJoin = absl::StrJoin(ordering->begin(), ordering->end(), ",");
            perspective.ordering = orderingJoin;
        }

        auto updated_perspective = qsgen::orm::ORM<PerspectiveBean>::get(ctx.databaseTransaction, *perspective.perspective_hash);

        std::cout << ::serialize(*updated_perspective) << std::endl;

        if (updated_perspective)
        {
            updated_perspective->update(perspective);
            qsgen::orm::ORM<PerspectiveBean>::update(ctx.databaseTransaction, *updated_perspective);
            messageBean.message = "OK";
        }
        else
        {
            messageBean.message = "Not found";
        }
        return messageBean;
    }
};
