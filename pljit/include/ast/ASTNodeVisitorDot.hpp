#pragma once
//---------------------------------------------------------------------------
#include "ast/ASTNode.hpp"
#include "ast/ASTNodeVisitor.hpp"
//---------------------------------------------------------------------------
namespace pljit {
//---------------------------------------------------------------------------
/// A visitor prints the AST in the DOT format.
class ASTNodeVisitorDot : public ASTNodeVisitor {
    private:
    /// Each AST node has a id.
    size_t node_id = 0;

    public:
    /// Dot Visit methods for the IdentifierPrimaryExpressionAST.
    void Visit(IdentifierPrimaryExpressionAST& node) override;
    /// Dot Visit methods for the LiteralPrimaryExpressionAST.
    void Visit(LiteralPrimaryExpressionAST& node) override;
    /// Dot Visit methods for the UnaryExpressionAST.
    void Visit(UnaryExpressionAST& node) override;
    /// Dot Visit methods for the BinaryExpressionAST.
    void Visit(BinaryExpressionAST& node) override;
    /// Dot Visit methods for the AssignmentStatementAST.
    void Visit(AssignmentStatementAST& node) override;
    /// Dot Visit methods for the ReturnStatementAST.
    void Visit(ReturnStatementAST& node) override;
    /// Dot Visit methods for the FunctionAST.
    void Visit(FunctionAST& node) override;
};
//---------------------------------------------------------------------------
} // namespace pljit
//---------------------------------------------------------------------------