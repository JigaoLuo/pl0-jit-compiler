#include "parser/Parser.hpp"
#include "ast/SemanticAnalyzer.hpp"
#include "ast/ASTNodeVisitorDot.hpp"
#include "optimization/DeadCodeElimination.hpp"
#include <gtest/gtest.h>
//---------------------------------------------------------------------------
namespace pljit {
//---------------------------------------------------------------------------
TEST(Optimization, DeadCodeElimination0) {
    const std::string code = "PARAM pa, pb;\n"
                             "VAR va, vb;\n"
                             "CONST ca=1, cb=2;\n"
                             "BEGIN\n"
                             "    RETURN pa + pb;\n"
                             "    pa := 42;\n"
                             "    pb := 42;\n"
                             "    va := 42;\n"
                             "    vb := 42\n"
                             "END.\n";
    SourceCodeManagement scm(code);
    Parser parser(scm);
    std::unique_ptr<NonTerminalParseTreeNode> parse_tree = parser.ParseFunctionDefinition();
    ASSERT_TRUE(parse_tree);
    SemanticAnalyzer semantic_analyzer;
    std::unique_ptr<FunctionAST> ast = semantic_analyzer.AnalyzeParseTree(std::move(parse_tree));
    ASSERT_TRUE(ast);
    // Optimization Pass: Dead Code Elimination.
    OptimizeDeadCode opc;
    opc.Optimize(*ast);
    testing::internal::CaptureStdout();
    ASTNodeVisitorDot visitor;
    visitor.Visit(*ast);
    const std::string expected = "digraph {\n"
                                 "0 [label=\"Function\"];\n"
                                 "0 -> 1\n"
                                 "1 [label=\"Return Statement\"];\n"
                                 "1 -> 2\n"
                                 "2 [label=\" Binary Operator: +\"];\n"
                                 "2 -> 3\n"
                                 "3 [label=\" Identifier:pa \"];\n"
                                 "2 -> 4\n"
                                 "4 [label=\" Identifier:pb \"];\n"
                                 "}\n";
    EXPECT_EQ(testing::internal::GetCapturedStdout(), expected);
}
//---------------------------------------------------------------------------
TEST(Optimization, DeadCodeElimination1) {
    const std::string code = "PARAM width, height;\n"
                             "VAR temp, foo;\n"
                             "CONST hello = 12, test = 2000;\n"
                             "BEGIN\n"
                             "    RETURN 12 * (8 - 5);\n"
                             "    width := 42;\n"
                             "    height := 42;\n"
                             "    temp := 42;\n"
                             "    foo := 42\n"
                             "END.\n";
    SourceCodeManagement scm(code);
    Parser parser(scm);
    std::unique_ptr<NonTerminalParseTreeNode> parse_tree = parser.ParseFunctionDefinition();
    ASSERT_TRUE(parse_tree);
    SemanticAnalyzer semantic_analyzer;
    std::unique_ptr<FunctionAST> ast = semantic_analyzer.AnalyzeParseTree(std::move(parse_tree));
    ASSERT_TRUE(ast);
    // Optimization Pass: Dead Code Elimination.
    OptimizeDeadCode opc;
    opc.Optimize(*ast);
    testing::internal::CaptureStdout();
    ASTNodeVisitorDot visitor;
    visitor.Visit(*ast);
    const std::string expected = "digraph {\n"
                                 "0 [label=\"Function\"];\n"
                                 "0 -> 1\n"
                                 "1 [label=\"Return Statement\"];\n"
                                 "1 -> 2\n"
                                 "2 [label=\" Binary Operator: *\"];\n"
                                 "2 -> 3\n"
                                 "3 [label=\" Literal:12 \"];\n"
                                 "2 -> 4\n"
                                 "4 [label=\" Binary Operator: -\"];\n"
                                 "4 -> 5\n"
                                 "5 [label=\" Literal:8 \"];\n"
                                 "4 -> 6\n"
                                 "6 [label=\" Literal:5 \"];\n"
                                 "}\n";
    EXPECT_EQ(testing::internal::GetCapturedStdout(), expected);
}
//---------------------------------------------------------------------------
TEST(Optimization, DeadCodeElimination2) {
    const std::string code = "PARAM a, b, c;\n"
                             "VAR d, e, f;\n"
                             "CONST g=1, h=2, i=3;\n"
                             "BEGIN\n"
                             "    a := a + 1;\n"
                             "    b := b + a;\n"
                             "    c := c + b;\n"
                             "    d := a * b / c;\n"
                             "    e := d;\n"
                             "    f := 3 * e;\n"
                             "    RETURN g * h * i * f;\n"
                             "    a := 42;\n"
                             "    b := 42;\n"
                             "    c := 42;\n"
                             "    d := 42;\n"
                             "    e := 42;\n"
                             "    f := 42\n"
                             "END.\n";
    SourceCodeManagement scm(code);
    Parser parser(scm);
    std::unique_ptr<NonTerminalParseTreeNode> parse_tree = parser.ParseFunctionDefinition();
    EXPECT_TRUE(parse_tree);
    SemanticAnalyzer semantic_analyzer;
    std::unique_ptr<FunctionAST> ast = semantic_analyzer.AnalyzeParseTree(std::move(parse_tree));
    ASSERT_TRUE(ast);
    // Optimization Pass: Dead Code Elimination.
    OptimizeDeadCode opc;
    opc.Optimize(*ast);
    testing::internal::CaptureStdout();
    ASTNodeVisitorDot visitor;
    visitor.Visit(*ast);
    const std::string expected = "digraph {\n"
                                 "0 [label=\"Function\"];\n"
                                 "0 -> 1\n"
                                 "1 [label=\"Assignment \"];\n"
                                 "1 -> 2\n"
                                 "2 [label=\" Identifier:a \"];\n"
                                 "1 -> 3\n"
                                 "3 [label=\" Binary Operator: +\"];\n"
                                 "3 -> 4\n"
                                 "4 [label=\" Identifier:a \"];\n"
                                 "3 -> 5\n"
                                 "5 [label=\" Literal:1 \"];\n"
                                 "0 -> 6\n"
                                 "6 [label=\"Assignment \"];\n"
                                 "6 -> 7\n"
                                 "7 [label=\" Identifier:b \"];\n"
                                 "6 -> 8\n"
                                 "8 [label=\" Binary Operator: +\"];\n"
                                 "8 -> 9\n"
                                 "9 [label=\" Identifier:b \"];\n"
                                 "8 -> 10\n"
                                 "10 [label=\" Identifier:a \"];\n"
                                 "0 -> 11\n"
                                 "11 [label=\"Assignment \"];\n"
                                 "11 -> 12\n"
                                 "12 [label=\" Identifier:c \"];\n"
                                 "11 -> 13\n"
                                 "13 [label=\" Binary Operator: +\"];\n"
                                 "13 -> 14\n"
                                 "14 [label=\" Identifier:c \"];\n"
                                 "13 -> 15\n"
                                 "15 [label=\" Identifier:b \"];\n"
                                 "0 -> 16\n"
                                 "16 [label=\"Assignment \"];\n"
                                 "16 -> 17\n"
                                 "17 [label=\" Identifier:d \"];\n"
                                 "16 -> 18\n"
                                 "18 [label=\" Binary Operator: *\"];\n"
                                 "18 -> 19\n"
                                 "19 [label=\" Identifier:a \"];\n"
                                 "18 -> 20\n"
                                 "20 [label=\" Binary Operator: /\"];\n"
                                 "20 -> 21\n"
                                 "21 [label=\" Identifier:b \"];\n"
                                 "20 -> 22\n"
                                 "22 [label=\" Identifier:c \"];\n"
                                 "0 -> 23\n"
                                 "23 [label=\"Assignment \"];\n"
                                 "23 -> 24\n"
                                 "24 [label=\" Identifier:e \"];\n"
                                 "23 -> 25\n"
                                 "25 [label=\" Identifier:d \"];\n"
                                 "0 -> 26\n"
                                 "26 [label=\"Assignment \"];\n"
                                 "26 -> 27\n"
                                 "27 [label=\" Identifier:f \"];\n"
                                 "26 -> 28\n"
                                 "28 [label=\" Binary Operator: *\"];\n"
                                 "28 -> 29\n"
                                 "29 [label=\" Literal:3 \"];\n"
                                 "28 -> 30\n"
                                 "30 [label=\" Identifier:e \"];\n"
                                 "0 -> 31\n"
                                 "31 [label=\"Return Statement\"];\n"
                                 "31 -> 32\n"
                                 "32 [label=\" Binary Operator: *\"];\n"
                                 "32 -> 33\n"
                                 "33 [label=\" Identifier:g \"];\n"
                                 "32 -> 34\n"
                                 "34 [label=\" Binary Operator: *\"];\n"
                                 "34 -> 35\n"
                                 "35 [label=\" Identifier:h \"];\n"
                                 "34 -> 36\n"
                                 "36 [label=\" Binary Operator: *\"];\n"
                                 "36 -> 37\n"
                                 "37 [label=\" Identifier:i \"];\n"
                                 "36 -> 38\n"
                                 "38 [label=\" Identifier:f \"];\n"
                                 "}\n";
    EXPECT_EQ(testing::internal::GetCapturedStdout(), expected);
}
//---------------------------------------------------------------------------
} // namespace pljit
//---------------------------------------------------------------------------
