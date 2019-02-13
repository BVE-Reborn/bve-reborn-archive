#include "instruction_generator.hpp"

namespace bve::parsers::csv_rw_route::instruction_generation {
	Instruction create_instruction_options_unitoflength(
	    const line_splitting::InstructionInfo& inst) {
		instructions::options::UnitOfLength uol;
		uol.factors_in_meters.reserve(std::max(std::size_t(2), inst.args.size() + 1));
		uol.factors_in_meters.emplace_back(1.0F);
		if (!inst.args.empty()) {
			uol.factors_in_meters.emplace_back(util::parse_loose_float(inst.args[0], 1));
			std::transform(inst.args.begin() + 1, inst.args.end(),
			               std::back_inserter(uol.factors_in_meters),
			               [](const std::string& s) { return util::parse_loose_float(s, 0); });
		}
		else {
			uol.factors_in_meters.emplace_back(1.0F);
		}

		return uol;
	}

	Instruction create_instruction_options_unitofspeed(
	    const line_splitting::InstructionInfo& inst) {
		return create_single_float_instruction<instructions::options::UnitOfSpeed>(inst,
		                                                                           "Options."
		                                                                           "UnitOfSpeed",
		                                                                           1);
	}

	Instruction create_instruction_options_blocklength(
	    const line_splitting::InstructionInfo& inst) {
		return create_single_float_instruction<instructions::options::BlockLength>(inst,
		                                                                           "Options."
		                                                                           "BlockLength",
		                                                                           25);
	}

	Instruction create_instruction_options_objectvisibility(
	    const line_splitting::InstructionInfo& inst) {
		instructions::options::ObjectVisibility ov;

		if (!inst.args.empty()) {
			auto const value = util::parse_loose_integer(inst.args[0], 0);

			ov.mode = value == 0 ? instructions::options::ObjectVisibility::Mode::legacy
			                     : instructions::options::ObjectVisibility::Mode::track_based;
		}
		else {
			ov.mode = instructions::options::ObjectVisibility::Mode::legacy;
		}

		return ov;
	}

	Instruction create_instruction_options_sectionbehavior(
	    const line_splitting::InstructionInfo& inst) {
		instructions::options::SectionBehavior sb;

		if (!inst.args.empty()) {
			auto const value = util::parse_loose_integer(inst.args[0], 0);

			sb.mode = value == 0 ? instructions::options::SectionBehavior::Mode::normal
			                     : instructions::options::SectionBehavior::Mode::simplified;
		}
		else {
			sb.mode = instructions::options::SectionBehavior::Mode::normal;
		}

		return sb;
	}

	Instruction create_instruction_options_cantbehavior(
	    const line_splitting::InstructionInfo& inst) {
		instructions::options::CantBehavior cb;

		if (!inst.args.empty()) {
			auto const value = util::parse_loose_integer(inst.args[0], 0);

			cb.mode = value == 0 ? instructions::options::CantBehavior::Mode::unsigned_cant
			                     : instructions::options::CantBehavior::Mode::signed_cant;
		}
		else {
			cb.mode = instructions::options::CantBehavior::Mode::unsigned_cant;
		}

		return cb;
	}

	Instruction create_instruction_options_fogbehavior(
	    const line_splitting::InstructionInfo& inst) {
		instructions::options::FogBehavior fb;

		if (!inst.args.empty()) {
			auto const value = util::parse_loose_integer(inst.args[0], 0);

			fb.mode = value == 0 ? instructions::options::FogBehavior::Mode::block_based
			                     : instructions::options::FogBehavior::Mode::interpolated;
		}
		else {
			fb.mode = instructions::options::FogBehavior::Mode::block_based;
		}

		return fb;
	}

	Instruction create_instruction_options_compatibletransparencymode(
	    const line_splitting::InstructionInfo& inst) {
		instructions::options::CompatibleTransparencyMode ctm;

		if (!inst.args.empty()) {
			auto const value = util::parse_loose_integer(inst.args[0], 0);

			ctm.mode = value == 0 ? instructions::options::CompatibleTransparencyMode::Mode::off
			                      : instructions::options::CompatibleTransparencyMode::Mode::on;
		}
		else {
			ctm.mode = instructions::options::CompatibleTransparencyMode::Mode::off;
		}

		return ctm;
	}

	Instruction create_instruction_options_enablebvetshacks(
	    const line_splitting::InstructionInfo& inst) {
		instructions::options::EnableBveTsHacks ts_hacks;

		if (!inst.args.empty()) {
			auto const value = util::parse_loose_integer(inst.args[0], 0);

			ts_hacks.mode = value == 0 ? instructions::options::EnableBveTsHacks::Mode::off
			                           : instructions::options::EnableBveTsHacks::Mode::on;
		}
		else {
			ts_hacks.mode = instructions::options::EnableBveTsHacks::Mode::off;
		}

		return ts_hacks;
	}
} // namespace bve::parsers::csv_rw_route::instruction_generation
