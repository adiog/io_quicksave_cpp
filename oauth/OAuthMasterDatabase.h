// This file is a part of quicksave project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#ifndef QUICKSAVE_OAUTHMASTERDATABASE_H
#define QUICKSAVE_OAUTHMASTERDATABASE_H

#include <string>
#include <libmemcached/memcached.hpp>
#include <SQLiteCpp/SQLiteCpp.h>
#include <databaseBean/DatabaseBeanUser.h>

class OAuthMasterDatabase
{
public:
    static std::optional<UserBean> authenticateWithPassword(std::pair<std::string, std::string> credentials)
    {
        std::string masterDatabaseString = FLAGS_IO_QUICKSAVE_DB_MASTER;
        auto db = std::make_unique<SQLite::Database>(masterDatabaseString.c_str(),
                                                     SQLite::OPEN_READWRITE);

        List<UserBean> userBeanList = DatabaseBean<UserBean>::get_by(db.get(), "username", credentials.first);

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

#endif
