// This file is a part of quicksave project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#pragma once

#include <absl/strings/string_view.h>

#include <sqlpp11/sqlite3/connection.h>

#include <qs/database/Provider.h>


namespace qs {
namespace database {

class SqliteProvider : public database::Provider
{
public:
    bool validate(const std::string &providerConnectionString) const override
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

    std::unique_ptr<sqlpp::connection> accept(const std::string &providerConnectionString) const override
    {
        static const std::string acceptingProtocolString = "sqlite://";
        static const size_t acceptingProtocolStringSize = acceptingProtocolString.size();

        const std::string connectionString(&providerConnectionString[acceptingProtocolStringSize],
                                           providerConnectionString.size() - acceptingProtocolStringSize);

        const sqlpp::sqlite3::connection_config connectionConfig = parseConnectionString(connectionString);

        return std::unique_ptr<sqlpp::connection>(new sqlpp::sqlite3::connection(connectionConfig));
    }

private:
    sqlpp::sqlite3::connection_config parseConnectionString(const std::string connectionString) const
    {
        sqlpp::sqlite3::connection_config connectionConfig;
        connectionConfig.path_to_database = std::move(connectionString);
        connectionConfig.flags = SQLITE_OPEN_READWRITE;
        connectionConfig.busy_timeout_ms = 3000;
        return std::move(connectionConfig);
    }
};
}
}