#include "executor_pass3.hpp"
#include <cmath>
#include <string>

using namespace std::string_literals;

namespace bve::parsers::csv_rw_route {
	void Pass3Executor::operator()(const instructions::route::Comment& inst) const {
		route_data_.comment = inst.text;
	}

	void Pass3Executor::operator()(const instructions::route::Image& inst) const {
		route_data_.image_location =
		    get_relative_file_(getFilename(inst.file_index), inst.filename);
	}

	void Pass3Executor::operator()(const instructions::route::Timetable& inst) const {
		route_data_.timetable_text = inst.text;
	}

	void Pass3Executor::operator()(const instructions::route::Change& inst) const {
		switch (inst.mode) {
			case instructions::route::Change::Mode::safety_activated_emergency_brakes:
				route_data_.safety_system_status =
				    SafetySystemStatus::safety_activated_emergency_brakes;
				break;
			case instructions::route::Change::Mode::safety_activated_service_brakes:
				route_data_.safety_system_status =
				    SafetySystemStatus::safety_activated_service_brakes;
				break;
			case instructions::route::Change::Mode::safety_deactivated_emergency_brakes:
				route_data_.safety_system_status =
				    SafetySystemStatus::safety_deactivated_emergency_brakes;
				break;
			default:
				break;
		}
	}

	void Pass3Executor::operator()(const instructions::route::Gauge& inst) const {
		route_data_.gauge = inst.width;
	}

	void Pass3Executor::operator()(const instructions::route::Signal& inst) const {
		if (inst.aspect_index >= route_data_.signal_speed.size()) {
			route_data_.signal_speed.resize(inst.aspect_index, -1);
		}

		route_data_.signal_speed[inst.aspect_index] = inst.speed * unit_of_speed_;
	}

	void Pass3Executor::operator()(const instructions::route::RunInterval& inst) const {
		route_data_.ai_train_start_intervals = inst.time_interval;
	}

	void Pass3Executor::operator()(
	    const instructions::route::AccelerationDueToGravity& inst) const {
		route_data_.acceleration_due_to_gravity = inst.value;
	}

	void Pass3Executor::operator()(const instructions::route::Elevation& inst) {
		route_data_.altitude = inst.height * units_of_length_[1];
	}

	void Pass3Executor::operator()(const instructions::route::Temperature& inst) const {
		route_data_.temperature = inst.celsius;
	}

	void Pass3Executor::operator()(const instructions::route::Pressure& inst) const {
		route_data_.pressure = inst.kpa;
	}

	void Pass3Executor::operator()(const instructions::route::DisplaySpeed& inst) const {
		route_data_.display_unit.unit_name = inst.unit_string;
		route_data_.display_unit.conversion_factor = inst.conversion_factor;
	}

	void Pass3Executor::operator()(const instructions::route::LoadingScreen& inst) const {
		route_data_.loading_image_location =
		    get_relative_file_(getFilename(inst.file_index), inst.filename);
	}

	void Pass3Executor::operator()(const instructions::route::StartTime& inst) const {
		route_data_.game_start_time = inst.time;
	}

	void Pass3Executor::operator()(const instructions::route::DynamicLight& inst) {
		auto const issuer_filename = getFilename(inst.file_index);

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

	void Pass3Executor::operator()(const instructions::route::AmbientLight& inst) const {
		if (used_dynamic_light_) {
			add_error(errors_, getFilename(inst.file_index), inst.line,
			          "Route.DynamicLight has already been used, ignoring Route.AmbiantLight"s);
			return;
		}

		if (route_data_.lighting.empty()) {
			route_data_.lighting.resize(1);
		}

		route_data_.lighting[0].ambient = inst.color;
	}

	void Pass3Executor::operator()(const instructions::route::DirectionalLight& inst) const {
		if (used_dynamic_light_) {
			add_error(errors_, getFilename(inst.file_index), inst.line,
			          "Route.DynamicLight has already been used, ignoring "
			          "Route.DirectionalLight");
			return;
		}

		if (route_data_.lighting.empty()) {
			route_data_.lighting.resize(1);
		}

		route_data_.lighting[0].directional_lighting = inst.color;
	}

	void Pass3Executor::operator()(const instructions::route::LightDirection& inst) const {
		if (used_dynamic_light_) {
			add_error(errors_, getFilename(inst.file_index), inst.line,
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
} // namespace bve::parsers::csv_rw_route
