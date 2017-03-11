// This file is a part of quicksave project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#ifndef QUICKSAVE_DATABASE_H
#define QUICKSAVE_DATABASE_H

#include <SQLiteCpp/SQLiteCpp.h>
#include <bean/db/ItemDb.h>

struct Database
{
    static List<ItemBean> retrieve(std::string sqlQuery)
    {
        List<ItemBean> result(0);

        try
        {
            // Open a database file
            SQLite::Database    db("db.sqlite3");

            return DatabaseBean<ItemBean>::sql(&db, sqlQuery);
        }
        catch (std::exception& e)
        {
            std::cout << "exception: " << e.what() << std::endl;
        }

        return result;
    }
/*
    static ItemBean insert(ItemBean itemBean)
    {

        try
        {
            // Open a database file
            SQLite::Database    db("db.sqlite3", SQLite::OPEN_READWRITE|SQLite::OPEN_CREATE);

            // Begin transaction
            SQLite::Transaction transaction(db);

            SQLite::Statement   id_query(db, "SELECT MAX(item_id) FROM django_db_djangoitementitiy");

            int nb = id_query.exec();
            int item_id = static_cast<int>(id_query.getColumn(0)) + 1;

            // Compile a SQL query, containing one parameter (index 1)
            SQLite::Statement insert_query(db, "INSERT INTO django_db_djangoitementity (item_id, title, freetext) VALUES(?, ?, ?)");

            insert_query.bind(0, item_id);
            insert_query.bind(1, *itemBean.title);
            insert_query.bind(2, *itemBean.freetext);

            insert_query.exec();

            // Commit transaction
            transaction.commit();

            itemBean.item_id = std::make_optional<int>(item_id);
            return itemBean;

        }
        catch (std::exception& e)
        {
            std::cout << "exception: " << e.what() << std::endl;
        }
    }*/
};

#endif