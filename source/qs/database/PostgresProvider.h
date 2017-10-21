// This file is a part of quicksave project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#pragma once

#include <absl/strings/string_view.h>

#include <sqlpp11/postgresql/connection.h>

#include <qs/database/Provider.h>


namespace qs {
namespace database {

class PostgresProvider : public qs::database::Provider
{
public:
    bool validate(const std::string &providerConnectionString) const override
    {
        static const std::string acceptingProtocolString = "postgres://";
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
        static const std::string acceptingProtocolString = "postgres://";
        static const size_t acceptingProtocolStringSize = acceptingProtocolString.size();

        const std::string connectionString(&providerConnectionString[acceptingProtocolStringSize],
                                           providerConnectionString.size() - acceptingProtocolStringSize);
        return std::unique_ptr<sqlpp::connection>(new sqlpp::postgresql::connection{});
    }
};
}
}