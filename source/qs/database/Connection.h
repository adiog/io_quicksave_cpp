// This file is a part of quicksave project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#pragma once

#include <style>
#include <qs/database/Transaction.h>


namespace database {

class Connection
{
public:
    virtual ~Connection() noexcept = default;
    virtual std::unique_ptr<database::Transaction> getTransaction() abstract;
};
}
