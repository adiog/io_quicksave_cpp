// This file is a part of quicksave project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#pragma once

#include <bean/RetrieveRequestBean.h>
#include <bean/RetrieveResponseBean.h>
#include <folly/io/IOBuf.h>
#include <qsql/qsqlQuery.h>
#include <databaseBean/DatabaseBeans.h>

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

            auto metas = database::Action::sql<MetaBean>(ctx.databaseTransaction, sqlQuery);
            for (auto &meta : metas) {
                ItemBean itemBean;
                itemBean.meta = meta;
                itemBean.tags = database::Action::get_by<TagBean>(ctx.databaseTransaction, "meta_hash", *meta.meta_hash);
                itemBean.files = database::Action::get_by<FileBean>(ctx.databaseTransaction, "meta_hash", *meta.meta_hash);
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
