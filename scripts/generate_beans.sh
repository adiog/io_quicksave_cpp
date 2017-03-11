#!/bin/bash
# Copyright 2017 Aleksander Gajewski <adiog@brainfuck.pl>
#   created:  sob, 18 lut 2017, 13:53:28
#   modified: czw, 9 mar 2017, 21:07:27

cd-git-root

mkdir -p generated/bean

for bean_file in extern/qsbeans/*.json;
do
    BEAN=$(basename $bean_file);
    python3 extern/beans/cppbeans/generate.py extern/qsbeans $BEAN | clang-format > generated/bean/${BEAN/.json/Bean.h}
done
