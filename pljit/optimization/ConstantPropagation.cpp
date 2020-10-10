//---------------------------------------------------------------------------
#include "optimization/ConstantPropagation.hpp"
#include <cassert>
#include <algorithm>
#include <optional>
//---------------------------------------------------------------------------
namespace pljit {
//---------------------------------------------------------------------------
ConstantPropagation::ConstantPropagation(const SymbolTable& symbol_table) {
    this->symbol_table = symbol_table;
    // Re-set the initialized flag for variables.
    for (auto& it : this->symbol_table) {
        if (it.second.GetType()  == Symbol::Type::VARIABLE) {
            it.second.SetUninitialized();
        }
    }
}
//---------------------------------------------------------------------------
void ConstantPropagation::Optimize(FunctionAST& node) {
    // Visit the AST to collect constant in the symbol table and expression mapping.
    Visit(node);
    // Replace all statements, when constants from the symbol table and expression mapping are available.
    for (auto& child: node.GetChildren()) {
        switch (child->GetType()) {
            case ASTNode::Type::AssignmentStatement:
                {
                    auto* const statement = static_cast<AssignmentStatementAST*>(child.get());
                    auto it_e = expressions.find(child.get());
                    if (it_e != expressions.end()) {
                        assert(expressions.find(statement->GetExpression().get()) != expressions.end());  // the child must be constant.
                        assert(symbol_table.find(statement->GetIdentifier()->GetName())->second.IfInitialized());
                        assert(symbol_table.find(statement->GetIdentifier()->GetName())->second.GetValue() == it_e->second);
                        assert(symbol_table.find(statement->GetIdentifier()->GetName())->second.GetType() != Symbol::Type::CONSTANT);
                        statement->SetToConstantLiteral(it_e->second);
                    }
                    break;
                }
            case ASTNode::Type::ReturnStatement: {
                auto* const statement = static_cast<ReturnStatementAST*>(child.get());
                auto it_e = expressions.find(child.get());
                if (it_e != expressions.end()) {
                    assert(expressions.find(statement->GetExpression().get()) != expressions.end());  // the child must be constant.
                    assert(return_value);
                    statement->SetToConstantLiteral(*return_value);
                }
                break;
            }
            default:
                break;
        }
    }
}
//---------------------------------------------------------------------------
void ConstantPropagation::Visit(IdentifierPrimaryExpressionAST& node) {
    auto it = symbol_table.find(node.GetName());
    if (it != symbol_table.end() && (it->second.GetType() == Symbol::Type::CONSTANT || (it->second.GetType() == Symbol::Type::VARIABLE && it->second.IfInitialized()))) {
        // The identifier is directly typed as constant.
        expressions.emplace(&node, it->second.GetValue());
    }
}
//---------------------------------------------------------------------------
void ConstantPropagation::Visit(LiteralPrimaryExpressionAST& node) { expressions.emplace(&node, node.GetValue()); }
//---------------------------------------------------------------------------
void ConstantPropagation::Visit(UnaryExpressionAST& node) {
    // unary-expression = [ "+" | "-" ] primary-expression.
    node.GetChild()->Accept(*this);
    auto it = expressions.find(node.GetChild().get());
    if (it != expressions.end()) {
        // If the child (primary-expression) generates a constant, then this unary expression generates also a constant.
        const auto unary_expression_constant = (node.GetUnaryOperatorType() == UnaryExpressionAST::UnaryOperator::POSTIVE ? 1 : -1) * it->second;
        expressions.emplace(&node, unary_expression_constant);
    }
}
//---------------------------------------------------------------------------
void ConstantPropagation::Visit(BinaryExpressionAST& node) {
    // additive-expression = multiplicative-expression [ ( "+" | "-" ) additive-expression ].
    // multiplicative-expression = unary-expression [ ( "*" | "/" ) multiplicative-expression ].

    // Map the children expression.
    node.GetRightChild()->Accept(*this);
    node.GetLeftChild()->Accept(*this);
    auto it_right = expressions.find(node.GetRightChild().get());
    auto it_left = expressions.find(node.GetLeftChild().get());
    if (it_left != expressions.end() && it_right != expressions.end()) {
        // If the children both generate constants, then this binary expression generates also a constant.
        switch (node.GetBinaryOperatorType()) {
            case BinaryExpressionAST::BinaryOperator::PLUS:
                {
                    expressions.emplace(&node, it_left->second + it_right->second);
                    return;
                }
            case BinaryExpressionAST::BinaryOperator::MINUS:
                {
                    expressions.emplace(&node, it_left->second - it_right->second);
                    return;
                }
            case BinaryExpressionAST::BinaryOperator::MUL:
                {
                    expressions.emplace(&node, it_left->second * it_right->second);
                    return;
                }
            case BinaryExpressionAST::BinaryOperator::DIV:
                {
                    if (it_right->second != 0) {
                        expressions.emplace(&node, it_left->second / it_right->second);
                    }
                    return;
                }
        }
    }
}
//---------------------------------------------------------------------------
void ConstantPropagation::Visit(AssignmentStatementAST& node) {
    // compound-statement = "BEGIN" statement-list "END".
    // statement-list = statement { ";" statement }.
    // statement = assignment-expression
    //           | "RETURN" additive-expression.
    // assignment-expression = identifier ":=" additive-expression.
    node.GetExpression()->Accept(*this);
    auto it = expressions.find(node.GetExpression().get());
    if (it != expressions.end()) {
        // If the child (additive-expression) generates a constant, then this identifier is also a constant.
        expressions.emplace(&node, it->second);
        // Update the identifier in symbol table.
        auto it_st = symbol_table.find(node.GetIdentifier()->GetName());
        it_st->second.SetInitialized();
        assert(it_st->second.GetType() != Symbol::Type::CONSTANT);
        it_st->second.SetValue(it->second);
    }
}
//---------------------------------------------------------------------------
void ConstantPropagation::Visit(ReturnStatementAST& node) {
    // compound-statement = "BEGIN" statement-list "END".
    // statement-list = statement { ";" statement }.
    // statement = assignment-expression
    //           | "RETURN" additive-expression.
    node.GetExpression()->Accept(*this);
    auto it = expressions.find(node.GetExpression().get());
    if (it != expressions.end()) {
        // If the child (additive-expression) generates a constant, then this "RETURN" is also a constant.
        expressions.emplace(&node, it->second);
        assert(!return_value);
        return_value = it->second;
        assert(return_value);
    }
}
//---------------------------------------------------------------------------
void ConstantPropagation::Visit(FunctionAST& node) {
    for (auto& child: node.GetChildren()) {
        child->Accept(*this);
    }
}
//---------------------------------------------------------------------------
} // namespace pljit
//---------------------------------------------------------------------------
