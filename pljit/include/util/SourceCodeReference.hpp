#pragma once
//---------------------------------------------------------------------------
#include "util/SourceCodeManagement.hpp"
#include <string_view>
//---------------------------------------------------------------------------
namespace pljit {
//---------------------------------------------------------------------------
/// A Reference Type to a location or range in the original source code
class SourceCodeReference {
    public:
    /// Constructor.
    SourceCodeReference(size_t line_number, size_t line_offset, size_t length, const SourceCodeManagement& source_code_management);
    /// Print context for a location or range reference.
    void PrintContext(std::string_view context) const;
    /// Get the specified location of source code.
    [[nodiscard]] std::string_view GetSourceCode() const;
    /// Get the line number.
    [[nodiscard]] size_t GetLineNumber() const;
    /// Get the offset of a char.
    [[nodiscard]] size_t GetCharOffset() const;
    /// Get the length.
    [[nodiscard]] size_t GetLength() const;
    /// Check if is a location.
    [[nodiscard]] size_t IfLocation() const;
    /// Check if is a range.
    [[nodiscard]] size_t IfRange() const;

    private:
    const size_t line_number;
    const size_t line_offset;
    const size_t length;
    const SourceCodeManagement& source_code_management;
};
//---------------------------------------------------------------------------
} // namespace pljit
//---------------------------------------------------------------------------
