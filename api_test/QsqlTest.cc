#include <qsql/qsqlAst.h>
#include <qsql/qsqlLexer.h>
#include <qsql/qsqlBaseListener.h>
#include <qsql/qsqlQuicksaveVisitor.h>
#include <gtest/gtest.h>
#include <ANTLRInputStream.h>
#include "tree/ParseTree.h"

TEST(QsqlTestSuite, QsqlTestCase) {
    std::stringstream stream("WHERE name ~ 'pattern'");
    antlr4::ANTLRInputStream input(stream);
    qsqlLexer lexer(&input);
    antlr4::CommonTokenStream tokens(&lexer);
    qsqlParser parser(&tokens);

    auto tree = parser.start();
    Ref<qsqlBaseListener> listener(new qsqlBaseListener());
    auto qsqlVisitor = qsqlQuicksaveVisitor();
    //auto b = qsqlVisitor.visitStart(tree);
    //auto a = static_cast<AstNode *>(b);
    //std::cout << a->buildQuery().first << std::endl;

    //delete b;
}
