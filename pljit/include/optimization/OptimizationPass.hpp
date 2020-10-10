#pragma once
//---------------------------------------------------------------------------
#include "ast/ASTNodeVisitor.hpp"
//---------------------------------------------------------------------------
namespace pljit {
//---------------------------------------------------------------------------
/// An abstract class that represents an optimization pass.
/// It has at least one member function that takes an AST and performs an optimization on it.
class OptimizationPass : public ASTNodeVisitor {
    public:
    /// The public interface of the Optimization Pass.
    virtual void Optimize(FunctionAST&) = 0;
};
//---------------------------------------------------------------------------
} // namespace pljit
//---------------------------------------------------------------------------
