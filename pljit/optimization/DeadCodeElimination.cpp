//---------------------------------------------------------------------------
#include "optimization/DeadCodeElimination.hpp"
#include <cassert>
#include <algorithm>
//---------------------------------------------------------------------------
namespace pljit {
//---------------------------------------------------------------------------
void OptimizeDeadCode::Optimize(FunctionAST& node) { Visit(node); }
//---------------------------------------------------------------------------
void OptimizeDeadCode::Visit(IdentifierPrimaryExpressionAST&) {}
//---------------------------------------------------------------------------
void OptimizeDeadCode::Visit(LiteralPrimaryExpressionAST&) {}
//---------------------------------------------------------------------------
void OptimizeDeadCode::Visit(UnaryExpressionAST&) {}
//---------------------------------------------------------------------------
void OptimizeDeadCode::Visit(BinaryExpressionAST&) {}
//---------------------------------------------------------------------------
void OptimizeDeadCode::Visit(AssignmentStatementAST&) {}
//---------------------------------------------------------------------------
void OptimizeDeadCode::Visit(ReturnStatementAST&) {}
//---------------------------------------------------------------------------
void OptimizeDeadCode::Visit(FunctionAST& node) {
    while (!node.GetChildren().empty() && node.GetChildren().back()->GetType() != ASTNode::Type::ReturnStatement) {
        node.EliminateLastChild();
    }
    assert(!node.GetChildren().empty());
    assert(node.GetChildren().back()->GetType() == ASTNode::Type::ReturnStatement); // last statement is "RETRUN".
}
//---------------------------------------------------------------------------
} // namespace pljit
//---------------------------------------------------------------------------
