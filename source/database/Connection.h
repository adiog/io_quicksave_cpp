// This file is a part of quicksave project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#pragma once

#include <style>
#include <database/Transaction.h>


namespace database {

class Connection
{
public:
    virtual ~Connection() = default;
    virtual std::unique_ptr<database::Transaction> getTransaction() abstract;
};
}