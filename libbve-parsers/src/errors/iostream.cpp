#include "parsers/errors.hpp"
#include <algorithm>
#include <ostream>

namespace bve::parsers::errors {
	std::ostream& operator<<(std::ostream& os, error_t& e) {
		os << e.line << ": " << e.error;
		return os;
	}

	std::ostream& operator<<(std::ostream& os, errors_t& es) {
		std::stable_sort(es.begin(), es.end(),
		                 [](const error_t& a, const error_t& b) { return a.line < b.line; });
		for (auto& e : es) {
			os << e << '\n';
		}
		return os;
	}

	std::ostream& operator<<(std::ostream& os, multi_error_t& me) {
		for (auto& tuple : me) {
			os << tuple.first << ":\n";
			std::stable_sort(tuple.second.begin(), tuple.second.end(),
			                 [](const error_t& a, const error_t& b) { return a.line < b.line; });
			for (auto& e : tuple.second) {
				os << '\t' << e << '\n';
			}
		}
		return os;
	}
} // namespace bve::parsers::errors
