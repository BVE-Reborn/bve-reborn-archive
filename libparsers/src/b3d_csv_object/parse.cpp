#include "b3d_csv_object.hpp"
#include "parsers/b3d_csv.hpp"
#include "utils.hpp"

namespace parsers {
namespace b3d_csv_object {
	parsed_b3d_csv_object_t parse_b3d(std::string& filename) {
		auto content = parsers::util::load_from_file_utf8_bom(filename);
		auto inst =
		    parsers::b3d_csv_object::create_instructions(content,
		                                                 parsers::b3d_csv_object::file_type::b3d);
		auto object = parsers::b3d_csv_object::run_csv_instructions(inst);
		return object;
	}

	parsed_b3d_csv_object_t parse_csv(std::string& filename) {
		auto content = parsers::util::load_from_file_utf8_bom(filename);
		auto inst =
		    parsers::b3d_csv_object::create_instructions(content,
		                                                 parsers::b3d_csv_object::file_type::csv);
		auto object = parsers::b3d_csv_object::run_csv_instructions(inst);
		return object;
	}
} // namespace b3d_csv_object
} // namespace parsers
