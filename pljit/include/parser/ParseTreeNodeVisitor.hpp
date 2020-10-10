#pragma once
//---------------------------------------------------------------------------
#include "parser/ParseTreeNode.hpp"
//---------------------------------------------------------------------------
namespace pljit {
//---------------------------------------------------------------------------
/// A visitor for the Parse Tree.
class ParseTreeNodeVisitor {
    public:
    /// Visit methods for the IdentifierNode.
    virtual void Visit(IdentifierParseTreeNode&) = 0;
    /// Visit methods for the LiteralNode.
    virtual void Visit(LiteralParseTreeNode&) = 0;
    /// Visit methods for the GenericTokenNode.
    virtual void Visit(GenericTokenParseTreeNode&) = 0;
    /// Visit methods for the NonTerminalNode.
    virtual void Visit(NonTerminalParseTreeNode&) = 0;
};
//---------------------------------------------------------------------------
} // namespace pljit
//---------------------------------------------------------------------------