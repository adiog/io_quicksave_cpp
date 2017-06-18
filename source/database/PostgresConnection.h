// This file is a part of quicksave project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#pragma once

#include <tao/postgres/connection.hpp>
#include <database/PostgresTransaction.h>
#include <util/split>

namespace database {

class PostgresConnection : public database::Connection
{
public:
    PostgresConnection(const std::string& connectionString)
    {
        //auto splitConnectionString = split(connectionString, ':');
        //const std::string host = splitConnectionString[0];
        //const std::string port = splitConnectionString[1];

        databaseConnection = tao::postgres::connection::create(connectionString.c_str());
    }

    std::unique_ptr<database::Transaction> getTransaction()
    {
        //database::Transaction* ptr = new database::PostgresTransaction(databaseConnection->transaction());
        //return ptr; //std::unique_ptr<database::Transaction>(dynamic_cast<database::Transaction*>(new database::PostgresTransaction(databaseConnection->transaction())));
        return std::make_unique<database::PostgresTransaction>(databaseConnection->transaction());
    }

private:
    std::shared_ptr<tao::postgres::connection> databaseConnection;
    std::shared_ptr<tao::postgres::transaction> databaseTransaction;
};
}
