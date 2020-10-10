#include "parser/Parser.hpp"
#include "ast/SemanticAnalyzer.hpp"
#include "ast/ASTNodeVisitorDot.hpp"
#include "optimization/ConstantPropagation.hpp"
#include <gtest/gtest.h>
//---------------------------------------------------------------------------
namespace pljit {
//---------------------------------------------------------------------------
TEST(Optimization, ConstantPropagation0) {
    {
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
        // Optimization Pass: Constant Propagation.
        ConstantPropagation cp(semantic_analyzer.GetSymbolTable());
        cp.Optimize(*ast);
        testing::internal::CaptureStdout();
        ASTNodeVisitorDot visitor;
        visitor.Visit(*ast);
    }
    const std::string optimized = testing::internal::GetCapturedStdout();
    {
        const std::string code = "BEGIN\n"
                                 "    RETURN 36\n"
                                 "END.\n";
        SourceCodeManagement scm(code);
        Parser parser(scm);
        std::unique_ptr<NonTerminalParseTreeNode> parse_tree = parser.ParseFunctionDefinition();
        ASSERT_TRUE(parse_tree);
        SemanticAnalyzer semantic_analyzer;
        std::unique_ptr<FunctionAST> ast = semantic_analyzer.AnalyzeParseTree(std::move(parse_tree));
        ASSERT_TRUE(ast);
        testing::internal::CaptureStdout();
        ASTNodeVisitorDot visitor;
        visitor.Visit(*ast);
    }
    const std::string mock = testing::internal::GetCapturedStdout();
    EXPECT_EQ(mock, optimized);
}
//---------------------------------------------------------------------------
TEST(Optimization, ConstantPropagation1) {
    {
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
        // Optimization Pass: Constant Propagation.
        ConstantPropagation cp(semantic_analyzer.GetSymbolTable());
        cp.Optimize(*ast);
        testing::internal::CaptureStdout();
        ASTNodeVisitorDot visitor;
        visitor.Visit(*ast);
    }
    const std::string optimized = testing::internal::GetCapturedStdout();
    {
        const std::string code = "BEGIN\n"
                                 "    RETURN 0\n"
                                 "END.\n";
        SourceCodeManagement scm(code);
        Parser parser(scm);
        std::unique_ptr<NonTerminalParseTreeNode> parse_tree = parser.ParseFunctionDefinition();
        ASSERT_TRUE(parse_tree);
        SemanticAnalyzer semantic_analyzer;
        std::unique_ptr<FunctionAST> ast = semantic_analyzer.AnalyzeParseTree(std::move(parse_tree));
        ASSERT_TRUE(ast);
        testing::internal::CaptureStdout();
        ASTNodeVisitorDot visitor;
        visitor.Visit(*ast);
    }
    const std::string mock = testing::internal::GetCapturedStdout();
    EXPECT_EQ(mock, optimized);
}
//---------------------------------------------------------------------------
TEST(Optimization, ConstantPropagation2) {
    {
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
        // Optimization Pass: Constant Propagation.
        ConstantPropagation cp(semantic_analyzer.GetSymbolTable());
        cp.Optimize(*ast);
        testing::internal::CaptureStdout();
        ASTNodeVisitorDot visitor;
        visitor.Visit(*ast);
    }
    const std::string optimized = testing::internal::GetCapturedStdout();
    {
        const std::string code = "PARAM pa, pb;\n"
                                 "VAR va, vb;\n"
                                 "CONST ca=1, cb=2;\n"
                                 "BEGIN\n"
                                 "    RETURN 3\n"
                                 "END.\n";
        SourceCodeManagement scm(code);
        Parser parser(scm);
        std::unique_ptr<NonTerminalParseTreeNode> parse_tree = parser.ParseFunctionDefinition();
        ASSERT_TRUE(parse_tree);
        SemanticAnalyzer semantic_analyzer;
        std::unique_ptr<FunctionAST> ast = semantic_analyzer.AnalyzeParseTree(std::move(parse_tree));
        ASSERT_TRUE(ast);
        testing::internal::CaptureStdout();
        ASTNodeVisitorDot visitor;
        visitor.Visit(*ast);
    }
    const std::string mock = testing::internal::GetCapturedStdout();
    EXPECT_EQ(mock, optimized);
}
//---------------------------------------------------------------------------
TEST(Optimization, ConstantPropagation3) {
    {
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
        // Optimization Pass: Constant Propagation.
        ConstantPropagation cp(semantic_analyzer.GetSymbolTable());
        cp.Optimize(*ast);
        testing::internal::CaptureStdout();
        ASTNodeVisitorDot visitor;
        visitor.Visit(*ast);
    }
    const std::string optimized = testing::internal::GetCapturedStdout();
    {
        const std::string code = "PARAM width, height;\n"
                                 "VAR temp, foo;\n"
                                 "CONST hello = 12, test = 2000;\n"
                                 "BEGIN\n"
                                 "    RETURN 36\n"
                                 "END.\n";
        SourceCodeManagement scm(code);
        Parser parser(scm);
        std::unique_ptr<NonTerminalParseTreeNode> parse_tree = parser.ParseFunctionDefinition();
        ASSERT_TRUE(parse_tree);
        SemanticAnalyzer semantic_analyzer;
        std::unique_ptr<FunctionAST> ast = semantic_analyzer.AnalyzeParseTree(std::move(parse_tree));
        ASSERT_TRUE(ast);
        testing::internal::CaptureStdout();
        ASTNodeVisitorDot visitor;
        visitor.Visit(*ast);
    }
    const std::string mock = testing::internal::GetCapturedStdout();
    EXPECT_EQ(mock, optimized);
}
//---------------------------------------------------------------------------
TEST(Optimization, ConstantPropagation4) {
    {
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
        // Optimization Pass: Constant Propagation.
        ConstantPropagation cp(semantic_analyzer.GetSymbolTable());
        cp.Optimize(*ast);
        testing::internal::CaptureStdout();
        ASTNodeVisitorDot visitor;
        visitor.Visit(*ast);
    }
    const std::string optimized = testing::internal::GetCapturedStdout();
    {
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
                                 "    RETURN 6\n"
                                 "END.\n";
        SourceCodeManagement scm(code);
        Parser parser(scm);
        std::unique_ptr<NonTerminalParseTreeNode> parse_tree = parser.ParseFunctionDefinition();
        ASSERT_TRUE(parse_tree);
        SemanticAnalyzer semantic_analyzer;
        std::unique_ptr<FunctionAST> ast = semantic_analyzer.AnalyzeParseTree(std::move(parse_tree));
        ASSERT_TRUE(ast);
        testing::internal::CaptureStdout();
        ASTNodeVisitorDot visitor;
        visitor.Visit(*ast);
    }
    const std::string mock = testing::internal::GetCapturedStdout();
    EXPECT_EQ(mock, optimized);
}
//---------------------------------------------------------------------------
TEST(Optimization, ConstantPropagation5) {
    {
        const std::string code = "PARAM a;\n"
                                 "VAR b, c, d;\n"
                                 "CONST e = 1;\n"
                                 "BEGIN\n"
                                 "    b := 1 + 2;\n"
                                 "    c := b + e;\n"
                                 "    d := b + c + e;\n"
                                 "    RETURN d + 1 * 2 - 2 / 1\n"
                                 "END.";
        SourceCodeManagement scm(code);
        Parser parser(scm);
        std::unique_ptr<NonTerminalParseTreeNode> parse_tree = parser.ParseFunctionDefinition();
        ASSERT_TRUE(parse_tree);
        SemanticAnalyzer semantic_analyzer;
        std::unique_ptr<FunctionAST> ast = semantic_analyzer.AnalyzeParseTree(std::move(parse_tree));
        ASSERT_TRUE(ast);
        // Optimization Pass: Constant Propagation.
        ConstantPropagation cp(semantic_analyzer.GetSymbolTable());
        cp.Optimize(*ast);
        testing::internal::CaptureStdout();
        ASTNodeVisitorDot visitor;
        visitor.Visit(*ast);
    }
    const std::string optimized = testing::internal::GetCapturedStdout();
    {
        const std::string code = "PARAM a;\n"
                                 "VAR b, c, d;\n"
                                 "CONST e = 1;\n"
                                 "BEGIN\n"
                                 "    b := 3;\n"
                                 "    c := 4;\n"
                                 "    d := 8;\n"
                                 "    RETURN 8\n"
                                 "END.";
        SourceCodeManagement scm(code);
        Parser parser(scm);
        std::unique_ptr<NonTerminalParseTreeNode> parse_tree = parser.ParseFunctionDefinition();
        ASSERT_TRUE(parse_tree);
        SemanticAnalyzer semantic_analyzer;
        std::unique_ptr<FunctionAST> ast = semantic_analyzer.AnalyzeParseTree(std::move(parse_tree));
        ASSERT_TRUE(ast);
        testing::internal::CaptureStdout();
        ASTNodeVisitorDot visitor;
        visitor.Visit(*ast);
    }
    const std::string mock = testing::internal::GetCapturedStdout();
    EXPECT_EQ(mock, optimized);
}
//---------------------------------------------------------------------------
} // namespace pljit
//---------------------------------------------------------------------------
