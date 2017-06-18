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

    std::unique_ptr<folly::IOBuf> handle(RequestContext& ctx)
    {

        RetrieveResponseBean retrieveResponseBean;
        auto ll = List<ItemBean>(0);
        retrieveResponseBean.items = List<ItemBean>(ll);
        try {
            std::string sqlQuery = QsqlQuery::parseQsql2Sql(query);

            Logger::log("%s", sqlQuery.c_str());

            auto metas = DatabaseBean<MetaBean>::sql(ctx.databaseTransaction, sqlQuery);
            for (auto &meta : metas) {
                ItemBean itemBean;
                itemBean.meta = meta;
                itemBean.tags = DatabaseBean<TagBean>::get_by(ctx.databaseTransaction, "meta_hash", *meta.meta_hash);
                itemBean.files = DatabaseBean<FileBean>::get_by(ctx.databaseTransaction, "meta_hash", *meta.meta_hash);
                retrieveResponseBean.items.push_back(itemBean);
            }
            retrieveResponseBean.total = metas.size();
        }
        catch (QsqlException &e)
        {
            std::cout << "QSQL FUCKED" << std::endl;
        }
        return retrieveResponseBean;
    }
};
