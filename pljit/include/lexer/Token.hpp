#pragma once
//---------------------------------------------------------------------------
#include "util/SourceCodeReference.hpp"
//---------------------------------------------------------------------------
namespace pljit {
//---------------------------------------------------------------------------
/// A token class helps the lexer to transform a stream of valid tokens from the raw source code.
class Token {
    public:
    /// All possible types of PL tokens.
    enum class Type {
        /// Arithmetic Token Types:
        PLUS               /* "+" unary & binary */,
        MINUS              /* "-" unary & binary */,
        MUL                /* "*" */,
        DIV                /* "/" */,
        LEFT_PARENTHESE    /* "(" */,
        RIGHT_PARENTHESE   /* ")" */,
        /// Keywords  (also called as *terminal symbols*):
        PARAM              /* "PARAM" */,
        VAR                /* "VAR" */,
        CONST              /* "CONST" */,
        BEGIN              /* "BEGIN" */,
        END                /* "END" */,
        RETURN             /* "RETURN" */,
        IDENTIFIER         /* any combination of lower- and upper-case letters that is not a keyword*/,
        LITERAL            /* any combination of digits */,
        /// non-terminal symbols in non-terminal productions:
        ASSIGNMENT         /* ":=" */,
        EQUAL              /* "=" */,
        DOT                /* "." */,
        COMMA              /* "," */,
        SEMICOLON          /* ";" */,
        /// Others:
        ERROR              /* A lexer error */,
        EOT                /* An indicator as well as a placeholder at end: End Of Tokens */
    };

    /// Constructor.
    Token(Type type, const SourceCodeReference& source_code_reference);
    /// Get the type of the Token
    [[nodiscard]] Token::Type GetType() const;
    /// Get the source code reference.
    const SourceCodeReference& GetSourceCodeReference() const;

    private:
    /// The type of the token.
    const Type type;
    /// The reference to the original source code instead of directly storing the code as characters.
    const SourceCodeReference source_code_reference;
};
//---------------------------------------------------------------------------
} // namespace pljit
//---------------------------------------------------------------------------
