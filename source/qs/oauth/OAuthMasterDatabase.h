// This file is a part of quicksave project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#pragma once

#include <reference_cast>

#include <string>

#include <absl/types/optional.h>

#include <libmemcached/memcached.hpp>

#include <qs/database/ProviderFactory.h>

#include <qsgen/ORM.h>

namespace qs {

class OAuthMasterDatabase
{
public:
    static absl::optional<UserBean> authenticateWithPassword(std::pair<std::string, std::string> credentials)
    {
        std::string masterDatabaseConnectionString = FLAGS_IO_QUICKSAVE_MASTER_DATABASE_CONNECTION_STRING;

        auto databaseConnectionOwner = qs::database::ProviderFactory::create(masterDatabaseConnectionString);
        auto& databaseConnection = reference_cast(databaseConnectionOwner);

        List<UserBean> userBeanList = qs::ORM<UserBean>::getBy(
            databaseConnection,
            qs::orm::User{}.username,
            credentials.first);

        // TODO: SALTY
        if ((userBeanList.size() == 1) && (credentials.second == userBeanList[0].password))
        {
            LOG(INFO) << folly::format("Authentication with BasicAuth: SUCCESS ({},{})", credentials.first.c_str(), credentials.second.c_str());
            return userBeanList[0];
        }
        else
        {
            LOG(INFO) << folly::format("Authentication with BasicAuth: FAILURE ({},{})", credentials.first.c_str(), credentials.second.c_str());
            return absl::nullopt;
        }
    }
};
}