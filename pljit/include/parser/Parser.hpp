#pragma once
//---------------------------------------------------------------------------
#include "lexer/Lexer.hpp"
#include "parser/ParseTreeNode.hpp"
#include <vector>
#include <memory>
//---------------------------------------------------------------------------
namespace pljit {
//---------------------------------------------------------------------------
/// Parser transforms the token stream produced by the lexical analysis stage (provided by the lexer) into a parse tree, which constitutes the syntax analysis stage of the compilation process.
class Parser {
    public:
    /// Constructor.
    explicit Parser(const SourceCodeManagement& source_code_management);

    /// The publicly exposed function to parse the *function-definition* non-terminal which is top-level symbol in grammar.
    ///
    /// Grammar:
    /// function-definition = [ parameter-declarations ]
    ///                       [ variable-declarations ]
    ///                       [ constant-declarations ]
    ///                       compound-statement
    ///                       ".".
    std::unique_ptr<NonTerminalParseTreeNode> ParseFunctionDefinition();

    private:
    /// The current to-be-processed token produced by the lexer's next() function.
    /// The reason for type as pointer: The Token Class has all const, thus can't be assigned.
    /// Hint: Only the init value is nullptr. My lexer does not produce any null value.
    std::unique_ptr<Token> current_token_ptr = nullptr;
    /// Error during the parsing of the optional declarations.
    bool declaration_error = false;
    /// The Lexer producing Tokens.
    Lexer lexer;

    /// Build a new Source Code Reference for the children.
    SourceCodeReference BuildChildrenSourceCodeReference(const std::vector<std::unique_ptr<ParseTreeNode>>& children);

    /// A helper function for error handling.
    std::unique_ptr<NonTerminalParseTreeNode> ErrorHandling(std::string_view str, bool in_declaration = false);

    /// Private function to parse the *parameter-declarations*.
    ///
    /// Grammar:
    /// parameter-declarations = "PARAM" declarator-list ";".
    std::unique_ptr<ParseTreeNode> ParseParameterDeclarations();

    /// Private function to parse the *variable-declarations*.
    ///
    /// Grammar:
    /// variable-declarations = "VAR" declarator-list ";".
    std::unique_ptr<ParseTreeNode> ParseVariableDeclarations();

    /// Private function to parse the *constant-declarations*.
    ///
    /// Grammar:
    /// constant-declarations = "CONST" init-declarator-list ";".
    std::unique_ptr<ParseTreeNode> ParseConstantDeclarations();

    /// Private function to parse the *declarator-list*.
    ///
    /// Grammar:
    /// declarator-list = identifier { "," identifier }.
    std::unique_ptr<ParseTreeNode> ParseDeclaratorList();

    /// Private function to parse the *init-declarator-list*.
    ///
    /// Grammar:
    /// init-declarator-list = init-declarator { "," init-declarator }.
    std::unique_ptr<ParseTreeNode> ParseInitDeclaratorList();

    /// Private function to parse the *init-declarator*.
    ///
    /// Grammar:
    /// init-declarator = identifier "=" literal.
    std::unique_ptr<ParseTreeNode> ParseInitDeclarator();

    /// Private function to parse the *compound-statement*.
    ///
    /// Grammar:
    /// compound-statement = "BEGIN" statement-list "END".
    std::unique_ptr<ParseTreeNode> ParseCompoundStatement();

    /// Private function to parse the *statement-list*.
    ///
    /// Grammar:
    /// statement-list = statement { ";" statement }.
    std::unique_ptr<ParseTreeNode> ParseStatementList();

    /// Private function to parse the *statement*.
    ///
    /// Grammar:
    /// statement = assignment-expression
    ///           | "RETURN" additive-expression.
    std::unique_ptr<ParseTreeNode> ParseStatement();

    /// Private function to parse the *assignment-expression*.
    ///
    /// Grammar:
    /// assignment-expression = identifier ":=" additive-expression.
    std::unique_ptr<ParseTreeNode> ParseAssignmentExpresion();

    /// Private function to parse the *additive-expression*.
    ///
    /// Grammar:
    /// additive-expression = multiplicative-expression [ ( "+" | "-" ) additive-expression ].
    std::unique_ptr<ParseTreeNode> ParseAdditiveExpression();

    /// Private function to parse the *multiplicative-expression*.
    ///
    /// Grammar:
    /// multiplicative-expression = unary-expression [ ( "*" | "/" ) multiplicative-expression ].
    std::unique_ptr<ParseTreeNode> ParseMultiplicativeExpression();

    /// Private function to parse the *unary-expression*.
    ///
    /// Grammar:
    /// unary-expression = [ "+" | "-" ] primary-expression.
    std::unique_ptr<ParseTreeNode> ParseUnaryExpression();

    /// Private function to parse the *primary-expression*.
    ///
    /// Grammar:
    /// primary-expression = identifier
    ///                    | literal
    ///                    | "(" additive-expression ")".
    std::unique_ptr<ParseTreeNode> ParsePrimaryExpression();
};
//---------------------------------------------------------------------------
} // namespace pljit
//---------------------------------------------------------------------------
