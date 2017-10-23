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
            const std::string &userHashParam,
            int limit,
            int offset
    )
    {
        RetrieveResponseBean retrieveResponseBean;
        auto ll = List<ItemBean>();
        retrieveResponseBean.items = List<ItemBean>(ll);
        try
        {
            std::__cxx11::string sqlQuery = QsqlQuery::parseQsqlToSql(userHashParam, queryParam, limit, offset);

            DLOG(INFO) << sqlQuery;

            auto metas = qsgen::orm::ORM<MetaBean>::query(db, sqlQuery);
            for (auto &meta : metas)
            {
                ItemBean itemBean;
                itemBean.meta = meta;
                itemBean.tags = qsgen::orm::ORM<TagBean>::getBy(db, qsgen::orm::Tag{}.metaHash, *meta.meta_hash);
                itemBean.files = qsgen::orm::ORM<FileBean>::getBy(db, qsgen::orm::File{}.metaHash, *meta.meta_hash);
                retrieveResponseBean.items.push_back(itemBean);
            }
            retrieveResponseBean.total = static_cast<int>(metas.size());
        }
        catch (QsqlException &e)
        {
            std::cout << "QSQL FAILED" << std::endl;
        }
        return retrieveResponseBean;
    }
};
}
