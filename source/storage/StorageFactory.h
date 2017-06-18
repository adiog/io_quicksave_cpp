// This file is a part of quicksave project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#pragma once

#include <storage/Storage.h>
#include <storage/LocalStorageProvider.h>
#include <storage/SshfsProvider.h>

namespace storage {

class StorageFactory
{
public:
    static std::vector<std::unique_ptr<storage::StorageProvider>> getProviders()
    {
        std::vector<std::unique_ptr<storage::StorageProvider>> providers;
        providers.emplace_back(dynamic_cast<storage::StorageProvider*>(new storage::LocalStorageProvider()));
        providers.emplace_back(dynamic_cast<storage::StorageProvider*>(new storage::SshfsProvider()));
        return std::move(providers);
    }

    static std::unique_ptr<storage::Storage> create(RequestContext &ctx, const std::string &storageConnectionString)
    {
        static const auto providers = getProviders();

        for (const auto& provider : providers)
        {
            if (provider->validate(storageConnectionString))
            {
                return provider->accept(ctx, storageConnectionString);
            }
        }

        throw std::runtime_error("");
    }
};
}
