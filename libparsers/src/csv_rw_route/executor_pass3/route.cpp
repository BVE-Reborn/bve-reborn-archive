#include "../executor_pass3.hpp"
#include <cmath>
#include <string>

using namespace std::string_literals;

namespace parsers {
namespace csv_rw_route {
	void pass3_executor::operator()(const instructions::route::Comment& instr) {
		_route_data.comment = instr.text;
	}

	void pass3_executor::operator()(const instructions::route::Image& instr) {
		_route_data.image_location = _get_relative_file(get_filename(instr.file_index), instr.filename);
	}

	void pass3_executor::operator()(const instructions::route::Timetable& instr) {
		_route_data.timetable_text = instr.text;
	}

	void pass3_executor::operator()(const instructions::route::Change& instr) {
		switch (instr.mode) {
			case instructions::route::Change::SaftyActiviatedEmergencyBrakes:
				_route_data.safty_system_status = SaftySystemStatus::SaftyActiviatedEmergencyBrakes;
				break;
			case instructions::route::Change::SaftyActiviatedServiceBrakes:
				_route_data.safty_system_status = SaftySystemStatus::SaftyActiviatedServiceBrakes;
				break;
			case instructions::route::Change::SaftyDeactivatedEmergencyBrakes:
				_route_data.safty_system_status = SaftySystemStatus::SaftyDeactivatedEmergencyBrakes;
				break;
			default:
				break;
		}
	}

	void pass3_executor::operator()(const instructions::route::Guage& instr) {
		_route_data.guage = instr.guage;
	}

	void pass3_executor::operator()(const instructions::route::Signal& instr) {
		if (instr.aspect_index >= _route_data.signal_speed.size()) {
			_route_data.signal_speed.resize(instr.aspect_index, -1);
		}

		_route_data.signal_speed[instr.aspect_index] = instr.speed * unit_of_speed;
	}

	void pass3_executor::operator()(const instructions::route::RunInterval& instr) {
		_route_data.ai_train_start_intervals = instr.time_interval;
	}

	void pass3_executor::operator()(const instructions::route::AccelerationDueToGravity& instr) {
		_route_data.acceleration_due_to_gravity = instr.value;
	}

	void pass3_executor::operator()(const instructions::route::Elevation& instr) {
		_route_data.altitude = instr.height * units_of_length[1];
	}

	void pass3_executor::operator()(const instructions::route::Temperature& instr) {
		_route_data.temperature = instr.celcius;
	}

	void pass3_executor::operator()(const instructions::route::Pressure& instr) {
		_route_data.pressure = instr.kPa;
	}

	void pass3_executor::operator()(const instructions::route::DisplaySpeed& instr) {
		_route_data.display_unit.unit_name = instr.unit_string;
		_route_data.display_unit.conversion_factor = instr.conversion_factor;
	}

	void pass3_executor::operator()(const instructions::route::LoadingScreen& instr) {
		_route_data.loading_image_location = _get_relative_file(get_filename(instr.file_index), instr.filename);
	}

	void pass3_executor::operator()(const instructions::route::StartTime& instr) {
		_route_data.game_start_time = instr.time;
	}

	void pass3_executor::operator()(const instructions::route::DynamicLight& instr) {
		auto issuer_filename = get_filename(instr.file_index);

		if (!_route_data.lighting.empty()) {
			_errors[get_filename(instr.file_index)].emplace_back<errors::error_t>(
			    {instr.line, "Route.DynamicLight is overwriting all prior calls the Route Lighting functions"s});
		}

		auto filename = _get_relative_file(issuer_filename, instr.filename);

		try {
			_route_data.lighting = xml::dynamic_lighting::parse(filename, _errors);
		}
		catch (const std::invalid_argument& e) {
			_errors[issuer_filename].emplace_back<errors::error_t>({instr.line, e.what()});
		}

		used_dynamic_light = true;
	}

	void pass3_executor::operator()(const instructions::route::AmbiantLight& instr) {
		if (used_dynamic_light) {
			_errors[get_filename(instr.file_index)].emplace_back<errors::error_t>(
			    {instr.line, "Route.DynamicLight has already been used, ignoring Route.AmbiantLight"s});
			return;
		}

		if (_route_data.lighting.empty()) {
			_route_data.lighting.resize(1);
		}

		_route_data.lighting[0].ambient = instr.color;
	}

	void pass3_executor::operator()(const instructions::route::DirectionalLight& instr) {
		if (used_dynamic_light) {
			_errors[get_filename(instr.file_index)].emplace_back<errors::error_t>(
			    {instr.line, "Route.DynamicLight has already been used, ignoring Route.DirectionalLight"});
			return;
		}

		if (_route_data.lighting.empty()) {
			_route_data.lighting.resize(1);
		}

		_route_data.lighting[0].directional_lighting = instr.color;
	}

	void pass3_executor::operator()(const instructions::route::LightDirection& instr) {
		if (used_dynamic_light) {
			_errors[get_filename(instr.file_index)].emplace_back<errors::error_t>(
			    {instr.line, "Route.DynamicLight has already been used, ignoring Route.LightDirection"s});
			return;
		}

		if (_route_data.lighting.empty()) {
			_route_data.lighting.resize(1);
		}

		_route_data.lighting[0].light_direction.x = std::cos(instr.theta) * std::sin(instr.phi);
		_route_data.lighting[0].light_direction.y = -std::sin(instr.theta);
		_route_data.lighting[0].light_direction.z = std::cos(instr.theta) * std::cos(instr.phi);
	}
} // namespace csv_rw_route
} // namespace parsers
