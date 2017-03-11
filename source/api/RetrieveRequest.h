// This file is a part of quicksave project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#ifndef QUICKSAVE_RETRIEVEREQUEST_H
#define QUICKSAVE_RETRIEVEREQUEST_H


#include <bean/RetrieveRequestBean.h>
#include <bean/RetrieveResponseBean.h>
#include <folly/io/IOBuf.h>
#include <qsql/qsqlQuery.h>
#include <databaseBean/DatabaseBeanItem.h>
#include <databaseBean/DatabaseBeanTag.h>


class RetrieveRequest : public RetrieveRequestBean
{
public:
    using RetrieveRequestBean::RetrieveRequestBean;

    template<typename CTX>
    std::unique_ptr<folly::IOBuf> handle(CTX*ctx)
    {

        RetrieveResponseBean retrieveResponseBean;
        auto ll = List<RichItemBean>(0);
        retrieveResponseBean.items = List<RichItemBean>(ll);
        std::string sqlQuery = QsqlQuery::parseQsql2Sql(query);

        Logger::log(sqlQuery);

        auto items = DatabaseBean<ItemBean>::sql(ctx->db.get(), sqlQuery);
        for(auto &item : items)
        {
            RichItemBean richItemBean;
            richItemBean.item = item;
            richItemBean.tags = DatabaseBean<TagBean>::get_by(ctx->db.get(), "item_id", *item.item_id);

            retrieveResponseBean.items.push_back(richItemBean);
        }
        retrieveResponseBean.total = items.size();

        return retrieveResponseBean;
    }
};

#endif
