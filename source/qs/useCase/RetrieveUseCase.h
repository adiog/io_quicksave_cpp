// This file is a part of quicksave project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#pragma once

#include <qsql/qsqlQuery.h>

#include <qsgen/bean/RetrieveResponseBean.h>


namespace useCase {

class Retrieve
{
public:
    static RetrieveResponseBean getBean(sqlpp::connection &db,
            const std::string &queryParam,
            const std::string &userHashParam)
    {
        RetrieveResponseBean retrieveResponseBean;
        auto ll = List<ItemBean>();
        retrieveResponseBean.items = List<ItemBean>(ll);
        try
        {
            std::__cxx11::string sqlQuery = QsqlQuery::parseQsqlToSql(userHashParam, queryParam);

            DLOG(INFO) << sqlQuery;
/*
            auto metas = qsgen::orm::ORM<MetaBean>::sql(databaseTransactionParam, sqlQuery);
            for (auto &meta : metas)
            {
                ItemBean itemBean;
                itemBean.meta = meta;
                itemBean.tags = database::Action::get_by<TagBean>(databaseTransactionParam, "meta_hash", *meta.meta_hash);
                itemBean.files = database::Action::get_by<FileBean>(databaseTransactionParam, "meta_hash", *meta.meta_hash);
                retrieveResponseBean.items.push_back(itemBean);
            }
            retrieveResponseBean.total = static_cast<int>(metas.size());
*/
        }
        catch (QsqlException &e)
        {
            std::cout << "QSQL FUCKED" << std::endl;
        }
        return retrieveResponseBean;
    }
};
}
