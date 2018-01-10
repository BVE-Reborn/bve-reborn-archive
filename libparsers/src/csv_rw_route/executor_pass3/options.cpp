#include "../executor_pass3.hpp"

namespace parsers {
namespace csv_rw_route {
	// defined in executor_pass3_options.cpp
	void pass3_executor::operator()(const instructions::options::UnitOfLength& inst) {
		units_of_length = inst.factors_in_meters;
	}
	void pass3_executor::operator()(const instructions::options::UnitOfSpeed& inst) {
		unit_of_speed = inst.factor_in_kph;
	}
	void pass3_executor::operator()(const instructions::options::SectionBehavior& inst) {
		section_behavior = inst.mode;
	}

	/*void pass3_executor::operator()(const instructions::options::FogBehavior&
	 * inst) {}*/

	void pass3_executor::operator()(const instructions::options::CompatibleTransparencyMode& inst) {
		_route_data.compatibility.bve2_4_transparancy = inst.mode == inst.On;
	}
	void pass3_executor::operator()(const instructions::options::EnableBveTsHacks& inst) {
		_route_data.compatibility.bve2_4_content = inst.mode == inst.On;
	}
} // namespace csv_rw_route
} // namespace parsers
