#include "executor_pass3.hpp"
#include <cmath>
#include <string>

using namespace std::string_literals;

namespace parsers {
namespace csv_rw_route {
	void pass3_executor::operator()(const instructions::route::Comment& inst) {
		_route_data.comment = inst.text;
	}

	void pass3_executor::operator()(const instructions::route::Image& inst) {
		_route_data.image_location =
		    _get_relative_file(get_filename(inst.file_index), inst.filename);
	}

	void pass3_executor::operator()(const instructions::route::Timetable& inst) {
		_route_data.timetable_text = inst.text;
	}

	void pass3_executor::operator()(const instructions::route::Change& inst) {
		switch (inst.mode) {
			case instructions::route::Change::SaftyActiviatedEmergencyBrakes:
				_route_data.safty_system_status = SaftySystemStatus::SaftyActiviatedEmergencyBrakes;
				break;
			case instructions::route::Change::SaftyActiviatedServiceBrakes:
				_route_data.safty_system_status = SaftySystemStatus::SaftyActiviatedServiceBrakes;
				break;
			case instructions::route::Change::SaftyDeactivatedEmergencyBrakes:
				_route_data.safty_system_status =
				    SaftySystemStatus::SaftyDeactivatedEmergencyBrakes;
				break;
			default:
				break;
		}
	}

	void pass3_executor::operator()(const instructions::route::Guage& inst) {
		_route_data.guage = inst.guage;
	}

	void pass3_executor::operator()(const instructions::route::Signal& inst) {
		if (inst.aspect_index >= _route_data.signal_speed.size()) {
			_route_data.signal_speed.resize(inst.aspect_index, -1);
		}

		_route_data.signal_speed[inst.aspect_index] = inst.speed * unit_of_speed;
	}

	void pass3_executor::operator()(const instructions::route::RunInterval& inst) {
		_route_data.ai_train_start_intervals = inst.time_interval;
	}

	void pass3_executor::operator()(const instructions::route::AccelerationDueToGravity& inst) {
		_route_data.acceleration_due_to_gravity = inst.value;
	}

	void pass3_executor::operator()(const instructions::route::Elevation& inst) {
		_route_data.altitude = inst.height * units_of_length[1];
	}

	void pass3_executor::operator()(const instructions::route::Temperature& inst) {
		_route_data.temperature = inst.celcius;
	}

	void pass3_executor::operator()(const instructions::route::Pressure& inst) {
		_route_data.pressure = inst.kPa;
	}

	void pass3_executor::operator()(const instructions::route::DisplaySpeed& inst) {
		_route_data.display_unit.unit_name = inst.unit_string;
		_route_data.display_unit.conversion_factor = inst.conversion_factor;
	}

	void pass3_executor::operator()(const instructions::route::LoadingScreen& inst) {
		_route_data.loading_image_location =
		    _get_relative_file(get_filename(inst.file_index), inst.filename);
	}

	void pass3_executor::operator()(const instructions::route::StartTime& inst) {
		_route_data.game_start_time = inst.time;
	}

	void pass3_executor::operator()(const instructions::route::DynamicLight& inst) {
		auto issuer_filename = get_filename(inst.file_index);

		if (!_route_data.lighting.empty()) {
			errors::add_error(
			    _errors, issuer_filename, inst.line,
			    "Route.DynamicLight is overwriting all prior calls the Route Lighting functions"s);
		}

		auto filename = _get_relative_file(issuer_filename, inst.filename);

		std::string file_contents;
		try {
			file_contents = util::load_from_file_utf8_bom(filename);
			_route_data.lighting =
			    xml::dynamic_lighting::parse(filename, std::move(file_contents), _errors);
		}
		catch (const std::invalid_argument& e) {
			errors::add_error(_errors, issuer_filename, inst.line, e.what());
		}

		used_dynamic_light = true;
	}

	void pass3_executor::operator()(const instructions::route::AmbiantLight& inst) {
		if (used_dynamic_light) {
			errors::
			    add_error(_errors, get_filename(inst.file_index), inst.line,
			              "Route.DynamicLight has already been used, ignoring Route.AmbiantLight"s);
			return;
		}

		if (_route_data.lighting.empty()) {
			_route_data.lighting.resize(1);
		}

		_route_data.lighting[0].ambient = inst.color;
	}

	void pass3_executor::operator()(const instructions::route::DirectionalLight& inst) {
		if (used_dynamic_light) {
			errors::add_error(_errors, get_filename(inst.file_index), inst.line,
			                  "Route.DynamicLight has already been used, ignoring "
			                  "Route.DirectionalLight");
			return;
		}

		if (_route_data.lighting.empty()) {
			_route_data.lighting.resize(1);
		}

		_route_data.lighting[0].directional_lighting = inst.color;
	}

	void pass3_executor::operator()(const instructions::route::LightDirection& inst) {
		if (used_dynamic_light) {
			errors::add_error(
			    _errors, get_filename(inst.file_index), inst.line,
			    "Route.DynamicLight has already been used, ignoring Route.LightDirection"s);
			return;
		}

		if (_route_data.lighting.empty()) {
			_route_data.lighting.resize(1);
		}

		_route_data.lighting[0].light_direction.x = std::cos(inst.theta) * std::sin(inst.phi);
		_route_data.lighting[0].light_direction.y = -std::sin(inst.theta);
		_route_data.lighting[0].light_direction.z = std::cos(inst.theta) * std::cos(inst.phi);
	}
} // namespace csv_rw_route
} // namespace parsers
