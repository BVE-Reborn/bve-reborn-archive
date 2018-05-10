#include "instruction_generator.hpp"

namespace parsers {
namespace csv_rw_route {
	namespace instruction_generation {
		instruction create_instruction_options_unitoflength(
		    const line_splitting::instruction_info& inst) {
			instructions::options::unit_of_length uol;
			uol.factors_in_meters.reserve(std::max(std::size_t(2), inst.args.size() + 1));
			uol.factors_in_meters.emplace_back(1.0f);
			if (!inst.args.empty()) {
				uol.factors_in_meters.emplace_back(util::parse_loose_float(inst.args[0], 1));
				std::transform(inst.args.begin() + 1, inst.args.end(),
				               std::back_inserter(uol.factors_in_meters),
				               [](const std::string& s) { return util::parse_loose_float(s, 0); });
			}
			else {
				uol.factors_in_meters.emplace_back(1.0f);
			}

			return uol;
		}

		instruction create_instruction_options_unitofspeed(
		    const line_splitting::instruction_info& inst) {
			return create_single_float_instruction<
			    instructions::options::unit_of_speed>(inst, "Options.UnitOfSpeed", 1);
		}

		instruction create_instruction_options_blocklength(
		    const line_splitting::instruction_info& inst) {
			return create_single_float_instruction<
			    instructions::options::block_length>(inst, "Options.BlockLength", 25);
		}

		instruction create_instruction_options_objectvisibility(
		    const line_splitting::instruction_info& inst) {
			instructions::options::object_visibility ov;

			if (!inst.args.empty()) {
				auto const value = util::parse_loose_integer(inst.args[0], 0);

				ov.mode = value == 0 ? instructions::options::object_visibility::legacy
				                     : instructions::options::object_visibility::track_based;
			}
			else {
				ov.mode = instructions::options::object_visibility::legacy;
			}

			return ov;
		}

		instruction create_instruction_options_sectionbehavior(
		    const line_splitting::instruction_info& inst) {
			instructions::options::section_behavior sb;

			if (!inst.args.empty()) {
				auto const value = util::parse_loose_integer(inst.args[0], 0);

				sb.mode = value == 0 ? instructions::options::section_behavior::normal
				                     : instructions::options::section_behavior::simplified;
			}
			else {
				sb.mode = instructions::options::section_behavior::normal;
			}

			return sb;
		}

		instruction create_instruction_options_cantbehavior(
		    const line_splitting::instruction_info& inst) {
			instructions::options::cant_behavior cb;

			if (!inst.args.empty()) {
				auto const value = util::parse_loose_integer(inst.args[0], 0);

				cb.mode = value == 0 ? instructions::options::cant_behavior::unsigned_cant
				                     : instructions::options::cant_behavior::signed_cant;
			}
			else {
				cb.mode = instructions::options::cant_behavior::unsigned_cant;
			}

			return cb;
		}

		instruction create_instruction_options_fogbehavior(
		    const line_splitting::instruction_info& inst) {
			instructions::options::fog_behavior fb;

			if (!inst.args.empty()) {
				auto const value = util::parse_loose_integer(inst.args[0], 0);

				fb.mode = value == 0 ? instructions::options::fog_behavior::block_based
				                     : instructions::options::fog_behavior::interpolated;
			}
			else {
				fb.mode = instructions::options::fog_behavior::block_based;
			}

			return fb;
		}

		instruction create_instruction_options_compatibletransparencymode(
		    const line_splitting::instruction_info& inst) {
			instructions::options::compatible_transparency_mode ctm;

			if (!inst.args.empty()) {
				auto const value = util::parse_loose_integer(inst.args[0], 0);

				ctm.mode = value == 0 ? instructions::options::compatible_transparency_mode::off
				                      : instructions::options::compatible_transparency_mode::on;
			}
			else {
				ctm.mode = instructions::options::compatible_transparency_mode::off;
			}

			return ctm;
		}

		instruction create_instruction_options_enablebvetshacks(
		    const line_splitting::instruction_info& inst) {
			instructions::options::enable_bve_ts_hacks ebth;

			if (!inst.args.empty()) {
				auto const value = util::parse_loose_integer(inst.args[0], 0);

				ebth.mode = value == 0 ? instructions::options::enable_bve_ts_hacks::off
				                       : instructions::options::enable_bve_ts_hacks::on;
			}
			else {
				ebth.mode = instructions::options::enable_bve_ts_hacks::off;
			}

			return ebth;
		}
	} // namespace instruction_generation
} // namespace csv_rw_route
} // namespace parsers
