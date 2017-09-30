// This file is a part of quicksave project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#pragma once

#include <SQLiteCpp/SQLiteCpp.h>
#include <absl/types/optional.h>
//#include <database/PostgresTransaction.h>
#include <qs/database/SqliteTransaction.h>
#include <qs/database/Transaction.h>
#include <qsgen/databaseBean/DatabaseBeans.h>

namespace database {

class Action
{
public:
    template <typename Bean>
    static absl::optional<Bean> get(Transaction *tr, const std::string &hash)
    {
        switch (tr->getRTTI())
        {
            case TransactionType::Sqlite:
            {
                SQLite::Database *casted_tr =
                    static_cast<database::SqliteTransaction *>(tr)->get();
                return DatabaseBean<SQLite::Database, Bean>::get(casted_tr, hash);
            }
            /*
            case TransactionType::Postgres:
            {
                tao::postgres::transaction *casted_tra =
                    dynamic_cast<database::PostgresTransaction *>(tr)->get();
                return DatabaseBean<tao::postgres::transaction, Bean>::get(tra, hash);
            }
*/
            default:
                throw std::runtime_error("");
        }
    }

    template <typename Bean, typename FIELD_VALUE>
    static List<Bean> get_by(Transaction *tr, std::string field, FIELD_VALUE field_value)
    {
        switch (tr->getRTTI())
        {
            case TransactionType::Sqlite:
            {
                SQLite::Database *casted_tr =
                    dynamic_cast<database::SqliteTransaction *>(tr)->get();
                return DatabaseBean<SQLite::Database, Bean>::get_by(casted_tr, field, field_value);
            }
            /*
            case TransactionType::Postgres:
            {
                tao::postgres::transaction *casted_tr =
                    dynamic_cast<database::PostgresTransaction *>(tr)->get();
                return DatabaseBean<tao::postgres::transaction, Bean>::get_by(casted_tr, field, field_value);
            }
*/
            default:
                throw std::runtime_error("");
        }
    }

    template <typename Bean>
    static void remove(Transaction *tr, std::string hash)
    {
        switch (tr->getRTTI())
        {
            case TransactionType::Sqlite:
            {
                SQLite::Database *casted_tr =
                    dynamic_cast<database::SqliteTransaction *>(tr)->get();
                return DatabaseBean<SQLite::Database, Bean>::remove(casted_tr, hash);
            }
            /*
            case TransactionType::Postgres:
            {
                tao::postgres::transaction *casted_tr =
                    dynamic_cast<database::PostgresTransaction *>(tr)->get();
                return DatabaseBean<tao::postgres::transaction, Bean>::remove(casted_tr, hash);
            }
*/
            default:
                throw std::runtime_error("");
        }
    }

    template <typename Bean, typename FIELD_VALUE>
    static void remove_by(Transaction *tr, std::string field, FIELD_VALUE field_value)
    {
        switch (tr->getRTTI())
        {
            case TransactionType::Sqlite:
            {
                SQLite::Database *casted_tr =
                    dynamic_cast<database::SqliteTransaction *>(tr)->get();
                return DatabaseBean<SQLite::Database, Bean>::remove_by(casted_tr, field, field_value);
            }
            /*
            case TransactionType::Postgres:
            {
                tao::postgres::transaction *casted_tr =
                    dynamic_cast<database::PostgresTransaction *>(tr)->get();
                return DatabaseBean<tao::postgres::transaction, Bean>::remove_by(casted_tr, field, field_value);
            }
*/
            default:
                throw std::runtime_error("");
        }
    }

    template <typename Bean>
    static List<Bean> sql(Transaction *tr, std::string sql)
    {
        switch (tr->getRTTI())
        {
            case TransactionType::Sqlite:
            {
                SQLite::Database *casted_tr =
                    dynamic_cast<database::SqliteTransaction *>(tr)->get();
                return DatabaseBean<SQLite::Database, Bean>::sql(casted_tr, sql);
            }
            /*
            case TransactionType::Postgres:
            {
                tao::postgres::transaction *casted_tr =
                    dynamic_cast<database::PostgresTransaction *>(tr)->get();
                return DatabaseBean<tao::postgres::transaction, Bean>::sql(casted_tr, sql);
            }
*/
            default:
                throw std::runtime_error("");
        }
    }

    template <typename Bean>
    static std::string insert(Transaction *tr, Bean bean)
    {
        switch (tr->getRTTI())
        {
            case TransactionType::Sqlite:
            {
                SQLite::Database *casted_tr =
                    dynamic_cast<database::SqliteTransaction *>(tr)->get();
                return DatabaseBean<SQLite::Database, Bean>::insert(casted_tr, bean);
            }
            /*
            case TransactionType::Postgres:
            {
                tao::postgres::transaction *casted_tr =
                    dynamic_cast<database::PostgresTransaction *>(tr)->get();
                return DatabaseBean<tao::postgres::transaction, Bean>::insert(casted_tr, bean);
            }
*/
            default:
                throw std::runtime_error("");
        }
    }

    template <typename Bean>
    static void insert_with_pk(Transaction *tr, Bean bean)
    {
        switch (tr->getRTTI())
        {
            case TransactionType::Sqlite:
            {
                SQLite::Database *casted_tr =
                    dynamic_cast<database::SqliteTransaction *>(tr)->get();
                return DatabaseBean<SQLite::Database, Bean>::insert_with_pk(casted_tr, bean);
            }
            /*
            case TransactionType::Postgres:
            {
                tao::postgres::transaction *casted_tr =
                    dynamic_cast<database::PostgresTransaction *>(tr)->get();
                return DatabaseBean<tao::postgres::transaction, Bean>::insert_with_pk(casted_tr, bean);
            }
*/
            default:
                throw std::runtime_error("");
        }
    }

    template <typename Bean>
    static void update(Transaction *tr, Bean bean)
    {
        switch (tr->getRTTI())
        {
            case TransactionType::Sqlite:
            {
                SQLite::Database *casted_tr =
                    dynamic_cast<database::SqliteTransaction *>(tr)->get();
                return DatabaseBean<SQLite::Database, Bean>::update(casted_tr, bean);
            }
            /*
            case TransactionType::Postgres:
            {
                tao::postgres::transaction *casted_tr =
                    dynamic_cast<database::PostgresTransaction *>(tr)->get();
                return DatabaseBean<tao::postgres::transaction, Bean>::update(casted_tr, bean);
            }
*/
            default:
                throw std::runtime_error("");
        }
    }
};
}
