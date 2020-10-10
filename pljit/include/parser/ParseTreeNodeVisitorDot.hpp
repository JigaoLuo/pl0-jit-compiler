#pragma once
//---------------------------------------------------------------------------
#include "parser/ParseTreeNodeVisitor.hpp"
//---------------------------------------------------------------------------
namespace pljit {
/// A visitor prints the parse tree in the DOT format.
class ParseTreeNodeVisitorDot : public ParseTreeNodeVisitor {
    public:
    /// Dot Visit methods for the IdentifierNode.
    void Visit(IdentifierParseTreeNode& node) override;
    /// Dot Visit methods for the LiteralNode.
    void Visit(LiteralParseTreeNode& node) override;
    /// Dot Visit methods for the GenericTokenNode.
    void Visit(GenericTokenParseTreeNode& node) override;
    /// Dot Visit methods for the NonTerminalNode.
    void Visit(NonTerminalParseTreeNode& node) override;

    private:
    /// Each parse tree node has an id.
    size_t node_id = 0;
};
//---------------------------------------------------------------------------
} // namespace pljit
//---------------------------------------------------------------------------