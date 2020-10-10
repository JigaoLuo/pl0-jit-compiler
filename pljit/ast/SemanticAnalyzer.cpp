//---------------------------------------------------------------------------
#include "ast/SemanticAnalyzer.hpp"
#include <cassert>
#include <iostream>
#include <vector>
#include <memory>
//---------------------------------------------------------------------------
namespace pljit {
//---------------------------------------------------------------------------
std::unique_ptr<FunctionAST> SemanticAnalyzer::AnalyzeParseTree(std::unique_ptr<NonTerminalParseTreeNode> node) {
    /// function-definition = [ parameter-declarations ]
    ///                       [ variable-declarations ]
    ///                       [ constant-declarations ]
    ///                       compound-statement
    ///                       ".".
    std::vector<std::unique_ptr<StatementAST>> children;
    auto root_children_iterator = node->GetChildren().begin();
    assert(root_children_iterator != node->GetChildren().end() || !node->GetChildren().empty());

    // parameter-declarations:
    if ((*root_children_iterator)->GetType() == ParseTreeNode::Type::ParameterDeclarations) {
        const bool error = AnalyzeDeclarations(static_cast<const NonTerminalParseTreeNode*>(root_children_iterator->get()), Symbol::Type::PARAMETER);
        if (!error) { return nullptr; }
        ++root_children_iterator;
    }

    // variable-declarations:
    if ((*root_children_iterator)->GetType() == ParseTreeNode::Type::VariableDeclarations) {
        const bool error = AnalyzeDeclarations(static_cast<const NonTerminalParseTreeNode*>(root_children_iterator->get()), Symbol::Type::VARIABLE);
        if (!error) { return nullptr; }
        ++root_children_iterator;
    }

    // constant-declarations:
    if ((*root_children_iterator)->GetType() == ParseTreeNode::Type::ConstantDeclarations) {
        const bool error = AnalyzeConstDeclarations(static_cast<const NonTerminalParseTreeNode*>(root_children_iterator->get()));
        if (!error) { return nullptr; }
        ++root_children_iterator;
    }

    // compound statement:
    // compound-statement = "BEGIN" statement-list "END".
    const auto* const compound_statement = static_cast<NonTerminalParseTreeNode*>(root_children_iterator->get());
    assert(compound_statement && " Parser ensures the compound statement exists.");
    assert(compound_statement->GetChildren().size() == 3);
    assert(compound_statement->GetChildren().front()->GetType() == ParseTreeNode::Type::GenericToken);  // "BEGIN"
    assert(compound_statement->GetChildren().back()->GetType() == ParseTreeNode::Type::GenericToken);  // "END"

    // statement-list = statement { ";" statement }.
    assert(compound_statement->GetChildren()[1]->GetType() == ParseTreeNode::Type::StatementList);
    const auto* const statement_list = static_cast<NonTerminalParseTreeNode*>(compound_statement->GetChildren()[1].get());
    if (!statement_list) { return nullptr; }

    // statement = assignment-expression
    //           | "RETURN" additive-expression.
    bool return_found = false;
    bool last_child_is_statement = false;

    for (auto& child: statement_list->GetChildren()) {
        assert(child->GetType() == ParseTreeNode::Type::Statement || child->GetType() == ParseTreeNode::Type::GenericToken);
        assert(last_child_is_statement || child->GetType() == ParseTreeNode::Type::Statement);  // The last it not an statement => this current one is an statement.

        if (child->GetType() == ParseTreeNode::Type::Statement) {
            last_child_is_statement = true;
            auto* statement = static_cast<NonTerminalParseTreeNode*>(child.get());
            std::unique_ptr<StatementAST> statement_ast = AnalyzeStatement(statement);
            if (!statement_ast) { return nullptr; }
            if (statement_ast->GetType() == ASTNode::Type::ReturnStatement) {
                // Allowed, statements after the return statement. <- Fixed by Milestone 5.
                return_found = true;
            }
            children.emplace_back(std::move(statement_ast));
        } else {
            last_child_is_statement = false;
        }
    }

    if (!return_found) {
        std::cerr << "Missing return statement." << std::endl;
        return nullptr;
    }
    return std::make_unique<FunctionAST>(std::move(children));
}
//---------------------------------------------------------------------------
const SymbolTable& SemanticAnalyzer::GetSymbolTable() const { return symbol_table; }
//---------------------------------------------------------------------------
bool SemanticAnalyzer::AnalyzeDeclarations(const NonTerminalParseTreeNode* const node, Symbol::Type type) {
    // parameter-declarations = "PARAM" declarator-list ";".
    // variable-declarations = "VAR" declarator-list ";".
    assert(node->GetType() == ParseTreeNode::Type::ParameterDeclarations || node->GetType() == ParseTreeNode::Type::VariableDeclarations);
    assert(type == Symbol::Type::PARAMETER || type == Symbol::Type::VARIABLE);
    assert(node->GetChildren().size() == 3);
    assert(node->GetChildren()[0]->GetType() == ParseTreeNode::Type::GenericToken);  // "PARAM" or "VAR".
    assert(node->GetChildren()[1]->GetType() == ParseTreeNode::Type::DeclaratorList);  // Declarator List.

    // declarator-list = identifier { "," identifier }.
    const auto* const declarator_list = static_cast<const NonTerminalParseTreeNode*>(node->GetChildren()[1].get());
    bool last_child_is_identifier = false;
    for (auto& child: declarator_list->GetChildren()) {
        assert(child->GetType() == ParseTreeNode::Type::Identifier || child->GetType() == ParseTreeNode::Type::GenericToken);
        assert(last_child_is_identifier || child->GetType() == ParseTreeNode::Type::Identifier);  // The last it not an identifier => this current one is an identifier.

        if (child->GetType() == ParseTreeNode::Type::Identifier) {
            last_child_is_identifier = true;
            const auto* const identifer_node = static_cast<const IdentifierParseTreeNode*>(child.get());
            if (symbol_table.find(identifer_node->GetName()) == symbol_table.end()) {
                symbol_table.emplace(identifer_node->GetName(), Symbol(type, (type != Symbol::Type::VARIABLE), child->GetSourceCodeReference()));
            } else {
                // Duplicate identifier name found!
                identifer_node->GetSourceCodeReference().PrintContext("The same identifier being declared twice.");
                return false /* failure */;
            }
        } else {
            last_child_is_identifier = false;
        }
    }
    return true;
}
//---------------------------------------------------------------------------
bool SemanticAnalyzer::AnalyzeConstDeclarations(const NonTerminalParseTreeNode* const node) {
    // constant-declarations = "CONST" init-declarator-list ";".
    assert(node->GetType() == ParseTreeNode::Type::ConstantDeclarations);
    assert(node->GetChildren().size() == 3);
    assert(node->GetChildren()[0]->GetType() == ParseTreeNode::Type::GenericToken);  // "CONST".
    assert(node->GetChildren()[1]->GetType() == ParseTreeNode::Type::InitDeclaratorList);  // Init Declarator List.
    assert(node->GetChildren()[2]->GetType() == ParseTreeNode::Type::GenericToken);  // ";".

    // init-declarator-list = init-declarator { "," init-declarator }.
    const auto* const init_declarator_list = static_cast<const NonTerminalParseTreeNode*>(node->GetChildren()[1].get());
    bool last_child_is_identifier = false;
    for (auto& init_declarator: init_declarator_list->GetChildren()) {
        assert(init_declarator->GetType() == ParseTreeNode::Type::InitDeclarator || init_declarator->GetType() == ParseTreeNode::Type::GenericToken);
        assert(last_child_is_identifier || init_declarator->GetType() == ParseTreeNode::Type::InitDeclarator);  // The last it not an identifier => this current one is an identifier.

        bool identifier_found = false;
        bool equal_found = false;
        std::string_view last_name;
        if (init_declarator->GetType() == ParseTreeNode::Type::InitDeclarator) {
            last_child_is_identifier = true;
            for (auto& child: static_cast<const NonTerminalParseTreeNode* const>(init_declarator.get())->GetChildren()) {
                // init-declarator = identifier "=" literal.
                assert(child->GetType() == ParseTreeNode::Type::Identifier || child->GetType() == ParseTreeNode::Type::GenericToken || child->GetType() == ParseTreeNode::Type::Literal);
                assert(identifier_found || child->GetType() == ParseTreeNode::Type::Identifier);  // Identifier.
                assert((!identifier_found || equal_found) || child->GetType() == ParseTreeNode::Type::GenericToken);  // "=".
                assert((!identifier_found || !equal_found) || child->GetType() == ParseTreeNode::Type::Literal);  // Literal.

                if (child->GetType() == ParseTreeNode::Type::Identifier) {
                    identifier_found = true;
                    const auto* const identifer_node = static_cast<const IdentifierParseTreeNode*>(child.get());
                    if (symbol_table.find(identifer_node->GetName()) == symbol_table.end()) {
                        last_name = identifer_node->GetName();
                        symbol_table.emplace(last_name, Symbol(Symbol::Type::CONSTANT, true, child->GetSourceCodeReference()));
                    } else {
                        // Duplicate identifier name found!
                        identifer_node->GetSourceCodeReference().PrintContext("The same identifier being declared twice.");
                        return false;
                    }
                } else if (child->GetType() == ParseTreeNode::Type::GenericToken) {
                    assert(identifier_found);
                    equal_found = true;
                } else {
                    assert(child->GetType() == ParseTreeNode::Type::Literal);
                    assert(equal_found && identifier_found);
                    equal_found = false;
                    identifier_found = false;
                    auto it = symbol_table.find(last_name);
                    assert(it != symbol_table.end());
                    const auto* const literal_node = static_cast<const LiteralParseTreeNode*>(child.get());
                    it->second.SetValue(literal_node->GetValue());
                }
            }
        } else {
            last_child_is_identifier = false;
        }
    }
    return true;
}
//---------------------------------------------------------------------------
std::unique_ptr<StatementAST> SemanticAnalyzer::AnalyzeStatement(NonTerminalParseTreeNode* node) {
    // statement = assignment-expression
    //           | "RETURN" additive-expression.
    const auto* const statement = static_cast<const NonTerminalParseTreeNode*>(node);
    assert(statement->GetType() == ParseTreeNode::Type::Statement);
    if (statement->GetChildren().front()->GetType() == ParseTreeNode::Type::AssignmentExpression) {
        // assignment-expression.
        assert(statement->GetChildren().size() == 1);
        const auto* const assignment_expression = static_cast<const NonTerminalParseTreeNode*>(statement->GetChildren().front().get());
        // assignment-expression = identifier ":=" additive-expression.
        const auto* const identifer_node = static_cast<const IdentifierParseTreeNode*>(assignment_expression->GetChildren().front().get());
        assert(identifer_node->GetType() == ParseTreeNode::Type::Identifier);
        assert(assignment_expression->GetChildren().size() == 3);
        assert(assignment_expression->GetChildren()[1]->GetType() == ParseTreeNode::Type::GenericToken);
        assert(assignment_expression->GetChildren()[2]->GetType() == ParseTreeNode::Type::AdditiveExpression);
        auto it = symbol_table.find(identifer_node->GetName());
        if (it == symbol_table.end()) {
            identifer_node->GetSourceCodeReference().PrintContext("Using an undeclared identifier.");
            return nullptr;
        } else if (it->second.GetType() == Symbol::Type::CONSTANT) {
            identifer_node->GetSourceCodeReference().PrintContext("Assigning a value to a constant.");
            return nullptr;
        } else if (it->second.GetType() == Symbol::Type::VARIABLE && !it->second.IfInitialized()) {
            // Set a variable as initialized.
            it->second.SetInitialized();
        }
        assert(it->second.IfInitialized());
        return std::make_unique<AssignmentStatementAST>(std::make_unique<IdentifierPrimaryExpressionAST>(identifer_node->GetName()),
                                                        AnalyzeAdditiveExpression(static_cast<NonTerminalParseTreeNode*>(assignment_expression->GetChildren().back().get())));
    } else {
        // "RETURN" additive-expression.
        assert(statement->GetChildren().size() == 2);
        assert(statement->GetChildren()[0]->GetType() == ParseTreeNode::Type::GenericToken);  // "RETURN".
        assert(statement->GetChildren()[1]->GetType() == ParseTreeNode::Type::AdditiveExpression);  // additive-expression.
        return std::make_unique<ReturnStatementAST>(AnalyzeAdditiveExpression(static_cast<NonTerminalParseTreeNode*>(statement->GetChildren().back().get())));
    }
}
//---------------------------------------------------------------------------
std::unique_ptr<ExpressionAST> SemanticAnalyzer::AnalyzeAdditiveExpression(NonTerminalParseTreeNode* node) {
    // additive-expression = multiplicative-expression [ ( "+" | "-" ) additive-expression ].
    assert(node->GetType() == ParseTreeNode::Type::AdditiveExpression);
    assert(node->GetChildren().front()->GetType() == ParseTreeNode::Type::MultiplicativeExpression);
    if (node->GetChildren().size() == 1) {
        // only *multiplicative-expression*.
        return AnalyzeMultiplicativeExpression(static_cast<NonTerminalParseTreeNode*>(node->GetChildren().front().get()));
    } else {
        // with *additive-expression*.
        assert(node->GetChildren().size() == 3);
        assert(node->GetChildren()[1]->GetType() == ParseTreeNode::Type::GenericToken);
        return std::make_unique<BinaryExpressionAST>
            (static_cast<OperatorAlternationParseTreeNode*>(node->GetChildren()[1].get())->GetOperatorType() == OperatorAlternationParseTreeNode::Plus ? BinaryExpressionAST::BinaryOperator::PLUS : BinaryExpressionAST::BinaryOperator::MINUS /* operand */,
            AnalyzeMultiplicativeExpression(static_cast<NonTerminalParseTreeNode*>(node->GetChildren().front().get())) /* left: multiplicative-expression */,
            AnalyzeAdditiveExpression(static_cast<NonTerminalParseTreeNode*>(node->GetChildren().back().get()))) /* right: additive-expression */;
    }
}
//---------------------------------------------------------------------------
std::unique_ptr<ExpressionAST> SemanticAnalyzer::AnalyzeMultiplicativeExpression(NonTerminalParseTreeNode* node) {
    // multiplicative-expression = unary-expression [ ( "*" | "/" ) multiplicative-expression ].
    assert(node->GetChildren().size() == 3 || node->GetChildren().size() == 1);
    assert(node->GetChildren().front()->GetType() == ParseTreeNode::Type::UnaryExpression);
    const auto* const unary_expression = static_cast<const NonTerminalParseTreeNode*>(node->GetChildren().front().get());

    // unary-expression = [ "+" | "-" ] primary-expression.
    const size_t primary_expression_index = unary_expression->GetChildren().size() == 1 ? 0 : 1;

    // primary-expression = identifier
    //                    | literal
    //                    | "(" additive-expression ")".
    const auto* const primary_expression = static_cast<const NonTerminalParseTreeNode*>(unary_expression->GetChildren()[primary_expression_index].get());

    std::unique_ptr<ExpressionAST> result{nullptr};
    if (primary_expression->GetChildren().front()->GetType() == ParseTreeNode::Type::Identifier) {
        // identifier.
        auto* identifer_node = static_cast<IdentifierParseTreeNode*>(primary_expression->GetChildren()[0].get());
        auto it = symbol_table.find(identifer_node->GetName());
        if (it == symbol_table.end()) {
            identifer_node->GetSourceCodeReference().PrintContext("Using an undeclared identifier.");
            return nullptr;
        } else if (!it->second.IfInitialized()) {
            assert(it->second.GetType() == Symbol::Type::VARIABLE);
            identifer_node->GetSourceCodeReference().PrintContext("Using an uninitialized variable.");
            return nullptr;
        }
        result = std::make_unique<IdentifierPrimaryExpressionAST>(identifer_node->GetName());
    } else if (primary_expression->GetChildren().front()->GetType() == ParseTreeNode::Type::Literal) {
        // literal.
        result = std::make_unique<LiteralPrimaryExpressionAST>(static_cast<LiteralParseTreeNode*>(primary_expression->GetChildren()[0].get())->GetValue());
    } else {
        // "(" additive-expression ")".
        assert(primary_expression->GetChildren().size() == 3);
        assert(primary_expression->GetChildren()[0]->GetType() == ParseTreeNode::Type::GenericToken);
        assert(primary_expression->GetChildren()[1]->GetType() == ParseTreeNode::Type::AdditiveExpression);
        assert(primary_expression->GetChildren()[2]->GetType() == ParseTreeNode::Type::GenericToken);
        result = AnalyzeAdditiveExpression(static_cast<NonTerminalParseTreeNode*>(primary_expression->GetChildren()[1].get()));
    }
    assert(result);

    // unary-expression = [ "+" | "-" ] primary-expression.
    // Process the option: unary operator.
    if (primary_expression_index == 1) {
        result = std::make_unique<UnaryExpressionAST>
            (static_cast<OperatorAlternationParseTreeNode*>(unary_expression->GetChildren()[0].get())->GetOperatorType() == OperatorAlternationParseTreeNode::Plus ? UnaryExpressionAST::UnaryOperator::POSTIVE : UnaryExpressionAST::UnaryOperator::NEGATIVE /* operand */,
             std::move(result) /* primary-expression */);
    }

    // multiplicative-expression = unary-expression [ ( "*" | "/" ) multiplicative-expression ].
    // Process the option: [ ( "*" | "/" ) multiplicative-expression ].
    if (node->GetChildren().size() == 3) {
        assert(node->GetChildren()[1]->GetType() == ParseTreeNode::Type::GenericToken);
        assert(node->GetChildren()[2]->GetType() == ParseTreeNode::Type::MultiplicativeExpression);
        result = std::make_unique<BinaryExpressionAST>(static_cast<OperatorAlternationParseTreeNode*>(node->GetChildren()[1].get())->GetOperatorType() == OperatorAlternationParseTreeNode::Multiply ? BinaryExpressionAST::MUL : BinaryExpressionAST::DIV,
            std::move(result),
            AnalyzeMultiplicativeExpression(static_cast<NonTerminalParseTreeNode*>(node->GetChildren()[2].get())));
    }
    return result;
}
//---------------------------------------------------------------------------
} // namespace pljit
//---------------------------------------------------------------------------
