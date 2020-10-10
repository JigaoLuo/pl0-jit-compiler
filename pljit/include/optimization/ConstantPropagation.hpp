#pragma once
//---------------------------------------------------------------------------
#include "ast/SymbolTable.hpp"
#include "optimization/OptimizationPass.hpp"
#include <unordered_map>
//---------------------------------------------------------------------------
namespace pljit {
//---------------------------------------------------------------------------
/// A visitor does optimization: constant propagation.
class ConstantPropagation : public OptimizationPass {
    public:
    /// Constructor.
    explicit ConstantPropagation(const SymbolTable& symbol_table);
    /// The public interface of the Optimization Pass: constant propagation.
    void Optimize(FunctionAST& node) override;

    private:
    /// Init symbol table from semantic analyzer.
    SymbolTable symbol_table;
    /// A mapping: expression id -> potentially constant value.
    std::unordered_map<ASTNode*, int64_t> expressions;
    /// A optional for return value.
    std::optional<int64_t> return_value = std::nullopt;

    /// Optimization Pass (constant propagation) Visit methods for the IdentifierPrimaryExpressionAST.
    void Visit(IdentifierPrimaryExpressionAST& node) override;
    /// Optimization Pass (constant propagation) Visit methods for the LiteralPrimaryExpressionAST.
    void Visit(LiteralPrimaryExpressionAST& node) override;
    /// Optimization Pass (constant propagation) Visit methods for the UnaryExpressionAST.
    void Visit(UnaryExpressionAST& node) override;
    /// Optimization Pass (constant propagation) Visit methods for the BinaryExpressionAST.
    void Visit(BinaryExpressionAST& node) override;
    /// Optimization Pass (constant propagation) Visit methods for the AssignmentStatementAST.
    void Visit(AssignmentStatementAST& node) override;
    /// Optimization Pass (constant propagation) Visit methods for the ReturnStatementAST.
    void Visit(ReturnStatementAST& node) override;
    /// Optimization Pass (constant propagation) Visit methods for the FunctionAST.
    void Visit(FunctionAST& node) override;
};
//---------------------------------------------------------------------------
} // namespace pljit
//---------------------------------------------------------------------------
