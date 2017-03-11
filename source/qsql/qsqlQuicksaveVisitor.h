
// Generated from qsql/qsql.g4 by ANTLR 4.6

#pragma once


#include "antlr4-runtime.h"
#include "qsql/qsqlVisitor.h"


/**
 * This class provides an empty implementation of qsqlVisitor, which can be
 * extended to create a visitor which only needs to handle a subset of the available methods.
 */
class  qsqlQuicksaveVisitor : public qsqlVisitor {
public:

  virtual antlrcpp::Any visitStart(qsqlParser::StartContext *ctx) override {
    return dynamic_cast<AstNode*>(new AstStart(ctx->children[0]->accept(this)));
  }

  virtual antlrcpp::Any visitQsql_query(qsqlParser::Qsql_queryContext *ctx) override {
    return dynamic_cast<AstNode *>
    (new AstQsqlQuery(ctx->children[1]->accept(this)));
  }

  virtual antlrcpp::Any visitPredicate(qsqlParser::PredicateContext *ctx) override {
    return dynamic_cast<AstNode *>(new AstPredicate(
            ctx->children[0]->accept(this)
    ));
  }

  virtual antlrcpp::Any visitOr_predicate(qsqlParser::Or_predicateContext *ctx) override {
    return  dynamic_cast<AstNode *>(new AstOrPredicate(
            ctx->children[1]->accept(this),
            ctx->children[3]->accept(this)
    ));
  }

  virtual antlrcpp::Any visitAnd_predicate(qsqlParser::And_predicateContext *ctx) override {
    return dynamic_cast<AstNode *>(new AstAndPredicate(
            ctx->children[1]->accept(this),
            ctx->children[3]->accept(this)
    ));
  }

  virtual antlrcpp::Any visitNot_predicate(qsqlParser::Not_predicateContext *ctx) override {
    return dynamic_cast<AstNode *>(new AstNotPredicate(
            ctx->children[1]->accept(this)
    ));
  }

  virtual antlrcpp::Any visitPar_predicate(qsqlParser::Par_predicateContext *ctx) override {
    return dynamic_cast<AstNode *>(new AstParPredicate(
            ctx->children[1]->accept(this)
    ));
  }

  virtual antlrcpp::Any visitQs_predicate(qsqlParser::Qs_predicateContext *ctx) override {
    return dynamic_cast<AstNode *>(new AstQsPredicate(
            ctx->children[0]->accept(this)
    ));
  }

  virtual antlrcpp::Any visitQs_has_predicate(qsqlParser::Qs_has_predicateContext *ctx) override {
    return dynamic_cast<AstNode *>(new AstQsHasPredicate(
            ctx->children[1]->accept(this)
    ));
  }

  virtual antlrcpp::Any visitQs_string_predicate(qsqlParser::Qs_string_predicateContext *ctx) override {
    return dynamic_cast<AstNode *>(new AstQsStringPredicate(
            ctx->children[0]->accept(this)
    ));
  }

  virtual antlrcpp::Any visitQs_string_equal_predicate(qsqlParser::Qs_string_equal_predicateContext *ctx) override {
    return dynamic_cast<AstNode *>(new AstQsStringEqualPredicate(
            ctx->children[0]->accept(this),
            ctx->children[2]->accept(this)
    ));
  }

  virtual antlrcpp::Any visitQs_string_in_predicate(qsqlParser::Qs_string_in_predicateContext *ctx) override {
    return dynamic_cast<AstNode *>(new AstQsStringInPredicate(
            ctx->children[0]->accept(this),
            ctx->children[2]->accept(this)
    ));
  }

  virtual antlrcpp::Any visitQs_string_match_predicate(qsqlParser::Qs_string_match_predicateContext *ctx) override {
    return dynamic_cast<AstNode *>(new AstQsStringMatchPredicate(
            ctx->children[0]->accept(this),
            ctx->children[2]->accept(this)
    ));
  }

  virtual antlrcpp::Any visitQs_string_with_parenthesis(qsqlParser::Qs_string_with_parenthesisContext *ctx) override {
    return dynamic_cast<AstNode *>(new AstQsStringWithParenthesis(
            ctx->children[1]->accept(this)
    ));
  }

  virtual antlrcpp::Any visitQs_string(qsqlParser::Qs_stringContext *ctx) override {
    return dynamic_cast<AstNode *>(new AstQsString(
            ctx->children[0]->accept(this)
    ));
  }

  virtual antlrcpp::Any visitQs_get_item_freetext(qsqlParser::Qs_get_item_freetextContext *ctx) override {
    return dynamic_cast<AstNode *>(new AstQsGetItemFreetext(
            ctx //->children[0]->accept(this)
    ));
  }

  virtual antlrcpp::Any visitQs_get_tag_value(qsqlParser::Qs_get_tag_valueContext *ctx) override {
    return dynamic_cast<AstNode *>(new AstQsGetTagValue(
            ctx->children[1]->accept(this)
    ) );
  }

  virtual antlrcpp::Any visitQs_identifier(qsqlParser::Qs_identifierContext *ctx) override {
    return dynamic_cast<AstNode *>(new AstQsIdentifier(
            ctx //->children[0]->accept(this)
    ));
  }


};

