// This file is a part of quicksave project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#ifndef QUICKSAVE_QSQLQUERY_H
#define QUICKSAVE_QSQLQUERY_H

#include <qsql/qsqlAst.h>
#include <qsql/qsqlLexer.h>
#include <qsql/qsqlBaseListener.h>
#include <qsql/qsqlQuicksaveVisitor.h>
#include <ANTLRInputStream.h>
#include "tree/ParseTree.h"

struct QsqlQuery {
    static std::string parseQsql2Sql(std::string qsqlQuery)
    {
        std::stringstream stream(qsqlQuery);
        antlr4::ANTLRInputStream input(stream);
        qsqlLexer lexer(&input);
        antlr4::CommonTokenStream tokens(&lexer);
        qsqlParser parser(&tokens);

        auto tree = parser.start();
        auto qsqlVisitor = qsqlQuicksaveVisitor();
        auto a = static_cast<AstNode*>(qsqlVisitor.visitStart(tree));
        return a->buildQuery().first;
    }
};

#endif