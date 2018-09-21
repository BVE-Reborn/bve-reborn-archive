#include "b3d_csv_object.hpp"
#include "parsers/b3d_csv.hpp"
#include "utils.hpp"

// ReSharper disable once CppInconsistentNaming
namespace bve::parsers::b3d_csv_object {
	// ReSharper disable once CppInconsistentNaming
	ParsedB3DCSVObject parse_b3d(std::string& file_contents) {
		auto const inst = create_instructions(file_contents, FileType::b3d);
		auto object = run_csv_instructions(inst);
		return object;
	}

	ParsedB3DCSVObject parse_csv(std::string& file_contents) {
		auto const inst = create_instructions(file_contents, FileType::csv);
		auto object = run_csv_instructions(inst);
		return object;
	}
} // namespace bve::parsers::b3d_csv_object
