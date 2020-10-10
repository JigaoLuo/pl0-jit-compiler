#include "lexer/Lexer.hpp"
#include <vector>
#include <gtest/gtest.h>
//---------------------------------------------------------------------------
using namespace pljit;
using T = pljit::Token::Type;
//---------------------------------------------------------------------------
/// Produce all possible tokens of the lexer. ONLY FOR TESTER.
std::vector<Token> ProduceTokens(Lexer& lexer) {
    std::vector<Token> tokens;
    while (true) {
        tokens.emplace_back(lexer.ProduceNextToken());

        // Once a error found, stop the lexer phase.
        if (tokens.back().GetType() == Token::Type::ERROR) {
            break;
        }
        // Once a EOT found, stop the lexer phase.
        if (tokens.back().GetType() == Token::Type::EOT) {
            tokens.pop_back();
            break;
        }
    }
    return tokens;
}
//---------------------------------------------------------------------------
TEST(Lexer, EdgeCase0) {
    const std::string code = "1234a";
    const SourceCodeManagement scm(code);
    Lexer lexer(scm);
    const std::vector<Token> tokens = ProduceTokens(lexer);
    const std::vector<T> expected_token_types{T::LITERAL /*1234*/, T::IDENTIFIER /*a*/};

    // Only Test the class Token's type.
    ASSERT_EQ(tokens.size(), expected_token_types.size());
    for (size_t i = 0; i < tokens.size(); i++) {
        EXPECT_EQ(tokens[i].GetType(), expected_token_types[i]);
    }
}
//---------------------------------------------------------------------------
TEST(Lexer, EdgeCase1) {
    const std::string code = "PARM";
    const SourceCodeManagement scm(code);
    Lexer lexer(scm);
    const std::vector<Token> tokens = ProduceTokens(lexer);
    const std::vector<T> expected_token_types{T::IDENTIFIER /*PARM*/};

    // Only Test the class Token's type.
    ASSERT_EQ(tokens.size(), expected_token_types.size());
    for (size_t i = 0; i < tokens.size(); i++) {
        EXPECT_EQ(tokens[i].GetType(), expected_token_types[i]);
    }
}
//---------------------------------------------------------------------------
TEST(Lexer, EdgeCase2) {
    const std::string code = "PARAMeter";
    const SourceCodeManagement scm(code);
    Lexer lexer(scm);
    const std::vector<Token> tokens = ProduceTokens(lexer);
    const std::vector<T> expected_token_types{T::IDENTIFIER /*PARAMeter*/};

    // Only Test the class Token's type.
    ASSERT_EQ(tokens.size(), expected_token_types.size());
    for (size_t i = 0; i < tokens.size(); i++) {
        EXPECT_EQ(tokens[i].GetType(), expected_token_types[i]);
    }
}
//---------------------------------------------------------------------------
TEST(Lexer, EdgeCase3) {
    const std::string code = "someidentPARAMifier";
    const SourceCodeManagement scm(code);
    Lexer lexer(scm);
    const std::vector<Token> tokens = ProduceTokens(lexer);
    const std::vector<T> expected_token_types{T::IDENTIFIER /*someidentPARAMifier*/};

    // Only Test the class Token's type.
    ASSERT_EQ(tokens.size(), expected_token_types.size());
    for (size_t i = 0; i < tokens.size(); i++) {
        EXPECT_EQ(tokens[i].GetType(), expected_token_types[i]);
    }
}
//---------------------------------------------------------------------------
TEST(Lexer, UnaryOperators) {
    // In the lexer phase, only take PLUS MINUS for unary as well as binary.
    const std::string code = "RETURN -2 * -a";
    SourceCodeManagement scm(code);
    Lexer lexer(scm);
    const std::vector<Token> tokens = ProduceTokens(lexer);
    const std::vector<T> expected_token_types{T::RETURN, T::MINUS, T::LITERAL, T::MUL, T::MINUS, T::IDENTIFIER};

    // Only Test the class Token's type.
    ASSERT_EQ(tokens.size(), expected_token_types.size());
    for (size_t i = 0; i < tokens.size(); i++) {
        EXPECT_EQ(tokens[i].GetType(), expected_token_types[i]);
    }
}
//---------------------------------------------------------------------------
TEST(Lexer, ParamConst) {
    const std::string code = "PARAM width, height, depth;\nCONST volume = 1, foo = 20;";
    const SourceCodeManagement scm(code);
    Lexer lexer(scm);
    const std::vector<Token> tokens = ProduceTokens(lexer);
    const std::vector<T> expected_token_types{T::PARAM, T::IDENTIFIER, T::COMMA, T::IDENTIFIER, T::COMMA, T::IDENTIFIER, T::SEMICOLON,
                                              T::CONST, T::IDENTIFIER, T::EQUAL, T::LITERAL, T::COMMA, T::IDENTIFIER, T::EQUAL, T::LITERAL, T::SEMICOLON};

    // Only Test the class Token's type.
    ASSERT_EQ(tokens.size(), expected_token_types.size());
    for (size_t i = 0; i < tokens.size(); i++) {
        EXPECT_EQ(tokens[i].GetType(), expected_token_types[i]);
    }
}
//---------------------------------------------------------------------------
TEST(Lexer, Const) {
    const std::string code = "CONST volume = 1, foo = 20;";
    const SourceCodeManagement scm(code);
    Lexer lexer(scm);
    const std::vector<Token> tokens = ProduceTokens(lexer);
    const std::vector<T> expected_token_types{T::CONST, T::IDENTIFIER, T::EQUAL, T::LITERAL, T::COMMA, T::IDENTIFIER, T::EQUAL, T::LITERAL, T::SEMICOLON};

    // Only Test the class Token's type.
    ASSERT_EQ(tokens.size(), expected_token_types.size());
    for (size_t i = 0; i < tokens.size(); i++) {
        EXPECT_EQ(tokens[i].GetType(), expected_token_types[i]);
    }
}
//---------------------------------------------------------------------------
TEST(Lexer, Assignment) {
    const std::string code = "foo := 200 * (10 + 12);";
    const SourceCodeManagement scm(code);
    Lexer lexer(scm);
    const std::vector<Token> tokens = ProduceTokens(lexer);
    const std::vector<T> expected_token_types{T::IDENTIFIER, T::ASSIGNMENT, T::LITERAL, T::MUL, T::LEFT_PARENTHESE, T::LITERAL, T::PLUS, T::LITERAL, T::RIGHT_PARENTHESE, T::SEMICOLON};

    // Only Test the class Token's type.
    ASSERT_EQ(tokens.size(), expected_token_types.size());
    for (size_t i = 0; i < tokens.size(); i++) {
        EXPECT_EQ(tokens[i].GetType(), expected_token_types[i]);
    }
}
//---------------------------------------------------------------------------
TEST(Lexer, ComplexAssignment) {
    const std::string code = "foo := a*1+1234a-12*3b;";
    const SourceCodeManagement scm(code);
    Lexer lexer(scm);
    const std::vector<Token> tokens = ProduceTokens(lexer);
    const std::vector<T> expected_token_types{T::IDENTIFIER, T::ASSIGNMENT, T::IDENTIFIER, T::MUL, T::LITERAL, T::PLUS, T::LITERAL, T::IDENTIFIER, T::MINUS, T::LITERAL, T::MUL, T::LITERAL, T::IDENTIFIER, T::SEMICOLON};

    // Only Test the class Token's type.
    ASSERT_EQ(tokens.size(), expected_token_types.size());
    for (size_t i = 0; i < tokens.size(); i++) {
        EXPECT_EQ(tokens[i].GetType(), expected_token_types[i]);
    }
}
//---------------------------------------------------------------------------
TEST(Lexer, ComplexAssignmentSpaced) {
    const std::string code = "foo  :=  a  *  1  +  1234  a  -  12  *  3  b  ;     ";
    const SourceCodeManagement scm(code);
    Lexer lexer(scm);
    const std::vector<Token> tokens = ProduceTokens(lexer);
    const std::vector<T> expected_token_types{T::IDENTIFIER, T::ASSIGNMENT, T::IDENTIFIER, T::MUL, T::LITERAL, T::PLUS, T::LITERAL, T::IDENTIFIER, T::MINUS, T::LITERAL, T::MUL, T::LITERAL, T::IDENTIFIER, T::SEMICOLON};

    // Only Test the class Token's type.
    ASSERT_EQ(tokens.size(), expected_token_types.size());
    for (size_t i = 0; i < tokens.size(); i++) {
        EXPECT_EQ(tokens[i].GetType(), expected_token_types[i]);
    }
}
//---------------------------------------------------------------------------
TEST(Lexer, Program1) {
    const std::string code = "PARAM width, height, depth;\nVAR volume;\nCONST foo = 12;";
    const SourceCodeManagement scm(code);
    Lexer lexer(scm);
    const std::vector<Token> tokens = ProduceTokens(lexer);
    const std::vector<T> expected_token_types{T::PARAM, T::IDENTIFIER, T::COMMA, T::IDENTIFIER, T::COMMA, T::IDENTIFIER, T::SEMICOLON,
                                              T::VAR, T::IDENTIFIER, T::SEMICOLON,
                                              T::CONST, T::IDENTIFIER, T::EQUAL, T::LITERAL, T::SEMICOLON};
    // Only Test the class Token's type.
    ASSERT_EQ(tokens.size(), expected_token_types.size());
    for (size_t i = 0; i < tokens.size(); i++) {
        EXPECT_EQ(tokens[i].GetType(), expected_token_types[i]);
    }
}
//---------------------------------------------------------------------------
TEST(Lexer, Program2) {
    const std::string code = "PARAM width;\n"
                             "CONST foo = 12;\n"
                             "BEGIN\n"
                             " foo := width;\n"
                             "RETURN foo\n"
                             "END.";
    const SourceCodeManagement scm(code);
    Lexer lexer(scm);
    const std::vector<Token> tokens = ProduceTokens(lexer);
    const std::vector<T> expected_token_types{T::PARAM, T::IDENTIFIER, T::SEMICOLON,
                                              T::CONST, T::IDENTIFIER, T::EQUAL, T::LITERAL, T::SEMICOLON,
                                              T::BEGIN,
                                              T::IDENTIFIER, T::ASSIGNMENT, T::IDENTIFIER, T::SEMICOLON,
                                              T::RETURN, T::IDENTIFIER,
                                              T::END, T::DOT};

    // Only Test the class Token's type.
    ASSERT_EQ(tokens.size(), expected_token_types.size());
    for (size_t i = 0; i < tokens.size(); i++) {
        EXPECT_EQ(tokens[i].GetType(), expected_token_types[i]);
    }
}
//---------------------------------------------------------------------------
TEST(Lexer, LexerError) {
    const std::string code = "?;\n";
    const SourceCodeManagement scm(code);
    Lexer lexer(scm);
    testing::internal::CaptureStderr();
    const std::vector<Token> tokens = ProduceTokens(lexer);
    const std::string lexer_error= testing::internal::GetCapturedStderr();
    const std::string expected_lexer_error = "0:0: Unexpected Character\n"
                                             "?;\n"
                                             "^\n";
    EXPECT_EQ(lexer_error, expected_lexer_error);
    EXPECT_EQ(tokens.back().GetType(), T::ERROR);
}
//---------------------------------------------------------------------------
TEST(Lexer, CorrectSources) {
    const std::string code = "PARAM width;\n"
                             "VAR temp;";
    const SourceCodeManagement scm(code);
    Lexer lexer(scm);
    const std::vector<Token> tokens = ProduceTokens(lexer);

    std::vector<SourceCodeReference> expected_scr{SourceCodeReference(0, 0, 5, scm),
                                                  SourceCodeReference(0, 6, 5, scm),
                                                  SourceCodeReference(0, 11, 1, scm),
                                                  SourceCodeReference(1, 0, 3, scm),
                                                  SourceCodeReference(1, 4, 4, scm),
                                                  SourceCodeReference(1, 8, 1, scm)};

    ASSERT_EQ(tokens.size(), expected_scr.size());
    for (size_t i = 0; i < tokens.size(); i++) {
        EXPECT_EQ(tokens[i].GetSourceCodeReference().GetSourceCode(), expected_scr[i].GetSourceCode());
        EXPECT_EQ(tokens[i].GetSourceCodeReference().GetLength(), expected_scr[i].GetLength());
        EXPECT_EQ(tokens[i].GetSourceCodeReference().GetCharOffset(), expected_scr[i].GetCharOffset());
        EXPECT_EQ(tokens[i].GetSourceCodeReference().GetLineNumber(), expected_scr[i].GetLineNumber());
        EXPECT_EQ(tokens[i].GetSourceCodeReference().IfLocation(), expected_scr[i].IfLocation());
        EXPECT_EQ(tokens[i].GetSourceCodeReference().IfRange(), expected_scr[i].IfRange());
    }
}
//---------------------------------------------------------------------------
