// This file is a part of quicksave project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#pragma once

#include <bean/RetrieveResponseBean.h>
#include <database/Action.h>
#include <database/Transaction.h>
#include <qsql/qsqlQuery.h>
#include <util/logger.h>

namespace useCase {

class Retrieve
{
public:
    static RetrieveResponseBean
    getBean(database::Transaction *const &databaseTransactionParam,
            const std::string &queryParam,
            const std::string &userHashParam)
    {
        RetrieveResponseBean retrieveResponseBean;
        auto ll = List<ItemBean>(0);
        retrieveResponseBean.items = List<ItemBean>(ll);
        try
        {
            std::__cxx11::string sqlQuery = QsqlQuery::parseQsqlToSql(userHashParam, queryParam);

            Logger::log("%s", sqlQuery.c_str());

            auto metas = database::Action::sql<MetaBean>(databaseTransactionParam, sqlQuery);
            for (auto &meta : metas)
            {
                ItemBean itemBean;
                itemBean.meta = meta;
                itemBean.tags = database::Action::get_by<TagBean>(databaseTransactionParam, "meta_hash", *meta.meta_hash);
                itemBean.files = database::Action::get_by<FileBean>(databaseTransactionParam, "meta_hash", *meta.meta_hash);
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
}
