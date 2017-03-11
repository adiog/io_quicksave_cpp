#!/bin/bash
# Copyright 2017 Aleksander Gajewski <adiog@brainfuck.pl>
#   created:  sob, 18 lut 2017, 13:53:28
#   modified: czw, 9 mar 2017, 15:21:19

cd-git-root

mkdir -p generated/databaseBean

function generate()
{
    BEAN_NAME=$1
    python3 extern/beans/cppbeans/generate_db.py extern/qsbeans $BEAN_NAME | clang-format > generated/databaseBean/DatabaseBean${BEAN_NAME}.h
}

generate User
generate Session
generate Item
generate Tag
