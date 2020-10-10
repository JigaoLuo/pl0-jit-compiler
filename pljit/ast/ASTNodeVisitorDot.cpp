#include "ast/ASTNodeVisitorDot.hpp"
#include <cassert>
#include <iostream>
//---------------------------------------------------------------------------
namespace pljit {
//---------------------------------------------------------------------------
void ASTNodeVisitorDot::Visit(IdentifierPrimaryExpressionAST& node) { std::cout << node_id << " [label=\" Identifier:" << node.GetName() << " \"];" << std::endl; }
//---------------------------------------------------------------------------
void ASTNodeVisitorDot::Visit(LiteralPrimaryExpressionAST& node) { std::cout << node_id << " [label=\" Literal:" << node.GetValue() << " \"];" << std::endl; }
//---------------------------------------------------------------------------
void ASTNodeVisitorDot::Visit(UnaryExpressionAST& node) {
    const size_t id = node_id;
    switch (node.GetUnaryOperatorType()) {
        case UnaryExpressionAST::UnaryOperator::POSTIVE:
            std::cout << id << " [label=\" Unary Operator: +\"];" << std::endl;
            break;
        case UnaryExpressionAST::UnaryOperator::NEGATIVE:
            std::cout << id << " [label=\" Unary Operator: -\"];" << std::endl;
            break;
    }
    std::cout << id << " -> " << ++node_id << std::endl;
    node.GetChild()->Accept(*this);
}
//---------------------------------------------------------------------------
void ASTNodeVisitorDot::Visit(BinaryExpressionAST& node) {
    const size_t id = node_id;
    switch (node.GetBinaryOperatorType()) {
        case BinaryExpressionAST::BinaryOperator::PLUS:
            std::cout << id << " [label=\" Binary Operator: +\"];" << std::endl;
            break;
        case BinaryExpressionAST::BinaryOperator::MINUS:
            std::cout << id << " [label=\" Binary Operator: -\"];" << std::endl;
            break;
        case BinaryExpressionAST::BinaryOperator::MUL:
            std::cout << id << " [label=\" Binary Operator: *\"];" << std::endl;
            break;
        case BinaryExpressionAST::BinaryOperator::DIV:
            std::cout << id << " [label=\" Binary Operator: /\"];" << std::endl;
            break;
    }
    std::cout << id << " -> " << ++node_id << std::endl;
    node.GetLeftChild()->Accept(*this);
    std::cout << id << " -> " << ++node_id << std::endl;
    node.GetRightChild()->Accept(*this);
}
//---------------------------------------------------------------------------
void ASTNodeVisitorDot::Visit(AssignmentStatementAST& node) {
    const size_t id = node_id;
    std::cout << id << " [label=\"Assignment \"];" << std::endl;
    std::cout << id << " -> " << ++node_id << std::endl;
    node.GetIdentifier()->Accept(*this);
    std::cout << id << " -> " << ++node_id << std::endl;
    node.GetExpression()->Accept(*this);
}
//---------------------------------------------------------------------------
void ASTNodeVisitorDot::Visit(ReturnStatementAST& node) {
    const size_t id = node_id++;
    std::cout << id << " [label=\"Return Statement\"];" << std::endl;
    std::cout << id << " -> " << node_id << std::endl;
    node.GetExpression()->Accept(*this);
}
//---------------------------------------------------------------------------
void ASTNodeVisitorDot::Visit(FunctionAST& node) {
    assert(node_id == 0);
    std::cout << "digraph {" << std::endl;
    const size_t id = node_id;
    std::cout << id << " [label=\"Function\"];" << std::endl;
    for (auto& child: node.GetChildren()) {
        std::cout << id << " -> " << ++node_id << std::endl;
        child->Accept(*this);
    }
    std::cout << "}" << std::endl;
}
//---------------------------------------------------------------------------
} // namespace pljit
//---------------------------------------------------------------------------
