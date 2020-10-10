//---------------------------------------------------------------------------
#include "lexer/Token.hpp"
//---------------------------------------------------------------------------
namespace pljit {
//---------------------------------------------------------------------------
Token::Token(Type type, const SourceCodeReference& source_code_reference) : type(type), source_code_reference(source_code_reference) {}
//---------------------------------------------------------------------------
Token::Type Token::GetType() const { return type; }
//---------------------------------------------------------------------------
const SourceCodeReference& Token::GetSourceCodeReference() const { return source_code_reference; }
//---------------------------------------------------------------------------
} // namespace pljit
//---------------------------------------------------------------------------
