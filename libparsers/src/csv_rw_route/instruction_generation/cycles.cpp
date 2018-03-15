#include "instruction_generator.hpp"

namespace parsers {
namespace csv_rw_route {
	namespace instruction_generation {
		instruction create_instruction_cycle_ground(const line_splitting::instruction_info& inst) {
			indices_at_least(inst, 1, "Cycle.Ground");
			args_at_least(inst, 1, "Cycle.Ground");

			instructions::cycle::Ground g;

			g.cycle_structure_index = std::size_t(util::parse_loose_integer(inst.indices[0]));

			g.input_indices.reserve(inst.args.size());
			std::transform(inst.args.begin(), inst.args.end(), std::back_inserter(g.input_indices),
			               [](const std::string& arg) {
				               return std::size_t(util::parse_loose_integer(arg));
			               });

			return g;
		}

		instruction create_instruction_cycle_rail(const line_splitting::instruction_info& inst) {
			indices_at_least(inst, 1, "Cycle.Rail");
			args_at_least(inst, 1, "Cycle.Rail");

			instructions::cycle::Rail r;

			r.cycle_structure_index = std::size_t(util::parse_loose_integer(inst.indices[0]));

			r.input_indices.reserve(inst.args.size());
			std::transform(inst.args.begin(), inst.args.end(), std::back_inserter(r.input_indices),
			               [](const std::string& arg) {
				               return std::size_t(util::parse_loose_integer(arg));
			               });

			return r;
		}
	} // namespace instruction_generation
} // namespace csv_rw_route
} // namespace parsers
