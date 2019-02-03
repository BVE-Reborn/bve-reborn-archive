#include "common.hpp"

namespace bve::parsers::csv_rw_route {
	void CSVRwRouteInstructionIOClass::operator()(const instructions::route::Comment& inst) {
		start(inst, "Route.Comment");

		PRINT_VALUE(text);

		end();
	};
	void CSVRwRouteInstructionIOClass::operator()(const instructions::route::Image& inst) {
		start(inst, "Route.Image");

		PRINT_VALUE(filename);

		end();
	};
	void CSVRwRouteInstructionIOClass::operator()(const instructions::route::Timetable& inst) {
		start(inst, "Route.Timetable");

		PRINT_VALUE(text);

		end();
	};
	void CSVRwRouteInstructionIOClass::operator()(const instructions::route::Change& inst) {
		start(inst, "Route.Change");

		PRINT_ENUM3(mode, route::Change::Mode, safety_activated_service_brakes,
		            safety_activated_emergency_brakes, safety_deactivated_emergency_brakes);

		end();
	};
	void CSVRwRouteInstructionIOClass::operator()(const instructions::route::Gauge& inst) {
		start(inst, "Route.Gauge");

		PRINT_VALUE(width);

		end();
	};
	void CSVRwRouteInstructionIOClass::operator()(const instructions::route::Signal& inst) {
		start(inst, "Route.Signal");

		PRINT_VALUE(aspect_index);

		end();
	};
	void CSVRwRouteInstructionIOClass::operator()(const instructions::route::RunInterval& inst) {
		start(inst, "Route.RunInterval");

		PRINT_VECTOR(time_interval);

		end();
	};
	void CSVRwRouteInstructionIOClass::operator()(
	    const instructions::route::AccelerationDueToGravity& inst) {
		start(inst, "Route.AccelerationDueToGravity");

		PRINT_VALUE(value);

		end();
	};
	void CSVRwRouteInstructionIOClass::operator()(const instructions::route::Elevation& inst) {
		start(inst, "Route.Elevation");

		PRINT_VALUE(height);

		end();
	};
	void CSVRwRouteInstructionIOClass::operator()(const instructions::route::Temperature& inst) {
		start(inst, "Route.Temperature");

		PRINT_VALUE(celsius);

		end();
	};
	void CSVRwRouteInstructionIOClass::operator()(const instructions::route::Pressure& inst) {
		start(inst, "Route.Pressure");

		PRINT_VALUE(kpa);

		end();
	};
	void CSVRwRouteInstructionIOClass::operator()(const instructions::route::DisplaySpeed& inst) {
		start(inst, "Route.DisplaySpeed");

		PRINT_VALUE(unit_string);
		PRINT_VALUE(conversion_factor);

		end();
	};
	void CSVRwRouteInstructionIOClass::operator()(const instructions::route::LoadingScreen& inst) {
		start(inst, "Route.LoadingScreen");

		PRINT_VALUE(filename);

		end();
	};
	void CSVRwRouteInstructionIOClass::operator()(const instructions::route::StartTime& inst) {
		start(inst, "Route.StartTime");

		PRINT_TIME(time);

		end();
	};
	void CSVRwRouteInstructionIOClass::operator()(const instructions::route::DynamicLight& inst) {
		start(inst, "Route.DynamicLight");

		PRINT_VALUE(filename);

		end();
	};
	void CSVRwRouteInstructionIOClass::operator()(const instructions::route::AmbientLight& inst) {
		start(inst, "Route.AmbientLight");

		PRINT_COLOR(color);

		end();
	};
	void CSVRwRouteInstructionIOClass::operator()(
	    const instructions::route::DirectionalLight& inst) {
		start(inst, "Route.DirectionalLight");

		PRINT_COLOR(color);

		end();
	};
	void CSVRwRouteInstructionIOClass::operator()(const instructions::route::LightDirection& inst) {
		start(inst, "Route.LightDirection");

		PRINT_VALUE(theta);
		PRINT_VALUE(phi);

		end();
	};
} // namespace bve::parsers::csv_rw_route