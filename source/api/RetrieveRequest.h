// This file is a part of quicksave project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#pragma once

#include <bean/RetrieveRequestBean.h>
#include <bean/RetrieveResponseBean.h>
#include <folly/io/IOBuf.h>
#include <qsql/qsqlQuery.h>
#include <databaseBean/DatabaseBeanMeta.h>
#include <databaseBean/DatabaseBeanTag.h>
#include <databaseBean/DatabaseBeanFile.h>

class RetrieveRequest : public RetrieveRequestBean
{
public:
    using RetrieveRequestBean::RetrieveRequestBean;

    template<typename CTX>
    std::unique_ptr<folly::IOBuf> handle(CTX*ctx)
    {

        RetrieveResponseBean retrieveResponseBean;
        auto ll = List<ItemBean>(0);
        retrieveResponseBean.items = List<ItemBean>(ll);
        std::string sqlQuery = QsqlQuery::parseQsql2Sql(query);

        Logger::log("%s", sqlQuery.c_str());

        auto metas = DatabaseBean<MetaBean>::sql(ctx->db.get(), sqlQuery);
        for(auto &meta : metas)
        {
            ItemBean itemBean;
            itemBean.meta = meta;
            itemBean.tags = DatabaseBean<TagBean>::get_by(ctx->db.get(), "meta_hash", *meta.meta_hash);
            itemBean.files = DatabaseBean<FileBean>::get_by(ctx->db.get(), "meta_hash", *meta.meta_hash);
            retrieveResponseBean.items.push_back(itemBean);
        }
        retrieveResponseBean.total = metas.size();

        return retrieveResponseBean;
    }
};
