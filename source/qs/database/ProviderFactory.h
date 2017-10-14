// This file is a part of quicksave project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#pragma once

#include <qs/database/Connection.h>
#include <qs/database/PostgresProvider.h>
#include <qs/database/Provider.h>
#include <qs/database/SqliteProvider.h>


namespace database {

class ProviderFactory
{
public:
    static std::unique_ptr<database::Connection> create(const std::string& databaseConnectionString)
    {
        static const auto providers = getProviders();

        for (const auto& provider : providers)
        {
            LOG(INFO) << "testing provider";
            if (provider->validate(databaseConnectionString))
            {
                LOG(INFO) << "accepting provider";
                return provider->accept(databaseConnectionString);
            }
        }

        throw std::runtime_error("");
    }

private:
    static std::vector<std::unique_ptr<database::Provider>> getProviders()
    {
        std::vector<std::unique_ptr<database::Provider>> providers;
        providers.emplace_back(dynamic_cast<database::Provider*>(new database::SqliteProvider()));
        providers.emplace_back(dynamic_cast<database::Provider*>(new database::PostgresProvider()));
        return std::move(providers);
    }
};
}
