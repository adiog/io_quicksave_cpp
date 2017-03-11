#!/bin/bash

cd-git-root

function generate_parser()
{
    cp extern/qsql/grammar/qsql.g4 .
    java -jar extern/antlr-4.6-complete.jar \
         -o generated/qsql \
         -listener \
         -visitor \
         -Dlanguage=Cpp \
         qsql.g4
    rm qsql.g4
}

function fix_antlr()
{
    sed -e "s#class  qsqlLexer : public antlr4::Lexer {#&\npublic:\nstd::vector<std::string> vs;\nvirtual const std::vector<std::string>\& getChannelNames() const override {\n       return vs;\n    };#" -i generated/qsql/qsqlLexer.h
}

generate_parser
fix_antlr
