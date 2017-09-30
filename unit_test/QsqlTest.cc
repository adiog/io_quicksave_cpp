// This file is a part of quicksave project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#include <gtest/gtest.h>

#include <ANTLRInputStream.h>

#include <qsql/qsqlAst.h>
#include <qsql/qsqlLexer.h>
#include <qsql/qsqlBaseListener.h>
#include <qsql/qsqlQuicksaveVisitor.h>


TEST(QsqlTestSuite, QsqlTestCase) {
    std::stringstream stream("WHERE name ~ 'pattern'");
    antlr4::ANTLRInputStream input(stream);
    qsqlLexer lexer(&input);
    antlr4::CommonTokenStream tokens(&lexer);
    qsqlParser parser(&tokens);

    auto tree = parser.start();
    Ref<qsqlBaseListener> listener(new qsqlBaseListener());
    auto qsqlVisitor = qsqlQuicksaveVisitor();
    auto traversedTree = qsqlVisitor.visitStart(tree);
    std::cout << traversedTree.as<AstNode*>()->buildQuery().first << std::endl;
}
