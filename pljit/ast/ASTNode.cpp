//---------------------------------------------------------------------------
#include "ast/ASTNode.hpp"
#include "ast/ASTNodeVisitor.hpp"
#include <cassert>
//---------------------------------------------------------------------------
namespace pljit {
//---------------------------------------------------------------------------
ASTNode::ASTNode(Type type) : type(type) {}
//---------------------------------------------------------------------------
ASTNode::Type ASTNode::GetType() const { return type;}
//---------------------------------------------------------------------------
ExpressionAST::ExpressionAST(Type type) : ASTNode(type) {}
//---------------------------------------------------------------------------
StatementAST::StatementAST(Type type) : ASTNode(type) {}
//---------------------------------------------------------------------------
FunctionAST::FunctionAST(std::vector<std::unique_ptr<StatementAST>> children) : ASTNode(ASTNode::Type::Function), children(std::move(children)) {}
//---------------------------------------------------------------------------
int64_t FunctionAST::Evaluate(EvaluationContext& ec) const {
    for (auto& child: children) {
        child->Evaluate(ec);

        // Return until "RETURN" is evaluated.
        if (ec.IfReturnValue()) {
            return ec.GetReturnValue();
        }
    }
    __builtin_unreachable();  // Must have "RETURN".
}
//---------------------------------------------------------------------------
const std::vector<std::unique_ptr<StatementAST>>& FunctionAST::GetChildren() const { return children; }
//---------------------------------------------------------------------------
void FunctionAST::EliminateLastChild() {
    assert(!children.empty());
    assert(children.back()->GetType() != ASTNode::Type::ReturnStatement);
    children.pop_back();
}
//---------------------------------------------------------------------------
void FunctionAST::Accept(ASTNodeVisitor& v) { v.Visit(*this); }
//---------------------------------------------------------------------------
LiteralPrimaryExpressionAST::LiteralPrimaryExpressionAST(int64_t value) : ExpressionAST(ASTNode::Type::LiteralPrimaryExpression), value(value) {}
//---------------------------------------------------------------------------
int64_t LiteralPrimaryExpressionAST::GetValue() const { return value; }
//---------------------------------------------------------------------------
void LiteralPrimaryExpressionAST::Accept(ASTNodeVisitor& v) { v.Visit(*this); }
//---------------------------------------------------------------------------
int64_t LiteralPrimaryExpressionAST::Evaluate(EvaluationContext&) const { return value; }
//---------------------------------------------------------------------------
IdentifierPrimaryExpressionAST::IdentifierPrimaryExpressionAST(std::string_view name) : ExpressionAST(Type::IdentifierPrimaryExpression), name(name) {}
//---------------------------------------------------------------------------
std::string_view IdentifierPrimaryExpressionAST::GetName() const { return name;}
//---------------------------------------------------------------------------
void IdentifierPrimaryExpressionAST::Accept(ASTNodeVisitor& v) { v.Visit(*this); }
//---------------------------------------------------------------------------
int64_t IdentifierPrimaryExpressionAST::Evaluate(EvaluationContext& ec) const {
    auto it = ec.GetValueTable().find(name);
    assert(it != ec.GetValueTable().end());
    return it->second.GetValue();
}
//---------------------------------------------------------------------------
UnaryExpressionAST::UnaryExpressionAST(UnaryOperator op, std::unique_ptr<ASTNode> child) : ExpressionAST(Type::UnaryExpression), op(op), child(std::move(child)){}
//---------------------------------------------------------------------------
UnaryExpressionAST::UnaryOperator UnaryExpressionAST::GetUnaryOperatorType() const { return op; }
//---------------------------------------------------------------------------
const std::unique_ptr<ASTNode>& UnaryExpressionAST::GetChild() const { return child; }
//---------------------------------------------------------------------------
void UnaryExpressionAST::Accept(ASTNodeVisitor& v) { v.Visit(*this); }
//---------------------------------------------------------------------------
int64_t UnaryExpressionAST::Evaluate(EvaluationContext& ec) const {
    switch (op) {
        case UnaryExpressionAST::UnaryOperator::POSTIVE:
            return child->Evaluate(ec);
        case UnaryExpressionAST::UnaryOperator::NEGATIVE:
            return -1 * child->Evaluate(ec);
    }
    __builtin_unreachable();
}
//---------------------------------------------------------------------------
BinaryExpressionAST::BinaryExpressionAST(BinaryOperator op, std::unique_ptr<ASTNode> left, std::unique_ptr<ASTNode> right) :  ExpressionAST(Type::BinaryExpression), op(op), left(std::move(left)), right(std::move(right)) {}
//---------------------------------------------------------------------------
BinaryExpressionAST::BinaryOperator BinaryExpressionAST::GetBinaryOperatorType() const { return op; }
//---------------------------------------------------------------------------
const std::unique_ptr<ASTNode>& BinaryExpressionAST::GetLeftChild() const { return left; }
//---------------------------------------------------------------------------
const std::unique_ptr<ASTNode>& BinaryExpressionAST::GetRightChild() const { return right; }
//---------------------------------------------------------------------------
void BinaryExpressionAST::Accept(ASTNodeVisitor& v) { v.Visit(*this); }
//---------------------------------------------------------------------------
int64_t BinaryExpressionAST::Evaluate(EvaluationContext& ec) const {
    switch (op) {
        case BinaryExpressionAST::BinaryOperator::PLUS:
            return left->Evaluate(ec) + right->Evaluate(ec);
        case BinaryExpressionAST::BinaryOperator::MINUS:
            return left->Evaluate(ec) - right->Evaluate(ec);
        case BinaryExpressionAST::BinaryOperator::MUL:
            return left->Evaluate(ec) * right->Evaluate(ec);
        case BinaryExpressionAST::BinaryOperator::DIV:
            const int64_t right_value = right->Evaluate(ec);
            if (right_value == 0) {
                assert(!ec.GetDivisionByZero());
                ec.SetDivisionByZero();
                return 0;
            }
            return left->Evaluate(ec) / right_value;
    }
    __builtin_unreachable();
}
//---------------------------------------------------------------------------
AssignmentStatementAST::AssignmentStatementAST(std::unique_ptr<IdentifierPrimaryExpressionAST> identifier, std::unique_ptr<ASTNode> expression) : StatementAST(Type::AssignmentStatement), identifier(std::move(identifier)), expression(std::move(expression)) {}
//---------------------------------------------------------------------------
const std::unique_ptr<IdentifierPrimaryExpressionAST>& AssignmentStatementAST::GetIdentifier() const { return identifier; }
//---------------------------------------------------------------------------
const std::unique_ptr<ASTNode>& AssignmentStatementAST::GetExpression() const { return expression; }
//---------------------------------------------------------------------------
void AssignmentStatementAST::Accept(ASTNodeVisitor& v) { v.Visit(*this); }
//---------------------------------------------------------------------------
int64_t AssignmentStatementAST::Evaluate(EvaluationContext& ec) const {
    auto it = ec.GetValueTable().find(identifier->GetName());
    assert(it != ec.GetValueTable().end());
    it->second.SetValue(expression->Evaluate(ec));
    return it->second.GetValue();
}
//---------------------------------------------------------------------------
void AssignmentStatementAST::SetToConstantLiteral(int64_t value) {
    assert(expression->GetType() != ASTNode::Type::LiteralPrimaryExpression);
    expression = std::make_unique<LiteralPrimaryExpressionAST>(value);
}
//---------------------------------------------------------------------------
ReturnStatementAST::ReturnStatementAST(std::unique_ptr<ASTNode> expression) : StatementAST(Type::ReturnStatement), expression(std::move(expression)) {}
//---------------------------------------------------------------------------
const std::unique_ptr<ASTNode>& ReturnStatementAST::GetExpression() const { return expression; }
//---------------------------------------------------------------------------
void ReturnStatementAST::Accept(ASTNodeVisitor& v) { v.Visit(*this); }
//---------------------------------------------------------------------------
int64_t ReturnStatementAST::Evaluate(EvaluationContext& ec) const {
    assert(!ec.IfReturnValue());
    const int64_t return_value = expression->Evaluate(ec);
    ec.SetReturnValue(return_value);
    assert(ec.IfReturnValue());
    return return_value;
}
//---------------------------------------------------------------------------
void ReturnStatementAST::SetToConstantLiteral(int64_t value) {
    assert(expression->GetType() != ASTNode::Type::LiteralPrimaryExpression);
    expression = std::make_unique<LiteralPrimaryExpressionAST>(value);
}
//---------------------------------------------------------------------------
} // namespace pljit
//---------------------------------------------------------------------------
