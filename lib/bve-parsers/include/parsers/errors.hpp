#pragma once

#include <iosfwd>
#include <map>
#include <string>
#include <vector>

namespace bve::parsers::errors {
	struct Error {
		std::intmax_t line;
		std::string error;
	};

	using Errors = std::vector<Error>;

	using MultiError = std::map<std::string, Errors>;

	void add_error(Errors& errors, std::intmax_t line, std::string msg);
	void add_error(Errors& errors, std::intmax_t line, const std::ostringstream& msg);
	void add_error(MultiError& errors,
	               const std::string& filename,
	               std::intmax_t line,
	               std::string msg);
	void add_error(MultiError& errors,
	               const std::string& filename,
	               std::intmax_t line,
	               const std::ostringstream& msg);

	std::ostream& operator<<(std::ostream& os, Error& /*e*/);
	std::ostream& operator<<(std::ostream& os, Errors& /*es*/);
	std::ostream& operator<<(std::ostream& os, MultiError& /*me*/);
} // namespace bve::parsers::errors
