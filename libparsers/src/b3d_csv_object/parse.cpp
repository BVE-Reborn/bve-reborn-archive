#include "b3d_csv_object.hpp"
#include "parsers/b3d_csv.hpp"
#include "utils.hpp"

namespace parsers {
// ReSharper disable once CppInconsistentNaming
namespace b3d_csv_object {
	// ReSharper disable once CppInconsistentNaming
	parsed_b3d_csv_object_t parse_b3d(std::string& filename) {
		auto const content = util::load_from_file_utf8_bom(filename);
		auto const inst = create_instructions(content, file_type::b3d);
		auto object = run_csv_instructions(inst);
		return object;
	}

	parsed_b3d_csv_object_t parse_csv(std::string& filename) {
		auto const content = util::load_from_file_utf8_bom(filename);
		auto const inst = create_instructions(content, file_type::csv);
		auto object = run_csv_instructions(inst);
		return object;
	}
} // namespace b3d_csv_object
} // namespace parsers
