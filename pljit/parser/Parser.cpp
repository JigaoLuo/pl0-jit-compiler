//---------------------------------------------------------------------------
#include "parser/Parser.hpp"
#include <cassert>
#include <vector>
#include <memory>
//---------------------------------------------------------------------------
namespace pljit {
//---------------------------------------------------------------------------
Parser::Parser(const SourceCodeManagement& source_code_management) : lexer(source_code_management) {}
//---------------------------------------------------------------------------
SourceCodeReference Parser::BuildChildrenSourceCodeReference(const std::vector<std::unique_ptr<ParseTreeNode>>& children) {
    const size_t length = [&children] {
        size_t l = 0;
        for (const std::unique_ptr<ParseTreeNode>& child: children) {
            l += child->GetSourceCodeReference().GetLength();
        }
        return l;
    } ();
    return SourceCodeReference(children[0]->GetSourceCodeReference().GetLineNumber(), children[0]->GetSourceCodeReference().GetCharOffset(), length, lexer.GetSourceCodeManagement());
}
//---------------------------------------------------------------------------
std::unique_ptr<NonTerminalParseTreeNode> Parser::ErrorHandling(std::string_view str, bool in_declaration) {
    assert(!declaration_error);
    if (in_declaration) {
        declaration_error = true;
    }
    current_token_ptr->GetSourceCodeReference().PrintContext(str);
    return nullptr;
}
//---------------------------------------------------------------------------
std::unique_ptr<NonTerminalParseTreeNode> Parser::ParseFunctionDefinition() {
    assert(!current_token_ptr && "The ultimate starting point of building the parse tree.");
    current_token_ptr = std::make_unique<Token>(lexer.ProduceNextToken());
    assert(current_token_ptr->GetType() != Token::Type::ERROR);
    assert(current_token_ptr->GetType() != Token::Type::EOT);

    // A vector storing parse tree nodes.
    std::vector<std::unique_ptr<ParseTreeNode>> parse_tree_nodes;

    if (current_token_ptr->GetType() == Token::Type::PARAM) {
        // Case for *[ parameter-declarations ]* inside of a *function-definition*, which is optional.
        std::unique_ptr<ParseTreeNode> parameter_declarations = ParseParameterDeclarations();
        if (declaration_error) { return nullptr; }
        assert(parameter_declarations && " A valid pointer without any declaration error.");
        parse_tree_nodes.emplace_back(std::move(parameter_declarations));
    }

    if (current_token_ptr->GetType() == Token::Type::VAR) {
        // Case for *[ variable-declarations ]* inside of a *function-definition*, which is optional.
        std::unique_ptr<ParseTreeNode> variable_declarations = ParseVariableDeclarations();
        if (declaration_error) { return nullptr; }
        assert(variable_declarations && " A valid pointer without any declaration error.");
        parse_tree_nodes.emplace_back(std::move(variable_declarations));
    }

    if (current_token_ptr->GetType() == Token::Type::CONST) {
        // Case for *[ constant-declarations ]* inside of a *function-definition*, which is optional.
        std::unique_ptr<ParseTreeNode> constant_declarations = ParseConstantDeclarations();
        if (declaration_error) { return nullptr; }
        assert(constant_declarations && " A valid pointer without any declaration error.");
        parse_tree_nodes.emplace_back(std::move(constant_declarations));
    }

    // Case for *compound-statement* inside of a *function-definition*.
    std::unique_ptr<ParseTreeNode> compound_statement = ParseCompoundStatement();
    if (!compound_statement) { return nullptr; }
    parse_tree_nodes.emplace_back(std::move(compound_statement));

    // Case for *"."* inside of a *function-definition*.
    if (current_token_ptr->GetType() != Token::Type::DOT) { return ErrorHandling("Expected \".\" DOT", true); }
    parse_tree_nodes.emplace_back(std::make_unique<GenericTokenParseTreeNode>(current_token_ptr->GetSourceCodeReference()));  // "."
    return std::make_unique<NonTerminalParseTreeNode>(BuildChildrenSourceCodeReference(parse_tree_nodes), ParseTreeNode::Type::FunctionDefinition, std::move(parse_tree_nodes));
}
//---------------------------------------------------------------------------
std::unique_ptr<ParseTreeNode> Parser::ParseParameterDeclarations() {
    assert(current_token_ptr->GetType() == Token::Type::PARAM && " Pre-condition of calling this function: PARAM.");
    std::vector<std::unique_ptr<ParseTreeNode>> children;
    children.emplace_back(std::make_unique<GenericTokenParseTreeNode>(current_token_ptr->GetSourceCodeReference()));  // "PARAM"
    // Let lexer produce the next token.
    current_token_ptr = std::make_unique<Token>(lexer.ProduceNextToken());

    // Case for *declarator-list* inside of a *parameter-declarations*.
    if (current_token_ptr->GetType() != Token::Type::IDENTIFIER) { return ErrorHandling("Expected the first identifier", true); }
    std::unique_ptr<ParseTreeNode> declarator_list = ParseDeclaratorList();
    if (!declarator_list) { return nullptr; }
    children.emplace_back(std::move(declarator_list));
    assert(children.back()->GetType() == ParseTreeNode::Type::DeclaratorList && !(static_cast<NonTerminalParseTreeNode*>(children.back().get())->GetChildren().empty()) && " At least one IDENTIFIER inside of *declarator-list*. ");

    if (current_token_ptr->GetType() != Token::Type::SEMICOLON) { return ErrorHandling("Expected a \";\" SEMICOLON", true); }
    children.emplace_back(std::make_unique<GenericTokenParseTreeNode>(current_token_ptr->GetSourceCodeReference()));  // SEMICOLON ";"
    // Let lexer produce the next token.
    current_token_ptr = std::make_unique<Token>(lexer.ProduceNextToken());
    return std::make_unique<NonTerminalParseTreeNode>(BuildChildrenSourceCodeReference(children), ParseTreeNode::Type::ParameterDeclarations, std::move(children));
}
//---------------------------------------------------------------------------
std::unique_ptr<ParseTreeNode> Parser::ParseVariableDeclarations() {
    assert(current_token_ptr->GetType() == Token::Type::VAR && " Pre-condition of calling this function: VAR.");
    std::vector<std::unique_ptr<ParseTreeNode>> children;
    children.emplace_back(std::make_unique<GenericTokenParseTreeNode>(current_token_ptr->GetSourceCodeReference()));  // "VAR"
    // Let lexer produce the next token.
    current_token_ptr = std::make_unique<Token>(lexer.ProduceNextToken());

    // Case for *declarator-list* inside of a *variable-declarations*.
    if (current_token_ptr->GetType() != Token::Type::IDENTIFIER) { return ErrorHandling("Expected the first identifier", true); }
    std::unique_ptr<ParseTreeNode> declarator_list = ParseDeclaratorList();
    if (!declarator_list) { return nullptr; }
    children.emplace_back(std::move(declarator_list));
    assert(children.back()->GetType() == ParseTreeNode::Type::DeclaratorList && !(static_cast<NonTerminalParseTreeNode*>(children.back().get())->GetChildren().empty()) && " At least one IDENTIFIER inside of *declarator-list*. ");

    if (current_token_ptr->GetType() != Token::Type::SEMICOLON) { return ErrorHandling("Expected a \";\" SEMICOLON"); }
    children.emplace_back(std::make_unique<GenericTokenParseTreeNode>(current_token_ptr->GetSourceCodeReference()));  // SEMICOLON ";"
    // Let lexer produce the next token.
    current_token_ptr = std::make_unique<Token>(lexer.ProduceNextToken());
    return std::make_unique<NonTerminalParseTreeNode>(BuildChildrenSourceCodeReference(children), ParseTreeNode::Type::VariableDeclarations, std::move(children));
}
//---------------------------------------------------------------------------
std::unique_ptr<ParseTreeNode> Parser::ParseConstantDeclarations() {
    assert(current_token_ptr->GetType() == Token::Type::CONST && " Pre-condition of calling this function: CONST.");
    std::vector<std::unique_ptr<ParseTreeNode>> children;
    children.emplace_back(std::make_unique<GenericTokenParseTreeNode>(current_token_ptr->GetSourceCodeReference()));  // "CONST"
    // Let lexer produce the next token.
    current_token_ptr = std::make_unique<Token>(lexer.ProduceNextToken());

    if (current_token_ptr->GetType() != Token::Type::IDENTIFIER) { return ErrorHandling("Expected the first identifier", true); }
    std::unique_ptr<ParseTreeNode> declarator_list = ParseInitDeclaratorList();
    if (!declarator_list) { return nullptr; }
    children.emplace_back(std::move(declarator_list));
    assert(children.back()->GetType() == ParseTreeNode::Type::InitDeclaratorList && !(static_cast<NonTerminalParseTreeNode*>(children.back().get())->GetChildren().empty()) && " At least one IDENTIFIER inside of *declarator-list*. ");

    if (current_token_ptr->GetType() != Token::Type::SEMICOLON) { return ErrorHandling("Expected a \";\" SEMICOLON"); }
    children.emplace_back(std::make_unique<GenericTokenParseTreeNode>(current_token_ptr->GetSourceCodeReference()));  // SEMICOLON ";"
    // Let lexer produce the next token.
    current_token_ptr = std::make_unique<Token>(lexer.ProduceNextToken());
    return std::make_unique<NonTerminalParseTreeNode>(BuildChildrenSourceCodeReference(children), ParseTreeNode::Type::ConstantDeclarations, std::move(children));
}
//---------------------------------------------------------------------------
std::unique_ptr<ParseTreeNode> Parser::ParseDeclaratorList() {
    assert(current_token_ptr->GetType() == Token::Type::IDENTIFIER && " Pre-condition of calling this function: IDENTIFIER.");
    std::vector<std::unique_ptr<ParseTreeNode>> children;
    children.emplace_back(std::make_unique<IdentifierParseTreeNode>(current_token_ptr->GetSourceCodeReference()));

    // The repetition part inside of a declarator-list: { "," identifier }.
    while ( (current_token_ptr = std::make_unique<Token>(lexer.ProduceNextToken())) /* Let the lexer produce the next token. */
            && current_token_ptr->GetType() == Token::Type::COMMA) {
        children.emplace_back(std::make_unique<GenericTokenParseTreeNode>(current_token_ptr->GetSourceCodeReference()));  // "COMMA"
        // Let lexer produce the next token.
        current_token_ptr = std::make_unique<Token>(lexer.ProduceNextToken());
        if (current_token_ptr->GetType() != Token::Type::IDENTIFIER) { return ErrorHandling("Expected identifier", true); }
        children.emplace_back(std::make_unique<IdentifierParseTreeNode>(current_token_ptr->GetSourceCodeReference()));  // IDENTIFIER
    }
    assert(current_token_ptr && current_token_ptr->GetType() != Token::Type::COMMA && " Post-condition of calling this function: not COMMA.");
    return std::make_unique<NonTerminalParseTreeNode>(BuildChildrenSourceCodeReference(children), ParseTreeNode::Type::DeclaratorList, std::move(children));
}
//---------------------------------------------------------------------------
std::unique_ptr<ParseTreeNode> Parser::ParseInitDeclaratorList() {
    assert(current_token_ptr->GetType() == Token::Type::IDENTIFIER && " Pre-condition of calling this function: IDENTIFIER.");
    std::unique_ptr<ParseTreeNode> init_declarator = ParseInitDeclarator();  // Should advance to next token after this function call.
    if (!init_declarator) { return nullptr; }
    std::vector<std::unique_ptr<ParseTreeNode>> children;
    children.emplace_back(std::move(init_declarator));

    // The repetition part inside of a init-declarator-list: { "," init-declarator }.
    while (current_token_ptr && current_token_ptr->GetType() == Token::Type::COMMA) {
        children.emplace_back(std::make_unique<GenericTokenParseTreeNode>(current_token_ptr->GetSourceCodeReference()));  // "COMMA"
        // Let lexer produce the next token.
        current_token_ptr = std::make_unique<Token>(lexer.ProduceNextToken());
        if (!current_token_ptr) { return nullptr; }
        init_declarator = ParseInitDeclarator();
        if (!init_declarator) { return nullptr; }
        children.emplace_back(std::move(init_declarator));
    }
    assert(current_token_ptr && current_token_ptr->GetType() != Token::Type::COMMA && " Post-condition of calling this function: not COMMA.");
    return std::make_unique<NonTerminalParseTreeNode>(BuildChildrenSourceCodeReference(children), ParseTreeNode::Type::InitDeclaratorList, std::move(children));
}
//---------------------------------------------------------------------------
std::unique_ptr<ParseTreeNode> Parser::ParseInitDeclarator() {
    assert(current_token_ptr->GetType() == Token::Type::IDENTIFIER && " Pre-condition of calling this function: IDENTIFIER.");
    std::vector<std::unique_ptr<ParseTreeNode>> children;
    children.emplace_back(std::make_unique<IdentifierParseTreeNode>(current_token_ptr->GetSourceCodeReference()));  // IDENTIFIER
    // Let lexer produce the next token.
    current_token_ptr = std::make_unique<Token>(lexer.ProduceNextToken());

    if (current_token_ptr->GetType() != Token::Type::EQUAL) { return ErrorHandling("Expected \"=\" equal sign", true); }
    children.emplace_back(std::make_unique<GenericTokenParseTreeNode>(current_token_ptr->GetSourceCodeReference()));  // "="
    // Let lexer produce the next token.
    current_token_ptr = std::make_unique<Token>(lexer.ProduceNextToken());

    if (current_token_ptr->GetType() != Token::Type::LITERAL) { return ErrorHandling("Expected literal", true); }
    children.emplace_back(std::make_unique<LiteralParseTreeNode>(current_token_ptr->GetSourceCodeReference(), std::stoll(std::string(current_token_ptr->GetSourceCodeReference().GetSourceCode()))));
    // Let lexer produce the next token.
    current_token_ptr = std::make_unique<Token>(lexer.ProduceNextToken());
    return std::make_unique<NonTerminalParseTreeNode>(BuildChildrenSourceCodeReference(children), ParseTreeNode::Type::InitDeclarator, std::move(children));
}
//---------------------------------------------------------------------------
std::unique_ptr<ParseTreeNode> Parser::ParseCompoundStatement() {
    if (current_token_ptr->GetType() != Token::Type::BEGIN) { return ErrorHandling("Expected \"BEGIN\""); }
    std::vector<std::unique_ptr<ParseTreeNode>> children;
    children.emplace_back(std::make_unique<GenericTokenParseTreeNode>(current_token_ptr->GetSourceCodeReference()));  // "BEGIN"
    // Let lexer produce the next token.
    current_token_ptr = std::make_unique<Token>(lexer.ProduceNextToken());

    std::unique_ptr<ParseTreeNode> statement_list = ParseStatementList();  // Should advance to next token after this function call.
    if (!statement_list) { return nullptr; }
    children.emplace_back(std::move(statement_list));

    if (current_token_ptr->GetType() != Token::Type::END) { return ErrorHandling("Expected \"END\""); }
    children.emplace_back(std::make_unique<GenericTokenParseTreeNode>(current_token_ptr->GetSourceCodeReference()));  // "END"

    // Let lexer produce the next token.
    current_token_ptr = std::make_unique<Token>(lexer.ProduceNextToken());
    return std::make_unique<NonTerminalParseTreeNode>(BuildChildrenSourceCodeReference(children), ParseTreeNode::Type::CompoundStatement, std::move(children));
}
//---------------------------------------------------------------------------
std::unique_ptr<ParseTreeNode> Parser::ParseStatementList() {
    std::unique_ptr<ParseTreeNode> statement = ParseStatement();  // Should advance to next token after this function call.
    if (!statement) { return nullptr; }
    std::vector<std::unique_ptr<ParseTreeNode>> children;
    children.emplace_back(std::move(statement));

    // The repetition part inside of a statement-list : { ";" statement }.
    while (current_token_ptr->GetType() == Token::Type::SEMICOLON) {
        children.emplace_back(std::make_unique<GenericTokenParseTreeNode>(current_token_ptr->GetSourceCodeReference()));  // SEMICOLON ";"
        current_token_ptr = std::make_unique<Token>(lexer.ProduceNextToken());
        statement = ParseStatement();
        if (!statement) { return nullptr; }
        children.emplace_back(std::move(statement));
    }
    return std::make_unique<NonTerminalParseTreeNode>(BuildChildrenSourceCodeReference(children), ParseTreeNode::Type::StatementList, std::move(children));
}
//---------------------------------------------------------------------------
std::unique_ptr<ParseTreeNode> Parser::ParseStatement() {
    std::vector<std::unique_ptr<ParseTreeNode>> children;
    if (current_token_ptr->GetType() == Token::Type::RETURN) {
        children.emplace_back(std::make_unique<GenericTokenParseTreeNode>(current_token_ptr->GetSourceCodeReference()));  // "RETURN"
        current_token_ptr = std::make_unique<Token>(lexer.ProduceNextToken());
        std::unique_ptr<ParseTreeNode> additive_expression = ParseAdditiveExpression();
        if (!additive_expression) { return nullptr; }
        children.emplace_back(std::move(additive_expression));
    } else {
        std::unique_ptr<ParseTreeNode> assignment_expression = ParseAssignmentExpresion();
        if (!assignment_expression) { return nullptr; }
        children.emplace_back(std::move(assignment_expression));
    }
    return std::make_unique<NonTerminalParseTreeNode>(BuildChildrenSourceCodeReference(children), ParseTreeNode::Type::Statement, std::move(children));
}
//---------------------------------------------------------------------------
std::unique_ptr<ParseTreeNode> Parser::ParseAssignmentExpresion() {
    // identifier
    if (current_token_ptr->GetType() != Token::Type::IDENTIFIER) { return ErrorHandling("Expected identifier"); }
    std::vector<std::unique_ptr<ParseTreeNode>> children;
    children.emplace_back(std::make_unique<IdentifierParseTreeNode>(current_token_ptr->GetSourceCodeReference()));  // IDENTIFIER
    current_token_ptr = std::make_unique<Token>(lexer.ProduceNextToken());

    // ":=
    if (current_token_ptr->GetType() != Token::Type::ASSIGNMENT) { return ErrorHandling("Expected assignment"); }
    children.emplace_back(std::make_unique<GenericTokenParseTreeNode>(current_token_ptr->GetSourceCodeReference()));  // ASSIGNMENT
    current_token_ptr = std::make_unique<Token>(lexer.ProduceNextToken());

    // additive-expression
    std::unique_ptr<ParseTreeNode> additive_expression = ParseAdditiveExpression();
    if (!additive_expression) { return nullptr; }
    children.emplace_back(std::move(additive_expression));
    return std::make_unique<NonTerminalParseTreeNode>(BuildChildrenSourceCodeReference(children), ParseTreeNode::Type::AssignmentExpression, std::move(children));
}
//---------------------------------------------------------------------------
std::unique_ptr<ParseTreeNode> Parser::ParseAdditiveExpression() {
    // multiplicative-expression
    std::unique_ptr<ParseTreeNode> multiplicative_expression = ParseMultiplicativeExpression();
    if (!multiplicative_expression) { return nullptr; }
    std::vector<std::unique_ptr<ParseTreeNode>> children;
    children.emplace_back(std::move(multiplicative_expression));

    // [ ( "+" | "-" ) additive-expression ]
    if (current_token_ptr->GetType() == Token::Type::PLUS || current_token_ptr->GetType() == Token::Type::MINUS) {
        if (current_token_ptr->GetType() == Token::Type::PLUS) {
            children.emplace_back(std::make_unique<OperatorAlternationParseTreeNode>(current_token_ptr->GetSourceCodeReference(), OperatorAlternationParseTreeNode::Plus));
        } else {
            children.emplace_back(std::make_unique<OperatorAlternationParseTreeNode>(current_token_ptr->GetSourceCodeReference(), OperatorAlternationParseTreeNode::Minus));
        }
        current_token_ptr = std::make_unique<Token>(lexer.ProduceNextToken());
        std::unique_ptr<ParseTreeNode> additive_expression = ParseAdditiveExpression();
        if (!additive_expression) { return nullptr; }
        children.emplace_back(std::move(additive_expression));
    }
    return std::make_unique<NonTerminalParseTreeNode>(BuildChildrenSourceCodeReference(children), ParseTreeNode::Type::AdditiveExpression, std::move(children));
}
//---------------------------------------------------------------------------
std::unique_ptr<ParseTreeNode> Parser::ParseMultiplicativeExpression() {
    // unary-expression
    std::unique_ptr<ParseTreeNode> unary_expression = ParseUnaryExpression();
    if (!unary_expression) { return nullptr; }
    std::vector<std::unique_ptr<ParseTreeNode>> children;
    children.emplace_back(std::move(unary_expression));

    // [ ( "*" | "/" ) multiplicative-expression ]
    if (current_token_ptr->GetType() == Token::Type::MUL || current_token_ptr->GetType() == Token::Type::DIV) {
        if (current_token_ptr->GetType() == Token::Type::MUL) {
            children.emplace_back(std::make_unique<OperatorAlternationParseTreeNode>(current_token_ptr->GetSourceCodeReference(), OperatorAlternationParseTreeNode::Multiply));
        } else {
            children.emplace_back(std::make_unique<OperatorAlternationParseTreeNode>(current_token_ptr->GetSourceCodeReference(), OperatorAlternationParseTreeNode::Divide));
        }
        current_token_ptr = std::make_unique<Token>(lexer.ProduceNextToken());
        std::unique_ptr<ParseTreeNode> multiplicative_expression = ParseMultiplicativeExpression();
        if (!multiplicative_expression) { return nullptr; }
        children.emplace_back(std::move(multiplicative_expression));
    }
    return std::make_unique<NonTerminalParseTreeNode>(BuildChildrenSourceCodeReference(children), ParseTreeNode::Type::MultiplicativeExpression, std::move(children));
}
//---------------------------------------------------------------------------
std::unique_ptr<ParseTreeNode> Parser::ParseUnaryExpression() {
    std::vector<std::unique_ptr<ParseTreeNode>> children;

    // [ "+" | "-" ]
    if (current_token_ptr->GetType() == Token::Type::PLUS) {
        children.emplace_back(std::make_unique<OperatorAlternationParseTreeNode>(current_token_ptr->GetSourceCodeReference(), OperatorAlternationParseTreeNode::Plus));
        current_token_ptr = std::make_unique<Token>(lexer.ProduceNextToken());
    } else if (current_token_ptr->GetType() == Token::Type::MINUS) {
        children.emplace_back(std::make_unique<OperatorAlternationParseTreeNode>(current_token_ptr->GetSourceCodeReference(), OperatorAlternationParseTreeNode::Minus));
        current_token_ptr = std::make_unique<Token>(lexer.ProduceNextToken());
    }

    // primary-expression.
    std::unique_ptr<ParseTreeNode> primary_expression = ParsePrimaryExpression();
    if (!primary_expression) { return nullptr; }
    children.emplace_back(std::move(primary_expression));
    return std::make_unique<NonTerminalParseTreeNode>(BuildChildrenSourceCodeReference(children), ParseTreeNode::Type::UnaryExpression, std::move(children));
}
//---------------------------------------------------------------------------
std::unique_ptr<ParseTreeNode> Parser::ParsePrimaryExpression() {
    std::vector<std::unique_ptr<ParseTreeNode>> children;

    if (current_token_ptr->GetType() == Token::Type::IDENTIFIER) {
        children.emplace_back(std::make_unique<IdentifierParseTreeNode>(current_token_ptr->GetSourceCodeReference()));
    } else if (current_token_ptr->GetType() == Token::Type::LITERAL) {
        children.emplace_back(std::make_unique<LiteralParseTreeNode>(current_token_ptr->GetSourceCodeReference(), std::stoll(std::string(current_token_ptr->GetSourceCodeReference().GetSourceCode()))));
    } else if (current_token_ptr->GetType() == Token::Type::LEFT_PARENTHESE) {
        children.emplace_back(std::make_unique<GenericTokenParseTreeNode>(current_token_ptr->GetSourceCodeReference()));  // "("
        auto left_parenthese = std::move(current_token_ptr);
        current_token_ptr = std::make_unique<Token>(lexer.ProduceNextToken());
        std::unique_ptr<ParseTreeNode> additive_expression = ParseAdditiveExpression();
        if (!additive_expression) { return nullptr; }
        children.emplace_back(std::move(additive_expression));
        if (current_token_ptr && current_token_ptr->GetType() != Token::Type::RIGHT_PARENTHESE) {
            current_token_ptr->GetSourceCodeReference().PrintContext("Expected a ')'");
            left_parenthese->GetSourceCodeReference().PrintContext("To match this previous '('");
            return nullptr;
        }
        children.emplace_back(std::make_unique<GenericTokenParseTreeNode>(current_token_ptr->GetSourceCodeReference()));  // ")"
    } else {
        return ErrorHandling("Expected an identifier, literal or '('");
    }
    current_token_ptr = std::make_unique<Token>(lexer.ProduceNextToken());
    return std::make_unique<NonTerminalParseTreeNode>(BuildChildrenSourceCodeReference(children), ParseTreeNode::Type::PrimaryExpression, std::move(children));
}
//---------------------------------------------------------------------------
} // namespace pljit
//---------------------------------------------------------------------------
