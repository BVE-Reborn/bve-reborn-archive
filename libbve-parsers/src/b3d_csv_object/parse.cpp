#include "b3d_csv_object.hpp"
#include "parsers/b3d_csv.hpp"
#include "utils.hpp"

namespace bve::parsers::b3d_csv_object {
	parsed_b3d_csv_object_t parse_b3d(std::string& file_contents) {
		auto const inst = create_instructions(file_contents, file_type::b3d);
		auto object = run_csv_instructions(inst);
		return object;
	}

	parsed_b3d_csv_object_t parse_csv(std::string& file_contents) {
		auto const inst = create_instructions(file_contents, file_type::csv);
		auto object = run_csv_instructions(inst);
		return object;
	}
} // namespace bve::parsers::b3d_csv_object
