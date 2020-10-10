//---------------------------------------------------------------------------
#include "optimization/EvaluationContext.hpp"
//---------------------------------------------------------------------------
namespace pljit {
//---------------------------------------------------------------------------
EvaluationContext::EvaluationContext(const SymbolTable& symbol_table) { value_table = symbol_table; }
//---------------------------------------------------------------------------
bool EvaluationContext::GetDivisionByZero() const { return division_by_zero; }
//---------------------------------------------------------------------------
void EvaluationContext::SetDivisionByZero() { division_by_zero = true; }
//---------------------------------------------------------------------------
bool EvaluationContext::IfReturnValue() const { return return_value.has_value(); }
//---------------------------------------------------------------------------
int64_t EvaluationContext::GetReturnValue() const { return *return_value; }
//---------------------------------------------------------------------------
void EvaluationContext::SetReturnValue(int64_t value) { return_value = value; }
//---------------------------------------------------------------------------
EvaluationContext::ValueTable& EvaluationContext::GetValueTable() { return value_table; }
//---------------------------------------------------------------------------
} // namespace pljit
//---------------------------------------------------------------------------
