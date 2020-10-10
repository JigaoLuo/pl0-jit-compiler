//---------------------------------------------------------------------------
#include "util/SourceCodeManagement.hpp"
#include <cassert>
#include <sstream>
#include <string>
//---------------------------------------------------------------------------
namespace pljit {
//---------------------------------------------------------------------------
SourceCodeManagement::SourceCodeManagement(const std::string& code) {
    std::istringstream ss(code);
    std::string line;
    while (std::getline(ss, line)) {
        loc.emplace_back(line + "\n");
    }
}
//---------------------------------------------------------------------------
std::string_view SourceCodeManagement::GetLine(size_t line_number) const {
    assert(loc.size() > line_number && "Invalid Access: Index Overflow!");
    return {loc[line_number]};
}
//---------------------------------------------------------------------------
char SourceCodeManagement::GetCharacter(size_t line_number, size_t char_offset) const {
    assert((loc.size() > line_number || loc[line_number].size() > char_offset) && "Invalid Access: Index Overflow!");
    return loc[line_number][char_offset];
}
//---------------------------------------------------------------------------
size_t SourceCodeManagement::GetNumberOfLines() const { return loc.size(); }
//---------------------------------------------------------------------------
size_t SourceCodeManagement::GetLineLength(size_t line_number) const {
    assert(loc.size() > line_number && "Invalid Access: Index Overflow!");
    return loc[line_number].size();
}
//---------------------------------------------------------------------------
} // namespace pljit
//---------------------------------------------------------------------------
