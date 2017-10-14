#!/usr/bin/env bash

cd $(dirname $0)

export TESTUSER=testuser
export TESTPASS=testpass

export KEY_PRIV=storage_id_rsa
export KEY_PUB=${KEY_PRIV}.pub

export SQLITE_MASTER_DB=master.db
export SQLITE_MASTER_SQL=master.sql
export SQLITE_USER_DB=${TESTUSER}.db
export SQLITE_USER_SQL=${TESTUSER}.sql

function create_key()
{
    if [[ ! -e ${KEY_PRIV} ]];
    then
        ssh-keygen -t rsa -f ${KEY_PRIV} -N ''
    fi
}

function create_master_db()
{
    if [[ ! -e ${SQLITE_MASTER_DB} ]];
    then
        ../../../deps/libbeans/sqlbeans/create_database_master_sqlite_create_scripts.sh > ${SQLITE_MASTER_SQL}
        ../../../deps/libbeans/sqlbeans/create_database_master_sqlite_insert_user_scripts.sh \
            ${TESTUSER} \
            ${TESTPASS} \
            sqlite:///storage/users/${SQLITE_USER_DB} \
            file:///storage/users/${TESTUSER} \
                >> ${SQLITE_MASTER_SQL}

        cat ${SQLITE_MASTER_SQL} | sqlite3 ${SQLITE_MASTER_DB}
    fi
}

function create_slave_db()
{
    if [[ ! -e ${SQLITE_USER_DB} ]];
    then
        ../../../deps/libbeans/sqlbeans/create_database_slave_sqlite_create_scripts.sh > ${SQLITE_USER_SQL}

        cat ${SQLITE_USER_SQL} | sqlite3 ${SQLITE_USER_DB}
    fi
}


create_key
create_master_db
create_slave_db

cat Dockerfile.in | envsubst > Dockerfile
sudo docker build --no-cache -t quicksave/test-storage .
rm Dockerfile

