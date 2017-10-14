// This file is a part of quicksave project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#pragma once

#include <memory>

#include <SQLiteCpp/SQLiteCpp.h>

#include <qs/database/Connection.h>
#include <qs/database/SqliteTransaction.h>


namespace database {

class SqliteConnection : public database::Connection
{
public:
    SqliteConnection(const std::string& connectionString)
    {
        static const int TIMEOUT = 3000;
        databaseConnection.reset(new SQLite::Database(connectionString, SQLite::OPEN_READWRITE, TIMEOUT));
    }

    std::unique_ptr<database::Transaction> getTransaction()
    {
        return std::make_unique<database::SqliteTransaction>(databaseConnection.get());
    }

private:
    std::unique_ptr<SQLite::Database> databaseConnection;
};
}
