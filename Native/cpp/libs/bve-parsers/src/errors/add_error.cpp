#include "parsers/errors.hpp"
#include <sstream>
#include <utility>

namespace bve::parsers::errors {
	void add_error(Errors& errors, std::intmax_t const line, std::string msg) {
		Error err;
		err.line = line;
		err.error = std::move(msg);

		errors.emplace_back(std::move(err));
	}

	void add_error(Errors& errors, std::intmax_t const line, const std::ostringstream& msg) {
		Error err;
		err.line = line;
		err.error = msg.str();

		errors.emplace_back(std::move(err));
	}

	void add_error(MultiError& errors, const std::string& filename, std::intmax_t const line, std::string msg) {
		add_error(errors[filename], line, std::move(msg));
	}

	void add_error(MultiError& errors, const std::string& filename, std::intmax_t const line, const std::ostringstream& msg) {
		add_error(errors[filename], line, msg);
	}

} // namespace bve::parsers::errors
