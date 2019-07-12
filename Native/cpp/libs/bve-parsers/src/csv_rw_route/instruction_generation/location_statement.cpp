#include "instruction_generator.hpp"

namespace bve::parsers::csv_rw_route::instruction_generation {
	Instruction create_instruction_location_statement(const line_splitting::InstructionInfo& inst) {
		args_at_least(inst, 1, "Location Statement");

		instructions::naked::Position pos;
		pos.distances.reserve(inst.args.size() + 1);
		pos.distances.emplace_back(inst.offset);
		std::transform(inst.args.begin(), inst.args.end(), std::back_inserter(pos.distances),
		               [](const std::string& s) { return util::parsers::parse_loose_float(s, 0); });

		return pos;
	}
} // namespace bve::parsers::csv_rw_route::instruction_generation
