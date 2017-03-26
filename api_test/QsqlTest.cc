#include <qsql/qsqlAst.h>
#include <qsql/qsqlLexer.h>
#include <qsql/qsqlBaseListener.h>
#include <qsql/qsqlQuicksaveVisitor.h>
#include <gtest/gtest.h>
#include <ANTLRInputStream.h>
#include "tree/ParseTree.h"

TEST(QsqlTestSuite, QsqlTestCase)
{
    std::cout << "test" << std::endl;

    //std::stringstream stream("WHERE freetext match 'pattern'");
    std::stringstream stream("WHERE has 'pattern'");
    antlr4::ANTLRInputStream input(stream);
    qsqlLexer lexer(&input);
    antlr4::CommonTokenStream tokens(&lexer);
    qsqlParser parser(&tokens);

//    Ref<antlr4::tree::ParseTree>
            auto tree = parser.start();
    Ref<qsqlBaseListener> listener(new qsqlBaseListener());
    auto qsqlVisitor = qsqlQuicksaveVisitor();
//    auto v = static_cast<AstStart*>(
    auto a = static_cast<AstNode*>(qsqlVisitor.visitStart(tree));
    std::cout << a->buildQuery().first << std::endl;

 //   auto sql = v->buildQuery();
//
  //  std::cout << sql.first << std::endl;

//    tree::ParseTreeWalker::DEFAULT->walk(listener, tree);
}
