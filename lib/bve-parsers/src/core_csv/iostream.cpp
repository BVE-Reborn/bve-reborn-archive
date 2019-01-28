#include "parsers/csv.hpp"
#include <ostream>

namespace bve::parsers::csv {
	std::ostream& operator<<(std::ostream& os, const CSVToken& rhs) {
		os << "(\"" << rhs.text << "\", " << rhs.line_begin << ", " << rhs.line_end << ", "
		   << rhs.char_begin << ", " << rhs.char_end << ')';
		return os;
	}

	std::ostream& operator<<(std::ostream& os, const ParsedCSV& rhs) {
		for (auto& row : rhs) {
			for (std::size_t i = 0; i < row.size(); ++i) {
				os << row[i];
				if (i != row.size() - 1) {
					os << ", ";
				}
			}
			os << '\n';
		}
		return os;
	}
} // namespace bve::parsers::csv
