// This file is a part of quicksave project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#pragma once

#include <SQLiteCpp/SQLiteCpp.h>
#include <database/SqliteTransaction.h>


namespace database {

class SqliteConnection : public database::Connection
{
public:
    SqliteConnection(const std::string& connectionString)
    {
        databaseConnection.reset(new SQLite::Database(connectionString, SQLite::OPEN_READWRITE));
    }

    std::unique_ptr<database::Transaction> getTransaction()
    {
        return std::make_unique<database::SqliteTransaction>(databaseConnection.get());
    }

private:
    std::unique_ptr<SQLite::Database> databaseConnection;
};
}
