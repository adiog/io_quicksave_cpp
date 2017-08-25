// This file is a part of quicksave project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#pragma once

#include <style>

namespace database {

enum class TransactionType
{
    Sqlite, Postgres
};

class Transaction
{
public:
    virtual ~Transaction() = default;

    void destructor()
    {
        if (!closed)
        {
            this->do_rollback();
        }
    }

    void commit()
    {
        closed = true;
        this->do_commit();
    }

    void rollback()
    {
        closed = true;
        this->do_rollback();
    }

    virtual void do_commit() abstract;
    virtual void do_rollback() abstract;

    virtual TransactionType getRTTI() const = 0;


private:
    bool closed = false;
};
}
