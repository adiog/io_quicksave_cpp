// This file is a part of quicksave project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#pragma once

#include <string>

#include <absl/types/optional.h>

#include <SQLiteCpp/SQLiteCpp.h>
#include <libmemcached/memcached.hpp>

#include <qs/database/Action.h>
#include <qs/database/Connection.h>
#include <qs/database/ProviderFactory.h>
#include <qs/database/Transaction.h>
#include <qsgen/databaseBean/DatabaseBeans.h>


class OAuthMasterDatabase
{
public:
    static absl::optional<UserBean> authenticateWithPassword(std::pair<std::string, std::string> credentials)
    {
        std::string masterDatabaseConnectionString = FLAGS_IO_QUICKSAVE_MASTER_DATABASE_CONNECTION_STRING;

        //std::cout << masterDatabaseConnectionString << std::endl;

        std::unique_ptr<database::Connection> databaseConnection = database::ProviderFactory::create(masterDatabaseConnectionString);
        std::unique_ptr<database::Transaction> databaseTransaction = databaseConnection->getTransaction();

        List<UserBean> userBeanList = database::Action::get_by<UserBean>(databaseTransaction.get(), "username", credentials.first);

        //std::cout << userBeanList.size() << std::endl;


        if ((userBeanList.size() == 1) && (credentials.second == userBeanList[0].password))
        {
            LOG(INFO) << Format::format("Authentication with BasicAuth: SUCCESS (%s,%s)", credentials.first.c_str(), credentials.second.c_str());
            return userBeanList[0];
        }
        else
        {
            LOG(INFO) << Format::format("Authentication with BasicAuth: FAILURE (%s,%s)", credentials.first.c_str(), credentials.second.c_str());
            return absl::nullopt;
        }
    }
};
