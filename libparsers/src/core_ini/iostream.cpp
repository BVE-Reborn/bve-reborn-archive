#include "ini.hpp"
#include <ostream>

namespace parsers {
namespace ini {
	std::ostream& operator<<(std::ostream& os, const parsed_ini_object& rhs) {
		for (auto& section : rhs) {
			os << "Line: " << section.line << " Section: \"" << section.name << "\"\n";
			for (auto& value : section.values) {
				os << "Line: " << value.line << " \t\t\"" << value.value << "\"\n";
			}
			for (auto& key_value_pair : section.key_value_pairs) {
				os << "Line: " << key_value_pair.line << " \t\t\"" << key_value_pair.key
				   << "\" = \"" << key_value_pair.value << "\"\n";
			}
		}

		return os;
	}
} // namespace ini
} // namespace parsers
