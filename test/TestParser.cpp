#include "parser/Parser.hpp"
#include "parser/ParseTreeNode.hpp"
#include <gtest/gtest.h>
//---------------------------------------------------------------------------
namespace pljit {
//---------------------------------------------------------------------------
using PTT = ParseTreeNode::Type;
//---------------------------------------------------------------------------
TEST(Parser, ParameterDeclaration) {
    const std::string code = "PARAM width, CONST, height, depth;";
    const std::string expected_error_msg = "0:13: Expected identifier\n"
                                           "PARAM width, CONST, height, depth;\n"
                                           "             ^~~~~\n";
    SourceCodeManagement scm(code);
    Parser parser(scm);
    testing::internal::CaptureStderr();
    std::unique_ptr<NonTerminalParseTreeNode> parse_tree = parser.ParseFunctionDefinition();
    EXPECT_EQ(testing::internal::GetCapturedStderr(), expected_error_msg);
    parse_tree.release();
}
//---------------------------------------------------------------------------
TEST(Parser, DOT) {
    const std::string code = "BEGIN\nRETURN 1\nEND";
    const std::string expected_error_msg = "2:3: Expected \".\" DOT\n"
                                           "END\n"
                                           "   ^\n";
    SourceCodeManagement scm(code);
    Parser parser(scm);
    testing::internal::CaptureStderr();
    std::unique_ptr<NonTerminalParseTreeNode> parse_tree = parser.ParseFunctionDefinition();
    EXPECT_EQ(testing::internal::GetCapturedStderr(), expected_error_msg);
    parse_tree.release();
}
//---------------------------------------------------------------------------
TEST(Parser, SEMICOLON) {
    const std::string code = "PARAM width";
    const std::string expected_error_msg = "0:11: Expected a \";\" SEMICOLON\n"
                                           "PARAM width\n"
                                           "           ^\n";
    SourceCodeManagement scm(code);
    Parser parser(scm);
    testing::internal::CaptureStderr();
    std::unique_ptr<NonTerminalParseTreeNode> parse_tree = parser.ParseFunctionDefinition();
    EXPECT_EQ(testing::internal::GetCapturedStderr(), expected_error_msg);
    parse_tree.release();
}
//---------------------------------------------------------------------------
TEST(Parser, RIGHT_PARENTHESE) {
    const std::string code = "BEGIN\n"
                             "RETURN 1 * (2 + 3;\n"
                             "END.";
    const std::string expected_error_msg = "1:17: Expected a ')'\n"
                                           "RETURN 1 * (2 + 3;\n"
                                           "                 ^\n"
                                           "1:11: To match this previous '('\n"
                                           "RETURN 1 * (2 + 3;\n"
                                           "           ^\n";
    SourceCodeManagement scm(code);
    Parser parser(scm);
    testing::internal::CaptureStderr();
    std::unique_ptr<NonTerminalParseTreeNode> parse_tree = parser.ParseFunctionDefinition();
    EXPECT_EQ(testing::internal::GetCapturedStderr(), expected_error_msg);
    parse_tree.release();
}
//---------------------------------------------------------------------------
TEST(Parser, BEGIN) {
    const std::string code = "RETURN 2 * (1 + 2 \n"
                             "END.";
    const std::string expected_error_msg = "0:0: Expected \"BEGIN\"\n"
                                           "RETURN 2 * (1 + 2 \n"
                                           "^~~~~~\n";
    SourceCodeManagement scm(code);
    Parser parser(scm);
    testing::internal::CaptureStderr();
    std::unique_ptr<NonTerminalParseTreeNode> parse_tree = parser.ParseFunctionDefinition();
    EXPECT_EQ(testing::internal::GetCapturedStderr(), expected_error_msg);
    parse_tree.release();
}
//---------------------------------------------------------------------------
TEST(Parser, PrimaryExpression) {
    const std::string code = "BEGIN\n"
                             "RETURN 1 * VAR\n"
                             "END.";
    const std::string expected_error_msg = "1:11: Expected an identifier, literal or '('\n"
                                           "RETURN 1 * VAR\n"
                                           "           ^~~\n";
    SourceCodeManagement scm(code);
    Parser parser(scm);
    testing::internal::CaptureStderr();
    std::unique_ptr<NonTerminalParseTreeNode> parse_tree = parser.ParseFunctionDefinition();
    EXPECT_EQ(testing::internal::GetCapturedStderr(), expected_error_msg);
    parse_tree.release();
}
//---------------------------------------------------------------------------
TEST(Parser, Assignment) {
    const std::string code = "BEGIN\n"
                             "foo 12 * 3;\n"
                             "RETURN 1\n"
                             "END";
    const std::string expected_error_msg = "1:4: Expected assignment\n"
                                           "foo 12 * 3;\n"
                                           "    ^~\n";
    SourceCodeManagement scm(code);
    Parser parser(scm);
    testing::internal::CaptureStderr();
    std::unique_ptr<NonTerminalParseTreeNode> parse_tree = parser.ParseFunctionDefinition();
    EXPECT_EQ(testing::internal::GetCapturedStderr(), expected_error_msg);
    parse_tree.release();
}
//---------------------------------------------------------------------------
TEST(ParserDefaultNodesTest, Verbose) {
    const std::string code = "BEGIN\n"
                             "RETURN 42\n"
                             "END.";
    SourceCodeManagement scm(code);
    Parser parser(scm);
    std::unique_ptr<NonTerminalParseTreeNode> parse_tree = parser.ParseFunctionDefinition();

    // function-definition
    EXPECT_EQ(parse_tree->GetType(), ParseTreeNode::Type::FunctionDefinition);
    EXPECT_EQ(parse_tree->GetChildren()[0]->GetType(), ParseTreeNode::Type::CompoundStatement);
    EXPECT_EQ(parse_tree->GetChildren()[1]->GetType(), ParseTreeNode::Type::GenericToken);

    // compound-statement = "BEGIN" statement-list "END".
    auto compound_statement = static_cast<NonTerminalParseTreeNode*>(parse_tree->GetChildren()[0].get());
    EXPECT_EQ(compound_statement->GetChildren()[0]->GetType(), ParseTreeNode::Type::GenericToken);  // "BEGIN"
    EXPECT_EQ(compound_statement->GetChildren()[1]->GetType(), ParseTreeNode::Type::StatementList);  // statement list
    EXPECT_EQ(compound_statement->GetChildren()[2]->GetType(), ParseTreeNode::Type::GenericToken);  // "END"

    // statement-list = statement { ";" statement }.
    auto* statement_list = static_cast<NonTerminalParseTreeNode*>(compound_statement->GetChildren()[1].get());
    EXPECT_EQ(statement_list->GetChildren()[0]->GetType(), ParseTreeNode::Type::Statement);  // statement

    // statement = assignment-expression
    //           | "RETURN" additive-expression.
    auto* return_statement = static_cast<NonTerminalParseTreeNode*>(statement_list->GetChildren()[0].get());
    EXPECT_EQ(return_statement->GetChildren()[0]->GetType(), ParseTreeNode::Type::GenericToken);  // "RETURN"
    EXPECT_EQ(return_statement->GetChildren()[1]->GetType(), ParseTreeNode::Type::AdditiveExpression);

    // additive-expression = multiplicative-expression [ ( "+" | "-" ) additive-expression ].
    auto* additive_expression = static_cast<NonTerminalParseTreeNode*>(return_statement->GetChildren()[1].get());
    EXPECT_EQ(additive_expression->GetChildren()[0]->GetType(), ParseTreeNode::Type::MultiplicativeExpression);

    // multiplicative-expression = unary-expression [ ( "*" | "/" ) multiplicative-expression ].
    auto* multiplicative_expression = static_cast<NonTerminalParseTreeNode*>(additive_expression->GetChildren()[0].get());
    EXPECT_EQ(multiplicative_expression->GetChildren()[0]->GetType(), ParseTreeNode::Type::UnaryExpression);

    // unary-expression = [ "+" | "-" ] primary-expression.
    auto* unary_expression = static_cast<NonTerminalParseTreeNode*>(multiplicative_expression->GetChildren()[0].get());
    EXPECT_EQ(unary_expression->GetChildren()[0]->GetType(), ParseTreeNode::Type::PrimaryExpression);

    // primary-expression = identifier
    //                    | literal
    //                    | "(" additive-expression ")".
    auto* primary_expression = static_cast<NonTerminalParseTreeNode*>(unary_expression->GetChildren()[0].get());
    EXPECT_EQ(primary_expression->GetChildren()[0]->GetType(), ParseTreeNode::Type::Literal);
}
//---------------------------------------------------------------------------
} // namespace pljit
//---------------------------------------------------------------------------
