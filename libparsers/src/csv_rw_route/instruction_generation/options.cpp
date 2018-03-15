#include "instruction_generator.hpp"

namespace parsers {
namespace csv_rw_route {
	namespace instruction_generation {
		instruction create_instruction_options_unitoflength(
		    const line_splitting::instruction_info& inst) {
			instructions::options::UnitOfLength uol;
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
			    instructions::options::UnitOfSpeed>(inst, "Options.UnitOfSpeed", 1);
		}

		instruction create_instruction_options_blocklength(
		    const line_splitting::instruction_info& inst) {
			return create_single_float_instruction<
			    instructions::options::BlockLength>(inst, "Options.BlockLength", 25);
		}

		instruction create_instruction_options_objectvisibility(
		    const line_splitting::instruction_info& inst) {
			instructions::options::ObjectVisibility ov;

			if (!inst.args.empty()) {
				auto value = util::parse_loose_integer(inst.args[0], 0);

				ov.mode = value == 0 ? instructions::options::ObjectVisibility::Legacy
				                     : instructions::options::ObjectVisibility::TrackBased;
			}
			else {
				ov.mode = instructions::options::ObjectVisibility::Legacy;
			}

			return ov;
		}

		instruction create_instruction_options_sectionbehavior(
		    const line_splitting::instruction_info& inst) {
			instructions::options::SectionBehavior sb;

			if (!inst.args.empty()) {
				auto value = util::parse_loose_integer(inst.args[0], 0);

				sb.mode = value == 0 ? instructions::options::SectionBehavior::Default
				                     : instructions::options::SectionBehavior::Simplified;
			}
			else {
				sb.mode = instructions::options::SectionBehavior::Default;
			}

			return sb;
		}

		instruction create_instruction_options_cantbehavior(
		    const line_splitting::instruction_info& inst) {
			instructions::options::CantBehavior cb;

			if (!inst.args.empty()) {
				auto value = util::parse_loose_integer(inst.args[0], 0);

				cb.mode = value == 0 ? instructions::options::CantBehavior::Unsigned
				                     : instructions::options::CantBehavior::Signed;
			}
			else {
				cb.mode = instructions::options::CantBehavior::Unsigned;
			}

			return cb;
		}

		instruction create_instruction_options_fogbehavior(
		    const line_splitting::instruction_info& inst) {
			instructions::options::FogBehavior fb;

			if (!inst.args.empty()) {
				auto value = util::parse_loose_integer(inst.args[0], 0);

				fb.mode = value == 0 ? instructions::options::FogBehavior::BlockBased
				                     : instructions::options::FogBehavior::Interpolated;
			}
			else {
				fb.mode = instructions::options::FogBehavior::BlockBased;
			}

			return fb;
		}

		instruction create_instruction_options_compatibletransparencymode(
		    const line_splitting::instruction_info& inst) {
			instructions::options::CompatibleTransparencyMode ctm;

			if (!inst.args.empty()) {
				auto value = util::parse_loose_integer(inst.args[0], 0);

				ctm.mode = value == 0 ? instructions::options::CompatibleTransparencyMode::Off
				                      : instructions::options::CompatibleTransparencyMode::On;
			}
			else {
				ctm.mode = instructions::options::CompatibleTransparencyMode::Off;
			}

			return ctm;
		}

		instruction create_instruction_options_enablebvetshacks(
		    const line_splitting::instruction_info& inst) {
			instructions::options::EnableBveTsHacks ebth;

			if (!inst.args.empty()) {
				auto value = util::parse_loose_integer(inst.args[0], 0);

				ebth.mode = value == 0 ? instructions::options::EnableBveTsHacks::Off
				                       : instructions::options::EnableBveTsHacks::On;
			}
			else {
				ebth.mode = instructions::options::EnableBveTsHacks::Off;
			}

			return ebth;
		}
	} // namespace instruction_generation
} // namespace csv_rw_route
} // namespace parsers
