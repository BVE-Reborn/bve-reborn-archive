#include "common.hpp"

namespace bve::parsers::csv_rw_route {
	void CSVRwRouteInstructionIOClass::operator()(const instructions::track::Limit& inst) {
		start(inst, "Track.Limit");

		PRINT_VALUE(speed);
		PRINT_ENUM3(post, track::Limit::Post, left, none, right);
		PRINT_ENUM3(course, track::Limit::Course, left, none, right);

		end();
	}
	void CSVRwRouteInstructionIOClass::operator()(const instructions::track::Section& inst) {
		start(inst, "Track.Section");

		PRINT_VECTOR(a_term);

		end();
	}
	void CSVRwRouteInstructionIOClass::operator()(const instructions::track::SigF& inst) {
		start(inst, "Track.SigF");

		PRINT_VALUE(signal_index);
		PRINT_VALUE(section);
		PRINT_VALUE(x_offset);
		PRINT_VALUE(y_offset);
		PRINT_VALUE(yaw);
		PRINT_VALUE(pitch);
		PRINT_VALUE(roll);

		end();
	}
	void CSVRwRouteInstructionIOClass::operator()(const instructions::track::Signal& inst) {
		start(inst, "Track.Signal");

		PRINT_ENUM8(type, track::Signal::Aspect, r_y, r_g, r_y_g, r_yy_y_g, r_y_yg_g, r_yy_y_yg_g, r_y_yg_g_gg, r_yy_y_yg_g_gg);
		PRINT_VALUE(x_offset);
		PRINT_VALUE(y_offset);
		PRINT_VALUE(yaw);
		PRINT_VALUE(pitch);
		PRINT_VALUE(roll);

		end();
	}
	void CSVRwRouteInstructionIOClass::operator()(const instructions::track::Relay& inst) {
		start(inst, "Track.Relay");

		PRINT_VALUE(x_offset);
		PRINT_VALUE(y_offset);
		PRINT_VALUE(yaw);
		PRINT_VALUE(pitch);
		PRINT_VALUE(roll);

		end();
	}
} // namespace bve::parsers::csv_rw_route
