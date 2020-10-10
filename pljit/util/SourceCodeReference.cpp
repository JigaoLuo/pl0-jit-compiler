//---------------------------------------------------------------------------
#include "util/SourceCodeReference.hpp"
#include <iostream>
#include <sstream>
#include <cassert>
#include <string>
#include <string_view>
//---------------------------------------------------------------------------
namespace pljit {
//---------------------------------------------------------------------------
SourceCodeReference::SourceCodeReference(size_t line_number, size_t line_offset, size_t length, const SourceCodeManagement& source_code_management)
 : line_number(line_number), line_offset(line_offset), length(length), source_code_management(source_code_management) {
    assert(length > 0 && "Length at least more than 0");
    assert(line_number < source_code_management.GetNumberOfLines());
    assert(line_offset < source_code_management.GetLineLength(line_number));
}
//---------------------------------------------------------------------------
void SourceCodeReference::PrintContext(std::string_view context) const {
    std::stringstream ss;
    ss << line_number << ":" << line_offset << ": " << context << std::endl;
    ss << source_code_management.GetLine(line_number);
    ss << std::string(line_offset, ' ') << '^' << std::string(length - 1, '~') << std::endl;
    std::cerr << ss.str() << std::flush;
}
//---------------------------------------------------------------------------
std::string_view SourceCodeReference::GetSourceCode() const { return source_code_management.GetLine(line_number).substr(line_offset, length); }
//---------------------------------------------------------------------------
size_t SourceCodeReference::GetLineNumber() const { return line_number; }
//---------------------------------------------------------------------------
size_t SourceCodeReference::GetCharOffset() const { return line_offset; }
//---------------------------------------------------------------------------
size_t SourceCodeReference::GetLength() const { return length; }
//---------------------------------------------------------------------------
size_t SourceCodeReference::IfLocation() const { return length == 1; }
//---------------------------------------------------------------------------
size_t SourceCodeReference::IfRange() const { return length > 1; }
//---------------------------------------------------------------------------
} // namespace pljit
//---------------------------------------------------------------------------
