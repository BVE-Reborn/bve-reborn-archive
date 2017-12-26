#include "csv.hpp"
#include <ostream>

namespace parsers {
namespace csv {
	std::ostream& operator<<(std::ostream& os, const csv_token& rhs) {
		os << "(\"" << rhs.text << "\", " << rhs.line_begin << ", " << rhs.line_end << ", " << rhs.char_begin << ", "
		   << rhs.char_end << "),";
		return os;
	}

	std::ostream& operator<<(std::ostream& os, const parsed_csv& rhs) {
		for (auto& row : rhs) {
			for (auto& elem : row) {
				os << elem;
			}
			os << '\n';
		}
		return os;
	}
} // namespace csv
} // namespace parsers
