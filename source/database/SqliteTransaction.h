// This file is a part of quicksave project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#pragma once

#include <database/Transaction.h>


namespace database {

class SqliteTransaction : public Transaction
{
public:
    ~SqliteTransaction()
    {
        this->destructor();
    }

    SqliteTransaction(SQLite::Database *sqliteConnection)
        : sqliteConnection(sqliteConnection)
    {
       // sqliteConnection->exec("BEGIN TRANSACTION;");
    }

    void do_commit() override
    {
     //   sqliteConnection->exec("COMMIT TRANSACTION;");
    }

    void do_rollback() override
    {
   //     sqliteConnection->exec("ROLLBACK TRANSACTION;");
    }

    TransactionType getRTTI() const override
    {
        return database::TransactionType::Sqlite;
    }

    SQLite::Database * get()
    {
        return sqliteConnection;
    }

private:
    SQLite::Database *sqliteConnection;
};
}
