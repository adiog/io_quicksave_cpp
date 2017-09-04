// This file is a part of quicksave project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#pragma once

#include <string>
#include <libmemcached/memcached.hpp>
#include <SQLiteCpp/SQLiteCpp.h>
#include <databaseBean/DatabaseBeans.h>
#include <database/Transaction.h>
#include <database/Connection.h>
#include <database/ProviderFactory.h>
#include <database/Action.h>

class OAuthMasterDatabase
{
public:
    static std::optional<UserBean> authenticateWithPassword(std::pair<std::string, std::string> credentials)
    {
        std::string masterDatabaseConnectionString = FLAGS_IO_QUICKSAVE_MASTER_DATABASE_CONNECTION_STRING;

        //std::cout << masterDatabaseConnectionString << std::endl;

        std::unique_ptr<database::Connection> databaseConnection = database::ProviderFactory::create(masterDatabaseConnectionString);
        std::unique_ptr<database::Transaction> databaseTransaction = databaseConnection->getTransaction();

        List<UserBean> userBeanList = database::Action::get_by<UserBean>(databaseTransaction.get(), "username", credentials.first);

        //std::cout << userBeanList.size() << std::endl;


        if ((userBeanList.size() == 1) && (credentials.second == userBeanList[0].password))
        {
            Logger::log("Authentication with BasicAuth: SUCCESS (%s,%s)", credentials.first.c_str(), credentials.second.c_str());
            return userBeanList[0];
        }
        else
        {
            Logger::log("Authentication with BasicAuth: FAILURE (%s,%s)", credentials.first.c_str(), credentials.second.c_str());
            return std::none;
        }
    }
};

