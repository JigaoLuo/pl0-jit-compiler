#pragma once
//---------------------------------------------------------------------------
#include "util/SourceCodeReference.hpp"
#include <unordered_map>
//---------------------------------------------------------------------------
namespace pljit {
//---------------------------------------------------------------------------
/// A symbol represents a identifier.
class Symbol {
    public:
    /// The different types of identifier.
    enum Type {
        PARAMETER,
        VARIABLE,
        CONSTANT
    };
    /// Constructor.
    Symbol(Type type, SourceCodeReference source_code_reference);
    /// Constructor.
    Symbol(Type type, bool initialized, SourceCodeReference source_code_reference);
    /// Get identifier (symbol) type.
    Type GetType() const;
    /// Get the status of initialization.
    bool IfInitialized() const;
    /// Set as initialized.
    void SetInitialized();
    /// Set as uninitialized. Only for evaluation and constant propagation.
    void SetUninitialized();
    /// Get identifier (symbol) name.
    const std::string_view GetName() const;
    /// Get value.
    int64_t GetValue() const;
    /// Set value.
    void SetValue(int64_t v);

    private:
    /// The type.
    Type type;
    /// The status of initialization.
    /// *constants* and *parameters* are always considered to be initialized.
    bool initialized;
    /// The name.
    const std::string_view name;
    /// The value.
    int64_t value;
    /// The reference to the original source code instead of directly storing the code as characters.
    const SourceCodeReference source_code_reference;
};
//---------------------------------------------------------------------------
/// A symbol table is exactly a hash table, which is already efficient to find the declaration given the name of an identifier
/// The symbol table keeps track of all identifiers that are declared in the program and keep the source code location of each declaration to enable better error messages.
using SymbolTable = std::unordered_map<std::string_view, Symbol>;
//---------------------------------------------------------------------------
} //namespace pljit
//---------------------------------------------------------------------------
