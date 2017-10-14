// This file is a part of quicksave project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#include <gtest/gtest.h>

#include <qs/database/SqliteConnection.h>


TEST(SqliteConnectionTestSuite, Constructor)
{
    auto sqliteConnection = database::SqliteConnection("/storage/users/testuser.sqlite");
}

TEST(SqliteConnectionTestSuite, GettingTransaction)
{
    auto sqliteConnection = database::SqliteConnection("/storage/users/testuser.sqlite");
    auto transaction = sqliteConnection.getTransaction();
}
