// This file is a part of quicksave project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#pragma once

#include <absl/strings/string_view.h>
#include <qs/database/Provider.h>
#include <qs/database/SqliteConnection.h>


namespace database {

class SqliteProvider : public database::Provider
{
public:
    bool validate(const std::string &providerConnectionString) const
    {
        static const std::string acceptingProtocolString = "sqlite://";
        static const size_t acceptingProtocolStringSize = acceptingProtocolString.size();

        if (providerConnectionString.size() < acceptingProtocolStringSize)
        {
            return false;
        }
        else
        {
            absl::string_view connectionProtocolString = {&providerConnectionString[0], acceptingProtocolStringSize};
            return connectionProtocolString == acceptingProtocolString;
        }
    }

    std::unique_ptr<database::Connection> accept(const std::string &providerConnectionString) const
    {
        static const std::string acceptingProtocolString = "sqlite://";
        static const size_t acceptingProtocolStringSize = acceptingProtocolString.size();

        const std::string connectionString(&providerConnectionString[acceptingProtocolStringSize],
                                           providerConnectionString.size() - acceptingProtocolStringSize);
        return std::unique_ptr<database::Connection>(dynamic_cast<database::Connection *>(new database::SqliteConnection(connectionString)));
    }
};
}