#!/bin/bash
# This file is a part of quicksave project.
# Copyright 2017 Aleksander Gajewski <adiog@quicksave.io>.


cd $(dirname $0)

USER=testuser
PASS=testpass

KEY_PRIV=storage_id_rsa
KEY_PUB=${KEY_PRIV}.pub

SQLITE_MASTER_DB=master.db
SQLITE_MASTER_SQL=master.sql
SQLITE_USER_DB=${USER}.db
SQLITE_USER_SQL=${USER}.sql


function create_key()
{
    if [[ ! -e ${KEY_PRIV} ]];
    then
        ssh-keygen -t rsa -f ${KEY_PRIV} -N ''
    fi
}

function register_key()
{
    cp ${KEY_PUB} authorized_keys
    sudo docker cp authorized_keys storage_quicksave_docker:/root/.ssh/authorized_keys
    rm authorized_keys
}

function create_master_db()
{
    if [[ ! -e ${SQLITE_MASTER_DB} ]];
    then
        ../../../deps/libbeans/sqlbeans/create_database_master_sqlite_create_scripts.sh > ${SQLITE_MASTER_SQL}
        ../../../deps/libbeans/sqlbeans/create_database_master_sqlite_insert_user_scripts.sh ${USER} ${PASS} >> ${SQLITE_MASTER_SQL}

        cat ${SQLITE_MASTER_SQL} | sqlite3 ${SQLITE_MASTER_DB}
    fi
}

function register_master_db()
{
    sudo docker exec storage_quicksave_docker mkdir -p /storage/
    sudo docker cp ${SQLITE_MASTER_DB} storage_quicksave_docker:/storage/${SQLITE_MASTER_DB}
}

function create_slave_db()
{
    if [[ ! -e ${SQLITE_USER_DB} ]];
    then
        ../../../deps/libbeans/sqlbeans/create_database_slave_sqlite_create_scripts.sh > ${SQLITE_USER_SQL}

        cat ${SQLITE_USER_SQL} | sqlite3 ${SQLITE_USER_DB}
    fi
}

function register_user()
{
    sudo docker exec storage_quicksave_docker mkdir -p /storage/users/${USER}
    sudo docker cp ${SQLITE_USER_DB} storage_quicksave_docker:/storage/users/${SQLITE_USER_DB}
}

create_key
register_key
create_master_db
register_master_db
create_slave_db
register_user
