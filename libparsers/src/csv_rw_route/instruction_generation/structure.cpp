#include "instruction_generator.hpp"

namespace parsers {
namespace csv_rw_route {
	namespace instruction_generation {
		instruction create_instruction_structure_command(
		    const line_splitting::instruction_info& inst) {
			indices_at_least(inst, 1, "Structure.Command");
			args_at_least(inst, 1, "Structure.Command");

			instructions::structure::Command c;

			c.command = command_type_mapping.find(inst.name)->second;

			c.structure_index = std::size_t(util::parse_loose_integer(inst.indices[0]));
			c.filename = inst.args[0];

			return c;
		}

		instruction create_instruction_structure_pole(
		    const line_splitting::instruction_info& inst) {
			indices_at_least(inst, 1, "Structure.Pole");
			args_at_least(inst, 1, "Structure.Pole");

			instructions::structure::Pole p;

			if (inst.indices.size() == 1) {
				p.additional_rails = 0;
				p.pole_structure_index = std::size_t(util::parse_loose_integer(inst.indices[0]));
			}
			else {
				p.additional_rails = std::size_t(util::parse_loose_integer(inst.indices[0]));
				p.pole_structure_index = std::size_t(util::parse_loose_integer(inst.indices[1]));
			}
			p.filename = inst.args[0];

			return p;
		}
	} // namespace instruction_generation
} // namespace csv_rw_route
} // namespace parsers
