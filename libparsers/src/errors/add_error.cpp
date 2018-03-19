#include <parsers/errors.hpp>
#include <sstream>
#include <utility>

namespace parsers {
namespace errors {
	void add_error(errors_t& errors, std::intmax_t line, std::string msg) {
		errors::error_t err;
		err.line = line;
		err.error = std::move(msg);

		errors.emplace_back(std::move(err));
	}

	void add_error(errors_t& errors, std::intmax_t line, const std::ostringstream& msg) {
		errors::error_t err;
		err.line = line;
		err.error = msg.str();

		errors.emplace_back(std::move(err));
	}

	void add_error(multi_error_t& errors,
	               const std::string& filename,
	               std::intmax_t line,
	               std::string msg) {
		add_error(errors[filename], line, std::move(msg));
	}

	void add_error(multi_error_t& errors,
	               const std::string& filename,
	               std::intmax_t line,
	               const std::ostringstream& msg) {
		add_error(errors[filename], line, msg);
	}

} // namespace errors
} // namespace parsers
