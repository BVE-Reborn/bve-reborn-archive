#include "../executor_pass3.hpp"

namespace parsers {
namespace csv_rw_route {
	// defined in executor_pass3_options.cpp
	void pass3_executor::operator()(const instructions::options::UnitOfLength& instr) {
		units_of_length = instr.factors_in_meters;
	}
	void pass3_executor::operator()(const instructions::options::UnitOfSpeed& instr) {
		unit_of_speed = instr.factor_in_kph;
	}
	void pass3_executor::operator()(const instructions::options::SectionBehavior& instr) {
		section_behavior = instr.mode;
	}

	/*void pass3_executor::operator()(const instructions::options::FogBehavior& instr) {}*/

	void pass3_executor::operator()(const instructions::options::CompatibleTransparencyMode& instr) {
		_route_data.compatibility.bve2_4_transparancy = instr.mode == instr.On;
	}
	void pass3_executor::operator()(const instructions::options::EnableBveTsHacks& instr) {
		_route_data.compatibility.bve2_4_content = instr.mode == instr.On;
	}
} // namespace csv_rw_route
} // namespace parsers
