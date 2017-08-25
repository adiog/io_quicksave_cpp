// This file is a part of quicksave project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#pragma once

#include <database/Connection.h>
#include <database/Transaction.h>
#include <tao/postgres/transaction.hpp>

namespace database {

class PostgresTransaction : public database::Transaction
{
public:
    ~PostgresTransaction()
    {
        this->destructor();
    }

    PostgresTransaction(std::shared_ptr<tao::postgres::transaction> postgresTransaction)
    {
        this->postgresTransaction = postgresTransaction;
    }

    void do_commit() override
    {
        postgresTransaction->commit();
    }

    void do_rollback() override
    {
        postgresTransaction->rollback();
    }

    TransactionType getRTTI() const override
    {
        return TransactionType::Postgres;
    }

    tao::postgres::transaction* get()
    {
        return postgresTransaction.get();
    }

private:
    std::shared_ptr<tao::postgres::transaction> postgresTransaction;
};
}
