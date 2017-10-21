// This file is a part of quicksave project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#include <gtest/gtest.h>

#include <qs/database/SqliteProvider.h>
#include <reference_cast>
#include <qsgen/bean/MetaBean.h>
#include <qsgen/orm/sqlppTables.h>
#include <qsgen/orm/sqlppWrappers.h>
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

template <typename DatabaseConnection>
static List<MetaBean> sql(DatabaseConnection &db, std::string sql)
{
    const auto table = qsgen::orm::Meta();

    List<MetaBean> result(0);

    try
    {
        for (const auto& row : db(
                sqlpp::custom_query(sqlpp::verbatim(sql))
                        .with_result_type_of(sqlpp::select(all_of(qsgen::orm::Meta{})))))
        {
            result.emplace_back(qsgen::orm::ORM<MetaBean>::constructor(row));
        }
    }
    catch (std::exception& e)
    {
        std::cout << "exception: " << e.what() << std::endl;
    }

    return result;
}

TEST_F(SqliteProviderTestSuite, Accept)
{
    auto databaseConnectionOwner = provider.accept("sqlite:///quicksave-storage/adiog.db");
    auto& databaseConnection = reference_cast<sqlpp::sqlite3::connection>(databaseConnectionOwner);

    for(auto& meta : sql(databaseConnection, "SELECT * FROM meta;"))
    {
        std::cout << meta << std::endl;
    }

}
