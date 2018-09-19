#include "executor_pass3.hpp"

namespace bve::parsers::csv_rw_route {
	// defined in executor_pass3_options.cpp
	void pass3_executor::operator()(const instructions::options::UnitOfLength& inst) {
		units_of_length_ = inst.factors_in_meters;
	}
	void pass3_executor::operator()(const instructions::options::UnitOfSpeed& inst) {
		unit_of_speed_ = inst.factor_in_kph;
	}
	void pass3_executor::operator()(const instructions::options::SectionBehavior& inst) {
		section_behavior_ = inst.mode;
	}

	/*void pass3_executor::operator()(const instructions::options::FogBehavior&
	 * inst) {}*/

	void pass3_executor::operator()(
	    const instructions::options::CompatibleTransparencyMode& inst) const {
		route_data_.compatibility.bve2_4_transparancy =
		    inst.mode == instructions::options::CompatibleTransparencyMode::Mode::on;
	}
	void pass3_executor::operator()(const instructions::options::EnableBveTsHacks& inst) const {
		route_data_.compatibility.bve2_4_content =
		    inst.mode == instructions::options::EnableBveTsHacks::Mode::on;
	}
} // namespace bve::parsers::csv_rw_route
