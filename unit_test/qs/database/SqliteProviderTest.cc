// This file is a part of quicksave project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#include <gtest/gtest.h>

#include <qs/database/SqliteProvider.h>
#include <reference_cast>
#include <qsgen/abi/MetaBean.h>
#include <qsgen/ORM.h>
#include <sqlpp11/custom_query.h>

class SqliteProviderTestSuite : public ::testing::Test
{
protected:
    virtual void SetUp()
    {
    }

    virtual void TearDown()
    {
    }

public:
    qs::database::SqliteProvider provider;
};

TEST_F(SqliteProviderTestSuite, ConnectionStringOk)
{
    EXPECT_TRUE(provider.validate("sqlite:///storage/users/testuser.sqlite"));
}

TEST_F(SqliteProviderTestSuite, ConnectionStringNotOk)
{
    EXPECT_FALSE(provider.validate("postgres:///storage/users/testuser.sqlite"));
}

TEST_F(SqliteProviderTestSuite, CRUD)
{
    auto databaseConnectionOwner = provider.accept("sqlite:///quicksave-storage/adiog.db");
    auto& databaseConnection = reference_cast<sqlpp::sqlite3::connection>(databaseConnectionOwner);

    qs::MetaBean metaBean;
    metaBean.name = "label";
    metaBean.text = "abc";
    metaBean.user_hash = "adiog_hash";

    auto metaHash = qs::ORM<qs::MetaBean>::insert(databaseConnection, metaBean);
    std::cout << metaHash << std::endl;

    auto optMetaBean = qs::ORM<qs::MetaBean>::get(databaseConnection, metaHash);
    std::cout << *optMetaBean << std::endl;

    metaBean.text = absl::nullopt;
    qs::ORM<qs::MetaBean>::override(databaseConnection, metaBean);

    std::cout << *qs::ORM<qs::MetaBean>::get(databaseConnection, metaHash) << std::endl;
    qs::MetaBean metaUp;
    metaUp.meta_hash = metaBean.meta_hash;
    metaUp.author = "updateField";

    qs::ORM<qs::MetaBean>::update(databaseConnection, metaUp);

    std::cout << *qs::ORM<qs::MetaBean>::get(databaseConnection, metaHash) << std::endl;

}

TEST_F(SqliteProviderTestSuite, SqlQuery)
{
    auto databaseConnectionOwner = provider.accept("sqlite:///quicksave-storage/adiog.db");
    auto& databaseConnection = reference_cast<sqlpp::sqlite3::connection>(databaseConnectionOwner);

    for(auto& meta : qs::ORM<qs::MetaBean>::query(databaseConnection, "SELECT * FROM meta;"))
    {
        std::cout << meta << std::endl;
    }

}
