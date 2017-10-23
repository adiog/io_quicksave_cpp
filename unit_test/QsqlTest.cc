// This file is a part of quicksave project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.

#include <gtest/gtest.h>

#include <ANTLRInputStream.h>

#include <qsql/qsqlAst.h>
#include <qsql/qsqlLexer.h>
#include <qsql/qsqlBaseListener.h>
#include <qsql/qsqlQuicksaveVisitor.h>
#include <qsql/qsqlQuery.h>


TEST(QsqlTestSuite, QsqlTestCase) {
    std::stringstream stream("WHERE name ~ 'pattern'");
    antlr4::ANTLRInputStream input(stream);
    qsqlLexer lexer(&input);
    antlr4::CommonTokenStream tokens(&lexer);
    qsqlParser parser(&tokens);

    auto tree = parser.start();
    auto qsqlVisitor = qsqlQuicksaveVisitor();
    auto anyNode = qsqlVisitor.visitStart(tree);
    auto output = FORMAT("SELECT * FROM meta WHERE ((user_hash = '{}') AND ({})) ORDER BY created_at DESC LIMIT {} OFFSET {};", "user_hash", GETSQL(anyNode).first.c_str(), 100, 0);

    EXPECT_EQ(output, "SELECT * FROM meta WHERE ((user_hash = 'user_hash') AND ((meta.name LIKE ('%%pattern%%')))) ORDER BY created_at DESC LIMIT 100 OFFSET 0;");

}
