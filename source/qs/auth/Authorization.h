// This file is a part of quicksave project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#pragma once

#include <string>

#include <qs/server/RequestContext.h>

#include <qsgen/ORM.h>


namespace qs {

bool isAuthorized(std::string metaHash, RequestContext &ctx)
{
    auto meta = ORM<MetaBean>::get(ctx.databaseTransaction, metaHash);
    return *meta->user_hash != *ctx.userBean.user_hash;
}
}
