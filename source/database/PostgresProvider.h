// This file is a part of quicksave project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#pragma once

#include <database/PostgresConnection.h>
#include <database/Provider.h>
#include <string_view>


namespace database {

class PostgresProvider : public database::Provider
{
public:
    bool validate(const std::string &providerConnectionString) const
    {
        static const std::string acceptingProtocolString = "postgres://";
        static const size_t acceptingProtocolStringSize = acceptingProtocolString.size();

        if (providerConnectionString.size() < acceptingProtocolStringSize)
        {
            return false;
        }
        else
        {
            std::string_view connectionProtocolString = {&providerConnectionString[0], acceptingProtocolStringSize};
            return connectionProtocolString == acceptingProtocolString;
        }
    }

    std::unique_ptr<database::Connection> accept(const std::string &providerConnectionString) const
    {
        static const std::string acceptingProtocolString = "postgres://";
        static const size_t acceptingProtocolStringSize = acceptingProtocolString.size();

        const std::string connectionString(&providerConnectionString[acceptingProtocolStringSize],
                                           providerConnectionString.size() - acceptingProtocolStringSize);
        return std::unique_ptr<database::Connection>(dynamic_cast<database::Connection*>(new database::PostgresConnection(connectionString)));
    }
};
}