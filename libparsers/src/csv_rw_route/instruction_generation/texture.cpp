#include "instruction_generator.hpp"
#include <gsl/gsl_util>

namespace parsers {
namespace csv_rw_route {
	namespace instruction_generation {
		instruction create_instruction_texture_background(
		    const line_splitting::instruction_info& inst) {
			indices_at_least(inst, 1, "Texture.Background");
			args_at_least(inst, 1, "Texture.Background");

			auto const background_texture_index =
			    gsl::narrow<std::size_t>(util::parse_loose_integer(inst.indices[0]));

			if (inst.suffix.empty() || inst.suffix == "load") {
				instructions::texture::background_load bl;
				bl.background_texture_index = background_texture_index;
				bl.filename = inst.args[0];
				return bl;
			}

			auto const number = gsl::narrow<std::size_t>(util::parse_loose_integer(inst.args[0]));

			if (inst.suffix == "x") {
				instructions::texture::background_x x;
				x.background_texture_index = background_texture_index;
				x.repetition_count = number;
				return x;
			}

			if (inst.suffix == "aspect") {
				instructions::texture::background_aspect ba;

				ba.background_texture_index = background_texture_index;
				ba.mode = number != 0 ? instructions::texture::background_aspect::aspect
				                      : instructions::texture::background_aspect::fixed;

				return ba;
			}

			throw std::invalid_argument("Invalid suffix to Texture.Background");
		}
	} // namespace instruction_generation
} // namespace csv_rw_route
} // namespace parsers
