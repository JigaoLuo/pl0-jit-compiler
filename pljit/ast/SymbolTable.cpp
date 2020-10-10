//---------------------------------------------------------------------------
#include "ast/SymbolTable.hpp"
#include <cassert>
//---------------------------------------------------------------------------
namespace pljit {
//---------------------------------------------------------------------------
Symbol::Symbol(Type type, SourceCodeReference source_code_reference) : type(type), initialized(false), value(0), source_code_reference(source_code_reference) {}
//---------------------------------------------------------------------------
Symbol::Symbol(Type type, bool initialized, SourceCodeReference source_code_reference) : type(type), initialized(initialized), value(0), source_code_reference(source_code_reference) {
    assert(type == Symbol::Type::VARIABLE || initialized);  // If not variable => must be initialized.
}
//---------------------------------------------------------------------------
Symbol::Type Symbol::GetType() const { return type; }
//---------------------------------------------------------------------------
bool Symbol::IfInitialized() const { return initialized; }
//---------------------------------------------------------------------------
void Symbol::SetInitialized() {
    assert(type == Symbol::Type::VARIABLE);
    initialized = true;
}
//---------------------------------------------------------------------------
void Symbol::SetUninitialized() {
    assert(type == Symbol::Type::VARIABLE);
    initialized = false;
}
//---------------------------------------------------------------------------
const std::string_view Symbol::GetName() const { return name; }
//---------------------------------------------------------------------------
int64_t Symbol::GetValue() const {
    assert(initialized);
    return value;
}
//---------------------------------------------------------------------------
void Symbol::SetValue(int64_t v) {
    assert(initialized);
    value = v;
}
//---------------------------------------------------------------------------
} // namespace pljit
//---------------------------------------------------------------------------
