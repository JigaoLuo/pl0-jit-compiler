#pragma once
//---------------------------------------------------------------------------
#include "ast/SymbolTable.hpp"
#include <optional>
//---------------------------------------------------------------------------
namespace pljit {
//---------------------------------------------------------------------------
/// A evaluation context stores all identifiers' values.
class EvaluationContext {
    public:
    /// Delete empty constructor.
    EvaluationContext() = delete;
    /// Constructor: initialization with symbol table.
    explicit EvaluationContext(const SymbolTable& symbol_table);
    /// If we have division by zero error.
    bool GetDivisionByZero() const;
    /// Set that we have division by zero error.
    void SetDivisionByZero();
    /// If we have the return value.
    bool IfReturnValue() const;
    /// Get the return value.
    int64_t GetReturnValue() const;
    /// Set the return value.
    void SetReturnValue(int64_t value);
    /// Get the value table.
    using ValueTable = SymbolTable;
    ValueTable& GetValueTable();

    private:
    /// If we have division by zero error.
    bool division_by_zero = false;
    /// A optional for return value.
    std::optional<int64_t> return_value = std::nullopt;
    /// The values of identifier. Init by the symbol table.
    ValueTable value_table;
};
//---------------------------------------------------------------------------
} // namespace pljit
//---------------------------------------------------------------------------