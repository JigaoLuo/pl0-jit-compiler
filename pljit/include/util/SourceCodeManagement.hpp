#pragma once
//---------------------------------------------------------------------------
#include <string>
#include <string_view>
#include <vector>
//---------------------------------------------------------------------------
namespace pljit {
//---------------------------------------------------------------------------
/// The centralized Source Code Management.
class SourceCodeManagement {
    public:
    /// Constructor.
    explicit SourceCodeManagement(const std::string& code);
    /// Copy constructor.
    SourceCodeManagement(const SourceCodeManagement&) = default;
    /// Get a line of code.
    [[nodiscard]] std::string_view GetLine(size_t line_number) const;
    /// Get a character from code.
    [[nodiscard]] char GetCharacter(size_t line_number, size_t char_offset) const;
    /// Get the number of code lines.
    [[nodiscard]] size_t GetNumberOfLines() const;
    /// Get the length of a line.
    [[nodiscard]] size_t GetLineLength(size_t line_number) const;

    private:
    /// Lines of code in vector.
    std::vector<std::string> loc;
};
//---------------------------------------------------------------------------
} // namespace pljit
//---------------------------------------------------------------------------