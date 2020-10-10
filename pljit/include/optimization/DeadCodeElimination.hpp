#pragma once
//---------------------------------------------------------------------------
#include "optimization/OptimizationPass.hpp"
//---------------------------------------------------------------------------
namespace pljit {
//---------------------------------------------------------------------------
/// A visitor does optimization: dead code elimination.
class OptimizeDeadCode : public OptimizationPass {
    public:
    /// Constructor.
    OptimizeDeadCode() = default;
    /// The public interface of the Optimization Pass: dead code elimination.
    void Optimize(FunctionAST& node) override;

    private:
    /// Optimization Pass (dead code elimination) Visit methods for the IdentifierPrimaryExpressionAST.
    void Visit(IdentifierPrimaryExpressionAST& node) override;
    /// Optimization Pass (dead code elimination) Visit methods for the LiteralPrimaryExpressionAST.
    void Visit(LiteralPrimaryExpressionAST& node) override;
    /// Optimization Pass (dead code elimination) Visit methods for the UnaryExpressionAST.
    void Visit(UnaryExpressionAST& node) override;
    /// Optimization Pass (dead code elimination) Visit methods for the BinaryExpressionAST.
    void Visit(BinaryExpressionAST& node) override;
    /// Optimization Pass (dead code elimination) Visit methods for the AssignmentStatementAST.
    void Visit(AssignmentStatementAST& node) override;
    /// Optimization Pass (dead code elimination) Visit methods for the ReturnStatementAST.
    void Visit(ReturnStatementAST& node) override;
    /// Optimization Pass (dead code elimination) Visit methods for the FunctionAST.
    void Visit(FunctionAST& node) override;
};
//---------------------------------------------------------------------------
} // namespace pljit
//---------------------------------------------------------------------------
