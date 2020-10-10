//---------------------------------------------------------------------------
#include "parser/ParseTreeNodeVisitorDot.hpp"
#include <cassert>
#include <iostream>
//---------------------------------------------------------------------------
namespace pljit {
//---------------------------------------------------------------------------
void ParseTreeNodeVisitorDot::Visit(IdentifierParseTreeNode& node) { std::cout << node_id++ << " [label=\"Identifier: " << node.GetName() << "\"];" << std::endl; }
//---------------------------------------------------------------------------
void ParseTreeNodeVisitorDot::Visit(LiteralParseTreeNode& node) { std::cout << node_id++ << " [label=\"Literal: " << node.GetValue() << "\"];" << std::endl; }
//---------------------------------------------------------------------------
void ParseTreeNodeVisitorDot::Visit(GenericTokenParseTreeNode& node) { std::cout << node_id++ << " [label=\"" << node.GetSourceCodeReference().GetSourceCode() << "\"];" << std::endl; }
//---------------------------------------------------------------------------
void ParseTreeNodeVisitorDot::Visit(NonTerminalParseTreeNode& node) {
    if (node.GetType() == ParseTreeNode::Type::FunctionDefinition) {
        assert(node_id == 0);
        std::cout << "digraph {" << std::endl;
    }

    const size_t id = node_id;
    std::cout << id << " [label=\"" << node.GetTypeAsString() << "\"];" << std::endl;
    for (auto&& child: node.GetChildren()) {
        std::cout << id << " -> "<< ++node_id << std::endl;
        child->Accept(*this);
    }

    if (node.GetType() == ParseTreeNode::Type::FunctionDefinition) {
        assert(node_id > 0);
        std::cout << "}" << std::endl;
    }
}
//---------------------------------------------------------------------------
} // namespace pljit
//---------------------------------------------------------------------------
