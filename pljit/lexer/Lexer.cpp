//---------------------------------------------------------------------------
#include "util/Defer.hpp"
#include "lexer/Lexer.hpp"
#include <cassert>
#include <cctype>
//---------------------------------------------------------------------------
namespace pljit {
//---------------------------------------------------------------------------
Lexer::Lexer(const SourceCodeManagement& source_code_management) : current_line_number(0), current_line_offset(0), source_code_management(source_code_management) {}
//---------------------------------------------------------------------------
Token Lexer::ProduceNextToken() {
    const size_t number_of_lines = source_code_management.GetNumberOfLines();
    size_t current_line_size = source_code_management.GetLine(current_line_number).size();
    while (current_line_number < number_of_lines) {
        assert(current_line_number < number_of_lines && current_line_offset < current_line_size && "This offset + line is not valid!");
        // Process from this offset of the current line.
        while (current_line_offset < current_line_size) {
            size_t local_offset = current_line_offset;
            Defer current_line_offset_update([&]() {
                assert(current_line_offset < local_offset);
                current_line_offset = local_offset;
            });
            const char c = source_code_management.GetCharacter(current_line_number, local_offset++);
            assert(local_offset == current_line_offset + 1 && "local offset is advanced to the next char!");
            if (std::isspace(static_cast<unsigned char>(c)) == 0 /* if this char is not ANY space */) {
                // A non-space char is found. Check GREEDILY if it can be a token.
                if (c == '+') {
                    return {Token::Type::PLUS, SourceCodeReference(current_line_number, current_line_offset, 1, source_code_management)};
                } else if (c == '-') {
                    return {Token::Type::MINUS, SourceCodeReference(current_line_number, current_line_offset, 1, source_code_management)};
                } else if (c == '*') {
                    return {Token::Type::MUL, SourceCodeReference(current_line_number, current_line_offset, 1, source_code_management)};
                } else if (c == '/') {
                    return {Token::Type::DIV, SourceCodeReference(current_line_number, current_line_offset, 1, source_code_management)};
                } else if (c == '(') {
                    return {Token::Type::LEFT_PARENTHESE, SourceCodeReference(current_line_number, current_line_offset, 1, source_code_management)};
                } else if (c == ')') {
                    return {Token::Type::RIGHT_PARENTHESE, SourceCodeReference(current_line_number, current_line_offset, 1, source_code_management)};
                } else if (c == '=') {
                    return {Token::Type::EQUAL, SourceCodeReference(current_line_number, current_line_offset, 1, source_code_management)};
                } else if (c == '.') {
                    return {Token::Type::DOT, SourceCodeReference(current_line_number, current_line_offset, 1, source_code_management)};
                } else if (c == ',') {
                    return {Token::Type::COMMA, SourceCodeReference(current_line_number, current_line_offset, 1, source_code_management)};
                } else if (c == ';') {
                    return {Token::Type::SEMICOLON, SourceCodeReference(current_line_number, current_line_offset, 1, source_code_management)};
                } else if (c == ':') {
                    if (current_line_offset + 1 >= current_line_size /* this line is finished */ || source_code_management.GetCharacter(current_line_number, local_offset++) != '=' /* the next char is not '=' */) {
                        Error(current_line_number, current_line_offset);
                    }
                    assert(local_offset == current_line_offset + 2);
                    return {Token::Type::ASSIGNMENT, SourceCodeReference(current_line_number, current_line_offset, 2, source_code_management)};
                } else if (std::isdigit(static_cast<unsigned char>(c))) {
                    // Check if it contains only digits, if so then is LITERAL.
                    size_t variable_length = 1;
                    // In the current line to collect digits. Until the current line finished OR a non-digit is found.
                    while (local_offset < current_line_size && std::isdigit(static_cast<unsigned char>(source_code_management.GetCharacter(current_line_number, local_offset)))) {
                        local_offset++;
                        variable_length++;
                    }
                    assert(local_offset == current_line_offset + variable_length);
                    return {Token::Type::LITERAL, SourceCodeReference(current_line_number, current_line_offset, variable_length, source_code_management)};
                } else if (std::isalpha(static_cast<unsigned char>(c))) {
                    // Check if coming chars can match any keyword or identifier.
                    if (current_line_offset + 5 < current_line_size && source_code_management.GetLine(current_line_number).substr(current_line_offset, 5) == "PARAM") {
                        if (current_line_offset + 5 + 1 < current_line_size && std::isalpha(source_code_management.GetCharacter(current_line_number, current_line_offset + 5))) { goto IDENTIFIER; }  // Still not keyword.
                        local_offset = current_line_offset + 5;
                        return {Token::Type::PARAM, SourceCodeReference(current_line_number, current_line_offset, 5, source_code_management)};
                    } else if (current_line_offset + 3 < current_line_size && source_code_management.GetLine(current_line_number).substr(current_line_offset, 3) == "VAR") {
                        if (current_line_offset + 3 + 1 < current_line_size && std::isalpha(source_code_management.GetCharacter(current_line_number, current_line_offset + 3))) { goto IDENTIFIER; }  // Still not keyword.
                        local_offset = current_line_offset + 3;
                        return {Token::Type::VAR, SourceCodeReference(current_line_number, current_line_offset, 3, source_code_management)};
                    } else if (current_line_offset + 5 < current_line_size && source_code_management.GetLine(current_line_number).substr(current_line_offset, 5) == "CONST") {
                        if (current_line_offset + 5 + 1 < current_line_size && std::isalpha(source_code_management.GetCharacter(current_line_number, current_line_offset + 5))) { goto IDENTIFIER; }  // Still not keyword.
                        local_offset = current_line_offset + 5;
                        return {Token::Type::CONST, SourceCodeReference(current_line_number, current_line_offset, 5, source_code_management)};
                    } else if (current_line_offset + 5 < current_line_size && source_code_management.GetLine(current_line_number).substr(current_line_offset, 5) == "BEGIN") {
                        if (current_line_offset + 5 + 1 < current_line_size && std::isalpha(source_code_management.GetCharacter(current_line_number, current_line_offset + 5))) { goto IDENTIFIER; }  // Still not keyword.
                        local_offset = current_line_offset + 5;
                        return {Token::Type::BEGIN, SourceCodeReference(current_line_number, current_line_offset, 5, source_code_management)};
                    } else if (current_line_offset + 3 < current_line_size && source_code_management.GetLine(current_line_number).substr(current_line_offset, 3) == "END") {
                        if (current_line_offset + 3 + 1 < current_line_size && std::isalpha(source_code_management.GetCharacter(current_line_number, current_line_offset + 3))) { goto IDENTIFIER; }  // Still not keyword.
                        local_offset = current_line_offset + 3;
                        return {Token::Type::END, SourceCodeReference(current_line_number, current_line_offset, 3, source_code_management)};
                    } else if (current_line_offset + 6 < current_line_size && source_code_management.GetLine(current_line_number).substr(current_line_offset, 6) == "RETURN") {
                        if (current_line_offset + 6 + 1 < current_line_size && std::isalpha(source_code_management.GetCharacter(current_line_number, current_line_offset + 6))) { goto IDENTIFIER; }  // Still not keyword.
                        local_offset = current_line_offset + 6;
                        return {Token::Type::RETURN, SourceCodeReference(current_line_number, current_line_offset, 6, source_code_management)};
                    }

                    IDENTIFIER:
                    // Check if it contains only chars, if so then is IDENTIFIER.
                    size_t variable_length = 1;
                    // In the current line to collect chars. Until the current line finished OR a non-digit is found.
                    while (local_offset < current_line_size && std::isalpha(static_cast<unsigned char>(source_code_management.GetCharacter(current_line_number, local_offset)))) {
                        variable_length++;
                        local_offset++;
                    }
                    return {Token::Type::IDENTIFIER, SourceCodeReference(current_line_number, current_line_offset, variable_length, source_code_management)};
                } else {
                    Error(current_line_number, current_line_offset);
                    return {Token::Type::ERROR, SourceCodeReference(current_line_number, current_line_offset, 1, source_code_management)};
                }
            }
        }

        // The current line is done. Process the next line or finish the code.
        if (++current_line_number == number_of_lines) {
            // All lines of code processed, thus the lex phase is done.
            // -1: go back to valid position.
            return {Token::Type::EOT, SourceCodeReference(current_line_number - 1, current_line_offset - 1, 1, source_code_management)};
        }
        // Start to process next line and reset the offset within the line.
        current_line_offset = 0;
        current_line_size = source_code_management.GetLine(current_line_number).size();
    }
    // Should return in the loop and should not reach here.
    __builtin_unreachable();
}
//---------------------------------------------------------------------------
const SourceCodeManagement& Lexer::GetSourceCodeManagement() const { return source_code_management; }
//---------------------------------------------------------------------------
void Lexer::Error(size_t line_number, size_t line_offset) const {
    SourceCodeReference source(line_number, line_offset, 1, source_code_management);
    source.PrintContext("Unexpected Character");
}
//---------------------------------------------------------------------------
} // namespace pljit
//---------------------------------------------------------------------------
