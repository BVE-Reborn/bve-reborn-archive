#include "executor_pass3.hpp"

namespace parsers::csv_rw_route {
	// defined in executor_pass3_options.cpp
	void pass3_executor::operator()(const instructions::options::unit_of_length& inst) {
		units_of_length_ = inst.factors_in_meters;
	}
	void pass3_executor::operator()(const instructions::options::unit_of_speed& inst) {
		unit_of_speed_ = inst.factor_in_kph;
	}
	void pass3_executor::operator()(const instructions::options::section_behavior& inst) {
		section_behavior_ = inst.mode;
	}

	/*void pass3_executor::operator()(const instructions::options::FogBehavior&
	 * inst) {}*/

	void pass3_executor::operator()(
	    const instructions::options::compatible_transparency_mode& inst) const {
		route_data_.compatibility.bve2_4_transparancy = inst.mode == inst.on;
	}
	void pass3_executor::operator()(const instructions::options::enable_bve_ts_hacks& inst) const {
		route_data_.compatibility.bve2_4_content = inst.mode == inst.on;
	}
} // namespace parsers::csv_rw_route
