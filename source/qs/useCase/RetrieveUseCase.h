// This file is a part of quicksave project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#pragma once

#include <qsql/qsqlQuery.h>

#include <qsgen/abi/RetrieveResponseBean.h>

namespace qs {
namespace useCase {

class Retrieve
{
public:
    static RetrieveResponseBean getBean(sqlpp::connection &db,
                                        const std::string &queryParam,
                                        const std::string &userHashParam,
                                        int limit,
                                        int offset)
    {
        RetrieveResponseBean retrieveResponseBean;
        auto ll = List<ItemBean>();
        retrieveResponseBean.items = List<ItemBean>(ll);
        try
        {
            std::string sqlQuery = qsql::QsqlQuery::parseQsqlToSql(userHashParam, queryParam, limit, offset);

            DLOG(INFO) << sqlQuery;

            auto metas = ORM<MetaBean>::query(db, sqlQuery);
            for (auto &meta : metas)
            {
                ItemBean itemBean;
                itemBean.meta = meta;
                itemBean.tags = ORM<TagBean>::getBy(db, orm::Tag{}.metaHash, *meta.meta_hash);
                itemBean.files = ORM<FileBean>::getBy(db, orm::File{}.metaHash, *meta.meta_hash);
                retrieveResponseBean.items.push_back(itemBean);
            }
            retrieveResponseBean.total = static_cast<int>(metas.size());
        }
        catch (...) //(QsqlException &e)
        {
            std::cout << "QSQL FAILED" << std::endl;
        }
        return retrieveResponseBean;
    }
};
}
}