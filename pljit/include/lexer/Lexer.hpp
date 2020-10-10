#pragma once
//---------------------------------------------------------------------------
#include "lexer/Token.hpp"
//---------------------------------------------------------------------------
namespace pljit {
//---------------------------------------------------------------------------
/// Transforms the source code into a stream of Tokens
class Lexer {
    public:
    /// Constructor.
    explicit Lexer(const SourceCodeManagement& source_code_management);
    /// Produce next possible token of this source code managed by the lexer.
    /// The reason to only get the next token instead of the whole token stream: early detection of grammar error.
    Token ProduceNextToken();
    /// Get the source code management.
    const SourceCodeManagement& GetSourceCodeManagement() const;

    private:
    /// The current processing line in source code.
    size_t current_line_number;
    /// The current processing offset in current processing line in source code.
    size_t current_line_offset;
    /// The source code management.
    const SourceCodeManagement& source_code_management;
    /// Construct an error message: "Unexpected Character".
    void Error(size_t line_number, size_t line_offset) const;
};
//---------------------------------------------------------------------------
} // namespace pljit
//---------------------------------------------------------------------------
