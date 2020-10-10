#include "parser/Parser.hpp"
#include "ast/SemanticAnalyzer.hpp"
#include "optimization/EvaluationContext.hpp"
#include <gtest/gtest.h>
//---------------------------------------------------------------------------
namespace pljit {
//---------------------------------------------------------------------------
TEST(Optimization, Evaluation0) {
    const std::string code = "BEGIN\n"
                             "    RETURN 12 * (8 - 5)\n"
                             "END.\n";
    SourceCodeManagement scm(code);
    Parser parser(scm);
    std::unique_ptr<NonTerminalParseTreeNode> parse_tree = parser.ParseFunctionDefinition();
    ASSERT_TRUE(parse_tree);
    SemanticAnalyzer semantic_analyzer;
    std::unique_ptr<FunctionAST> ast = semantic_analyzer.AnalyzeParseTree(std::move(parse_tree));
    ASSERT_TRUE(ast);
    EvaluationContext ec(semantic_analyzer.GetSymbolTable());
    EXPECT_EQ(ast->Evaluate(ec), 12 * (8 - 5));
}
//---------------------------------------------------------------------------
TEST(Optimization, Evaluation1) {
    const std::string code = "BEGIN\n"
                             "    RETURN +42 + -42\n"
                             "END.\n";
    SourceCodeManagement scm(code);
    Parser parser(scm);
    std::unique_ptr<NonTerminalParseTreeNode> parse_tree = parser.ParseFunctionDefinition();
    ASSERT_TRUE(parse_tree);
    SemanticAnalyzer semantic_analyzer;
    std::unique_ptr<FunctionAST> ast = semantic_analyzer.AnalyzeParseTree(std::move(parse_tree));
    ASSERT_TRUE(ast);
    EvaluationContext ec(semantic_analyzer.GetSymbolTable());
    EXPECT_EQ(ast->Evaluate(ec), +42 + -42);
}
//---------------------------------------------------------------------------
TEST(Optimization, Evaluation2) {
    const std::string code = "PARAM pa, pb;\n"
                             "VAR va, vb;\n"
                             "CONST ca=1, cb=2;\n"
                             "BEGIN\n"
                             "    RETURN ca + cb\n"
                             "END.\n";
    SourceCodeManagement scm(code);
    Parser parser(scm);
    std::unique_ptr<NonTerminalParseTreeNode> parse_tree = parser.ParseFunctionDefinition();
    ASSERT_TRUE(parse_tree);
    SemanticAnalyzer semantic_analyzer;
    std::unique_ptr<FunctionAST> ast = semantic_analyzer.AnalyzeParseTree(std::move(parse_tree));
    ASSERT_TRUE(ast);
    EvaluationContext ec(semantic_analyzer.GetSymbolTable());
    EXPECT_EQ(ast->Evaluate(ec), 1 + 2);
}
//---------------------------------------------------------------------------
TEST(Optimization, Evaluation3) {
    const std::string code = "PARAM width, height;\n"
                             "VAR temp, foo;\n"
                             "CONST hello = 12, test = 2000;\n"
                             "BEGIN\n"
                             "    RETURN hello * (8 - 5)\n"
                             "END.\n";
    SourceCodeManagement scm(code);
    Parser parser(scm);
    std::unique_ptr<NonTerminalParseTreeNode> parse_tree = parser.ParseFunctionDefinition();
    ASSERT_TRUE(parse_tree);
    SemanticAnalyzer semantic_analyzer;
    std::unique_ptr<FunctionAST> ast = semantic_analyzer.AnalyzeParseTree(std::move(parse_tree));
    ASSERT_TRUE(ast);
    EvaluationContext ec(semantic_analyzer.GetSymbolTable());
    EXPECT_EQ(ast->Evaluate(ec), 12 * (8 - 5));
}
//---------------------------------------------------------------------------
TEST(Optimization, Evaluation4) {
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
                             "    RETURN g * h * i\n"
                             "END.\n";
    SourceCodeManagement scm(code);
    Parser parser(scm);
    std::unique_ptr<NonTerminalParseTreeNode> parse_tree = parser.ParseFunctionDefinition();
    ASSERT_TRUE(parse_tree);
    SemanticAnalyzer semantic_analyzer;
    std::unique_ptr<FunctionAST> ast = semantic_analyzer.AnalyzeParseTree(std::move(parse_tree));
    ASSERT_TRUE(ast);
    EvaluationContext ec(semantic_analyzer.GetSymbolTable());
    EXPECT_EQ(ast->Evaluate(ec), 1 * 2 * 3);
}
//---------------------------------------------------------------------------
} // namespace pljit
//---------------------------------------------------------------------------
