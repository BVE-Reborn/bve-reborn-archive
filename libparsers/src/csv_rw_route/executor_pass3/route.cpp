#include "executor_pass3.hpp"
#include <cmath>
#include <string>

using namespace std::string_literals;

namespace parsers {
namespace csv_rw_route {
	void pass3_executor::operator()(const instructions::route::comment& inst) const {
		route_data_.comment = inst.text;
	}

	void pass3_executor::operator()(const instructions::route::image& inst) const {
		route_data_.image_location =
		    get_relative_file_(get_filename(inst.file_index), inst.filename);
	}

	void pass3_executor::operator()(const instructions::route::timetable& inst) const {
		route_data_.timetable_text = inst.text;
	}

	void pass3_executor::operator()(const instructions::route::change& inst) const {
		switch (inst.mode) {
			case instructions::route::change::safty_activiated_emergency_brakes:
				route_data_.safty_system_status =
				    safty_system_status_t::safty_activiated_emergency_brakes;
				break;
			case instructions::route::change::safty_activiated_service_brakes:
				route_data_.safty_system_status =
				    safty_system_status_t::safty_activiated_service_brakes;
				break;
			case instructions::route::change::safty_deactivated_emergency_brakes:
				route_data_.safty_system_status =
				    safty_system_status_t::safty_deactivated_emergency_brakes;
				break;
			default:
				break;
		}
	}

	void pass3_executor::operator()(const instructions::route::guage& inst) const {
		route_data_.guage = inst.width;
	}

	void pass3_executor::operator()(const instructions::route::signal& inst) const {
		if (inst.aspect_index >= route_data_.signal_speed.size()) {
			route_data_.signal_speed.resize(inst.aspect_index, -1);
		}

		route_data_.signal_speed[inst.aspect_index] = inst.speed * unit_of_speed_;
	}

	void pass3_executor::operator()(const instructions::route::run_interval& inst) const {
		route_data_.ai_train_start_intervals = inst.time_interval;
	}

	void pass3_executor::operator()(
	    const instructions::route::acceleration_due_to_gravity& inst) const {
		route_data_.acceleration_due_to_gravity = inst.value;
	}

	void pass3_executor::operator()(const instructions::route::elevation& inst) {
		route_data_.altitude = inst.height * units_of_length_[1];
	}

	void pass3_executor::operator()(const instructions::route::temperature& inst) const {
		route_data_.temperature = inst.celcius;
	}

	void pass3_executor::operator()(const instructions::route::pressure& inst) const {
		route_data_.pressure = inst.kpa;
	}

	void pass3_executor::operator()(const instructions::route::display_speed& inst) const {
		route_data_.display_unit.unit_name = inst.unit_string;
		route_data_.display_unit.conversion_factor = inst.conversion_factor;
	}

	void pass3_executor::operator()(const instructions::route::loading_screen& inst) const {
		route_data_.loading_image_location =
		    get_relative_file_(get_filename(inst.file_index), inst.filename);
	}

	void pass3_executor::operator()(const instructions::route::start_time& inst) const {
		route_data_.game_start_time = inst.time;
	}

	void pass3_executor::operator()(const instructions::route::dynamic_light& inst) {
		auto const issuer_filename = get_filename(inst.file_index);

		if (!route_data_.lighting.empty()) {
			add_error(
			    errors_, issuer_filename, inst.line,
			    "Route.DynamicLight is overwriting all prior calls the Route Lighting functions"s);
		}

		auto const filename = get_relative_file_(issuer_filename, inst.filename);

		try {
			auto file_contents = util::load_from_file_utf8_bom(filename);
			route_data_.lighting =
			    xml::dynamic_lighting::parse(filename, std::move(file_contents), errors_);
		}
		catch (const std::invalid_argument& e) {
			add_error(errors_, issuer_filename, inst.line, e.what());
		}

		used_dynamic_light_ = true;
	}

	void pass3_executor::operator()(const instructions::route::ambiant_light& inst) const {
		if (used_dynamic_light_) {
			add_error(errors_, get_filename(inst.file_index), inst.line,
			          "Route.DynamicLight has already been used, ignoring Route.AmbiantLight"s);
			return;
		}

		if (route_data_.lighting.empty()) {
			route_data_.lighting.resize(1);
		}

		route_data_.lighting[0].ambient = inst.color;
	}

	void pass3_executor::operator()(const instructions::route::directional_light& inst) const {
		if (used_dynamic_light_) {
			add_error(errors_, get_filename(inst.file_index), inst.line,
			          "Route.DynamicLight has already been used, ignoring "
			          "Route.DirectionalLight");
			return;
		}

		if (route_data_.lighting.empty()) {
			route_data_.lighting.resize(1);
		}

		route_data_.lighting[0].directional_lighting = inst.color;
	}

	void pass3_executor::operator()(const instructions::route::light_direction& inst) const {
		if (used_dynamic_light_) {
			add_error(errors_, get_filename(inst.file_index), inst.line,
			          "Route.DynamicLight has already been used, ignoring Route.LightDirection"s);
			return;
		}

		if (route_data_.lighting.empty()) {
			route_data_.lighting.resize(1);
		}

		route_data_.lighting[0].light_direction.x = std::cos(inst.theta) * std::sin(inst.phi);
		route_data_.lighting[0].light_direction.y = -std::sin(inst.theta);
		route_data_.lighting[0].light_direction.z = std::cos(inst.theta) * std::cos(inst.phi);
	}
} // namespace csv_rw_route
} // namespace parsers
