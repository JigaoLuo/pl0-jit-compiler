#include "parser/Parser.hpp"
#include "ast/SemanticAnalyzer.hpp"
#include <gtest/gtest.h>
//---------------------------------------------------------------------------
namespace pljit {
//---------------------------------------------------------------------------
TEST(AST, IdentifierDeclaredTwice_Test0) {
    const std::string code = "PARAM foo, foo;\n"
                             "BEGIN\n"
                             "    RETURN 42\n"
                             "END.\n";
    SourceCodeManagement scm(code);
    Parser parser(scm);
    std::unique_ptr<NonTerminalParseTreeNode> parse_tree = parser.ParseFunctionDefinition();
    ASSERT_TRUE(parse_tree);
    SemanticAnalyzer semantic_analyzer;
    testing::internal::CaptureStderr();
    std::unique_ptr<FunctionAST> ast = semantic_analyzer.AnalyzeParseTree(std::move(parse_tree));
    const std::string expected_error = "0:11: The same identifier being declared twice.\n"
                                       "PARAM foo, foo;\n"
                                       "           ^~~\n";
    EXPECT_EQ(testing::internal::GetCapturedStderr(), expected_error);
}
//---------------------------------------------------------------------------
TEST(AST, IdentifierDeclaredTwice_Test1) {
    const std::string code = "PARAM foo;\n"
                             "VAR foo;\n"
                             "BEGIN\n"
                             "    RETURN 42\n"
                             "END.\n";
    SourceCodeManagement scm(code);
    Parser parser(scm);
    std::unique_ptr<NonTerminalParseTreeNode> parse_tree = parser.ParseFunctionDefinition();
    ASSERT_TRUE(parse_tree);
    SemanticAnalyzer semantic_analyzer;
    testing::internal::CaptureStderr();
    std::unique_ptr<FunctionAST> ast = semantic_analyzer.AnalyzeParseTree(std::move(parse_tree));
    const std::string expected_error = "1:4: The same identifier being declared twice.\n"
                                       "VAR foo;\n"
                                       "    ^~~\n";
    EXPECT_EQ(testing::internal::GetCapturedStderr(), expected_error);
}
//---------------------------------------------------------------------------
TEST(AST, IdentifierDeclaredTwice_Test2) {
    const std::string code = "PARAM foo;\n"
                             "CONST foo = 42;\n"
                             "BEGIN\n"
                             "    RETURN 42\n"
                             "END.\n";
    SourceCodeManagement scm(code);
    Parser parser(scm);
    std::unique_ptr<NonTerminalParseTreeNode> parse_tree = parser.ParseFunctionDefinition();
    ASSERT_TRUE(parse_tree);
    SemanticAnalyzer semantic_analyzer;
    testing::internal::CaptureStderr();
    std::unique_ptr<FunctionAST> ast = semantic_analyzer.AnalyzeParseTree(std::move(parse_tree));
    const std::string expected_error = "1:6: The same identifier being declared twice.\n"
                                       "CONST foo = 42;\n"
                                       "      ^~~\n";
    EXPECT_EQ(testing::internal::GetCapturedStderr(), expected_error);
}
//---------------------------------------------------------------------------
TEST(AST, UndeclaredIdentifier_Test0) {
    const std::string code = "BEGIN\n"
                             "    RETURN foo\n"
                             "END.\n";
    SourceCodeManagement scm(code);
    Parser parser(scm);
    std::unique_ptr<NonTerminalParseTreeNode> parse_tree = parser.ParseFunctionDefinition();
    ASSERT_TRUE(parse_tree);
    SemanticAnalyzer semantic_analyzer;
    testing::internal::CaptureStderr();
    std::unique_ptr<FunctionAST> ast = semantic_analyzer.AnalyzeParseTree(std::move(parse_tree));
    const std::string expected_error = "1:11: Using an undeclared identifier.\n"
                                       "    RETURN foo\n"
                                       "           ^~~\n";
    EXPECT_EQ(testing::internal::GetCapturedStderr(), expected_error);
}
//---------------------------------------------------------------------------
TEST(AST, UndeclaredIdentifier_Test1) {
    const std::string code = "PARAM foo;\n"
                             "VAR bar;\n"
                             "CONST foobar = 42;\n"
                             "BEGIN\n"
                             "    RETURN width\n"
                             "END.\n";
    SourceCodeManagement scm(code);
    Parser parser(scm);
    std::unique_ptr<NonTerminalParseTreeNode> parse_tree = parser.ParseFunctionDefinition();
    ASSERT_TRUE(parse_tree);
    SemanticAnalyzer semantic_analyzer;
    testing::internal::CaptureStderr();
    std::unique_ptr<FunctionAST> ast = semantic_analyzer.AnalyzeParseTree(std::move(parse_tree));
    const std::string expected_error = "4:11: Using an undeclared identifier.\n"
                                       "    RETURN width\n"
                                       "           ^~~~~\n";
    EXPECT_EQ(testing::internal::GetCapturedStderr(), expected_error);
}
//---------------------------------------------------------------------------
TEST(AST, AssignmentToConstant_Test0) {
    const std::string code = "CONST foo = 42;\n"
                             "BEGIN\n"
                             "    foo := 4242;\n"
                             "    RETURN foo\n"
                             "END.\n";
    SourceCodeManagement scm(code);
    Parser parser(scm);
    std::unique_ptr<NonTerminalParseTreeNode> parse_tree = parser.ParseFunctionDefinition();
    ASSERT_TRUE(parse_tree);
    SemanticAnalyzer semantic_analyzer;
    testing::internal::CaptureStderr();
    std::unique_ptr<FunctionAST> ast = semantic_analyzer.AnalyzeParseTree(std::move(parse_tree));
    const std::string expected_error = "2:4: Assigning a value to a constant.\n"
                                       "    foo := 4242;\n"
                                       "    ^~~\n";
    EXPECT_EQ(testing::internal::GetCapturedStderr(), expected_error);
}
//---------------------------------------------------------------------------
TEST(AST, AssignmentToConstant_Test1) {
    // Assignment with same value. Still not allowed.
    // Also not allowed by other programming languages: C++.
    const std::string code = "CONST foo = 42;\n"
                             "BEGIN\n"
                             "    foo := 42;\n"
                             "    RETURN foo\n"
                             "END.\n";
    SourceCodeManagement scm(code);
    Parser parser(scm);
    std::unique_ptr<NonTerminalParseTreeNode> parse_tree = parser.ParseFunctionDefinition();
    ASSERT_TRUE(parse_tree);
    SemanticAnalyzer semantic_analyzer;
    testing::internal::CaptureStderr();
    std::unique_ptr<FunctionAST> ast = semantic_analyzer.AnalyzeParseTree(std::move(parse_tree));
    const std::string expected_error = "2:4: Assigning a value to a constant.\n"
                                       "    foo := 42;\n"
                                       "    ^~~\n";
    EXPECT_EQ(testing::internal::GetCapturedStderr(), expected_error);
}
//---------------------------------------------------------------------------
TEST(AST, UsingUnitializedVariable) {
    const std::string code = "VAR foo;\n"
                             "BEGIN\n"
                             "    RETURN foo\n"
                             "END.\n";
    SourceCodeManagement scm(code);
    Parser parser(scm);
    std::unique_ptr<NonTerminalParseTreeNode> parse_tree = parser.ParseFunctionDefinition();
    ASSERT_TRUE(parse_tree);
    SemanticAnalyzer semantic_analyzer;
    testing::internal::CaptureStderr();
    std::unique_ptr<FunctionAST> ast = semantic_analyzer.AnalyzeParseTree(std::move(parse_tree));
    const std::string expected_error = "2:11: Using an uninitialized variable.\n"
                                       "    RETURN foo\n"
                                       "           ^~~\n";
    EXPECT_EQ(testing::internal::GetCapturedStderr(), expected_error);
}
//---------------------------------------------------------------------------
TEST(Ast, MissingReturnStatement) {
    const std::string code = "PARAM foo;\n"
                             "BEGIN\n"
                             "    foo := 20\n"
                             "END.\n";
    SourceCodeManagement scm(code);
    Parser parser(scm);
    std::unique_ptr<NonTerminalParseTreeNode> parse_tree = parser.ParseFunctionDefinition();
    ASSERT_TRUE(parse_tree);
    SemanticAnalyzer semantic_analyzer;
    testing::internal::CaptureStderr();
    std::unique_ptr<FunctionAST> ast = semantic_analyzer.AnalyzeParseTree(std::move(parse_tree));
    const std::string expected_error = "Missing return statement.\n";
    EXPECT_EQ(testing::internal::GetCapturedStderr(), expected_error);
}
//---------------------------------------------------------------------------
} // namespace pljit
//---------------------------------------------------------------------------
