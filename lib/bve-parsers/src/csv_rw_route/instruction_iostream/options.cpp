#include "common.hpp"

namespace bve::parsers::csv_rw_route {
	void CSVRwRouteInstructionIOClass::operator()(const instructions::options::UnitOfLength& inst) {
		start(inst, "Options.UnitOfLength");

		PRINT_VECTOR(factors_in_meters);

		end();
	};
	void CSVRwRouteInstructionIOClass::operator()(const instructions::options::UnitOfSpeed& inst) {
		start(inst, "Options.UnitOfSpeed");

		PRINT_VALUE(factor_in_kph);

		end();
	};
	void CSVRwRouteInstructionIOClass::operator()(const instructions::options::BlockLength& inst) {
		start(inst, "Options.BlockLength");

		PRINT_VALUE(length);

		end();
	};
	void CSVRwRouteInstructionIOClass::operator()(
	    const instructions::options::ObjectVisibility& inst) {
		start(inst, "Options.ObjectVisibility");

		PRINT_ENUM2(mode, options::ObjectVisibility::Mode, legacy, track_based);

		end();
	};
	void CSVRwRouteInstructionIOClass::operator()(
	    const instructions::options::SectionBehavior& inst) {
		start(inst, "Options.SectionBehavior");

		PRINT_ENUM2(mode, options::SectionBehavior::Mode, normal, simplified);

		end();
	};
	void CSVRwRouteInstructionIOClass::operator()(const instructions::options::CantBehavior& inst) {
		start(inst, "Options.CantBehavior");

		PRINT_ENUM2(mode, options::CantBehavior::Mode, unsigned_cant, signed_cant);

		end();
	};
	void CSVRwRouteInstructionIOClass::operator()(const instructions::options::FogBehavior& inst) {
		start(inst, "Options.FogBehavior");

		PRINT_ENUM2(mode, options::FogBehavior::Mode, block_based, interpolated);

		end();
	};
	void CSVRwRouteInstructionIOClass::operator()(
	    const instructions::options::CompatibleTransparencyMode& inst) {
		start(inst, "Options.CompatibleTransparencyMode");

		PRINT_ENUM2(mode, options::CompatibleTransparencyMode::Mode, off, on);

		end();
	};
	void CSVRwRouteInstructionIOClass::operator()(
	    const instructions::options::EnableBveTsHacks& inst) {
		start(inst, "Options.EnableBveTsHacks");

		PRINT_ENUM2(mode, options::EnableBveTsHacks::Mode, off, on);

		end();
	};
} // namespace bve::parsers::csv_rw_route