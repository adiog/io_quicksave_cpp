// This file is a part of quicksave project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#include <gtest/gtest.h>

#include <qs/database/SqliteProvider.h>

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
    database::SqliteProvider provider;
};

TEST_F(SqliteProviderTestSuite, ConnectionStringOk)
{
    EXPECT_TRUE(provider.validate("sqlite:///storage/users/testuser.sqlite"));
}

TEST_F(SqliteProviderTestSuite, ConnectionStringNotOk)
{
    EXPECT_FALSE(provider.validate("postgres:///storage/users/testuser.sqlite"));
}

TEST_F(SqliteProviderTestSuite, Accept)
{
    auto databaseConnection = provider.accept("sqlite:///storage/users/testuser.sqlite");
}
