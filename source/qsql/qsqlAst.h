// This file is a part of quicksave project.
// Copyright (c) 2017 Aleksander Gajewski <adiog@quicksave.io>.


//class QsqlDoNotDisplayException(Exception):
#include <antlr4-runtime.h>
#include <qsql/qsqlVisitor.h>
#include <util/format.h>
using SQL = std::pair<std::string, std::unordered_map<std::string, std::string>>;
using CTX = antlr4::ParserRuleContext;

struct AstNode
{
    virtual SQL buildQuery() = 0;
};

using AstNodePtr = antlrcpp::Any; //std::unique_ptr<AstNode>;

//class QsqlDoNotDisplayException(Exception):
//pass
#define GETSQL(node) static_cast<AstNode*>(node)->buildQuery()
#define FORMAT(...) Format::format(__VA_ARGS__)
std::string get_hash(std::string str)
{
    return str;
}

//        def get_sha(content):
//return sha1(content.encode('utf-8')).hexdigest()



struct NullaryAstNode : public AstNode
{
    NullaryAstNode(CTX * ctx) : ctx(*ctx)
    {
    }

    CTX ctx;
};


struct UnaryAstNode : public AstNode
{
    UnaryAstNode(AstNodePtr astNode) : astNode(std::move(astNode)) {}

    AstNodePtr astNode;
};


struct BinaryAstNode : public AstNode
{
    BinaryAstNode(AstNodePtr lhsAstNode, AstNodePtr rhsAstNode)
            : lhsAstNode(std::move(lhsAstNode))
            , rhsAstNode(std::move(rhsAstNode))
    {}

    AstNodePtr lhsAstNode;
    AstNodePtr rhsAstNode;
};

struct AstStart : public UnaryAstNode
{
    using UnaryAstNode::UnaryAstNode;

    SQL buildQuery()
    {
        SQL sql = GETSQL(astNode);
        return {
                FORMAT("SELECT * FROM item WHERE %s;", sql.first),
                sql.second
        };
    }
};


struct AstQsqlQuery : public UnaryAstNode
{
    using UnaryAstNode::UnaryAstNode;

    SQL buildQuery()
    {
        return GETSQL(astNode);
    }
};


struct AstPredicate : public UnaryAstNode
{
    using UnaryAstNode::UnaryAstNode;

    SQL buildQuery()
    {
        return GETSQL(astNode);
    }
};


struct AstOrPredicate : public BinaryAstNode
{
    using BinaryAstNode::BinaryAstNode;

    SQL buildQuery()
    {
        SQL lhsSql = GETSQL(lhsAstNode);
        SQL rhsSql = GETSQL(rhsAstNode);
        auto sqlParams = lhsSql.second;
        sqlParams.insert(rhsSql.second.begin(), rhsSql.second.end());
        auto sqlQuery = FORMAT("(%s OR %s)", lhsSql.first, rhsSql.first);
        return {sqlQuery, sqlParams};
    }
};

struct AstAndPredicate : public BinaryAstNode
{
    using BinaryAstNode::BinaryAstNode;

    SQL buildQuery()
    {
        SQL lhsSql = GETSQL(lhsAstNode);
        SQL rhsSql = GETSQL(rhsAstNode);
        auto sqlParams = lhsSql.second;
        sqlParams.insert(rhsSql.second.begin(), rhsSql.second.end());
        auto sqlQuery = FORMAT("(%s AND %s)", lhsSql.first, rhsSql.first);
        return {sqlQuery, sqlParams};
    }

};


struct AstNotPredicate : public UnaryAstNode
{
    using UnaryAstNode::UnaryAstNode;

    SQL buildQuery()
    {
        SQL sql = GETSQL(astNode);
        return {
                FORMAT("(NOT %s)", sql.first),
                sql.second
        };
    }
};


struct AstParPredicate : public UnaryAstNode
{
    using UnaryAstNode::UnaryAstNode;

    SQL buildQuery()
    {
        return GETSQL(astNode);
    }
};


struct AstQsPredicate : public UnaryAstNode
{
    using UnaryAstNode::UnaryAstNode;

    SQL buildQuery()
    {
        return GETSQL(astNode);
    }
};

struct AstQsHasPredicate : public UnaryAstNode
{
    using UnaryAstNode::UnaryAstNode;

    SQL buildQuery()
    {
        SQL sql = GETSQL(astNode);
        return {
                FORMAT("EXISTS (SELECT * FROM tag WHERE tag.item_id = item.item_id AND tag.name = '%s')",
                       sql.first),
                sql.second
        };
    }
};


struct AstQsStringPredicate : public UnaryAstNode
{
    using UnaryAstNode::UnaryAstNode;

    SQL buildQuery()
    {
        return GETSQL(astNode);
    }
};


struct AstQsStringEqualPredicate : public BinaryAstNode
{
    using BinaryAstNode::BinaryAstNode;

    SQL buildQuery()
    {
        SQL lhsSql = GETSQL(lhsAstNode);
        SQL rhsSql = GETSQL(rhsAstNode);
        auto sqlParams = lhsSql.second;
        sqlParams.insert(rhsSql.second.begin(), rhsSql.second.end());
        auto sqlQuery = FORMAT("(%s = %s)", lhsSql.first, rhsSql.first);
        return {sqlQuery, sqlParams};
    }
};


struct AstQsStringInPredicate : public BinaryAstNode
{
    using BinaryAstNode::BinaryAstNode;

    SQL buildQuery()
    {
        SQL lhsSql = GETSQL(lhsAstNode);
        SQL rhsSql = GETSQL(rhsAstNode);
        auto sqlParams = lhsSql.second;
        sqlParams.insert(rhsSql.second.begin(), rhsSql.second.end());
        auto sqlQuery = FORMAT("(%s LIKE %s)", lhsSql.first, rhsSql.first);
        return {sqlQuery, sqlParams};
    }
};


class AstQsStringMatchPredicate : public BinaryAstNode
{
    using BinaryAstNode::BinaryAstNode;

    SQL buildQuery()
    {
        SQL lhsSql = GETSQL(lhsAstNode);
        SQL rhsSql = GETSQL(rhsAstNode);
        auto sqlParams = lhsSql.second;
        sqlParams.insert(rhsSql.second.begin(), rhsSql.second.end());
        auto sqlQuery = FORMAT("(%s LIKE ('%%%s%%'))", lhsSql.first, rhsSql.first);
        return {sqlQuery, sqlParams};
    }
};


struct AstQsStringWithParenthesis : public UnaryAstNode
{
    using UnaryAstNode::UnaryAstNode;

    SQL buildQuery()
    {
        return GETSQL(astNode);
    }
};

struct AstQsString : public UnaryAstNode
{
    using UnaryAstNode::UnaryAstNode;

    SQL buildQuery()
    {
        return GETSQL(astNode);
    }
};

struct AstQsGetItemFreetext : public NullaryAstNode
{
    using NullaryAstNode::NullaryAstNode;

    SQL buildQuery()
    {
        return {"item.freetext", {}};
    }
};

struct AstQsGetTagValue : public UnaryAstNode
{
    using UnaryAstNode::UnaryAstNode;

    SQL buildQuery()
    {
        SQL sql = GETSQL(astNode);
        return {FORMAT("(SELECT tag.value FROM tag WHERE tag.item_id = item.item_id AND tag.name = '%s')", sql.first), sql.second};
    }
};


struct AstQsIdentifier : public NullaryAstNode
{
    using NullaryAstNode::NullaryAstNode;

    std::string get_identifier()
    {
        std::string s = ctx.getText();
        return {&s[1], s.size()-2};
    }

    SQL buildQuery()
    {
        std::string id = get_identifier();
        std::string hash = get_hash(id);
        std::unordered_map<std::string, std::string> m;
        m.insert({hash, id});
        return {FORMAT("%s", hash), m};
    }
};
