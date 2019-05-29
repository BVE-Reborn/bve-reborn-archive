#include "parsers/errors.hpp"
#include <algorithm>
#include <ostream>

namespace bve::parsers::errors {
	std::ostream& operator<<(std::ostream& os, Error& e) {
		os << e.line << ": " << e.error;
		return os;
	}

	std::ostream& operator<<(std::ostream& os, Errors& es) {
		std::stable_sort(es.begin(), es.end(), [](const Error& a, const Error& b) { return a.line < b.line; });
		for (auto& e : es) {
			os << e << '\n';
		}
		return os;
	}

	std::ostream& operator<<(std::ostream& os, MultiError& me) {
		for (auto& tuple : me) {
			os << tuple.first << ":\n";
			std::stable_sort(tuple.second.begin(), tuple.second.end(), [](const Error& a, const Error& b) { return a.line < b.line; });
			for (auto& e : tuple.second) {
				os << '\t' << e << '\n';
			}
		}
		return os;
	}
} // namespace bve::parsers::errors
