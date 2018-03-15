#include "instruction_generator.hpp"

namespace parsers {
namespace csv_rw_route {
	namespace instruction_generation {
		instruction create_instruction_location_statement(
		    const line_splitting::instruction_info& inst) {
			args_at_least(inst, 1, "Location Statement");

			instructions::naked::position pos;
			pos.distances.reserve(inst.args.size() + 1);
			pos.distances.emplace_back(inst.offset);
			std::transform(inst.args.begin(), inst.args.end(), std::back_inserter(pos.distances),
			               [](const std::string& s) { return util::parse_loose_float(s, 0); });

			return pos;
		}
	} // namespace instruction_generation
} // namespace csv_rw_route
} // namespace parsers
