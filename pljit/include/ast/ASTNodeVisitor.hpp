#pragma once
//---------------------------------------------------------------------------
#include "ast/ASTNode.hpp"
//---------------------------------------------------------------------------
namespace pljit {
//---------------------------------------------------------------------------
/// A visitor for the AST.
class ASTNodeVisitor {
    public:
    /// Visit methods for the IdentifierPrimaryExpressionAST.
    virtual void Visit(IdentifierPrimaryExpressionAST&) = 0;
    /// Visit methods for the LiteralPrimaryExpressionAST.
    virtual void Visit(LiteralPrimaryExpressionAST&) = 0;
    /// Visit methods for the UnaryExpressionAST.
    virtual void Visit(UnaryExpressionAST&) = 0;
    /// Visit methods for the BinaryExpressionAST.
    virtual void Visit(BinaryExpressionAST&) = 0;
    /// Visit methods for the AssignmentStatementAST.
    virtual void Visit(AssignmentStatementAST&) = 0;
    /// Visit methods for the ReturnStatementAST.
    virtual void Visit(ReturnStatementAST&) = 0;
    /// Visit methods for the FunctionAST.
    virtual void Visit(FunctionAST&) = 0;
};
//---------------------------------------------------------------------------
}  // namespace pljit
//---------------------------------------------------------------------------
