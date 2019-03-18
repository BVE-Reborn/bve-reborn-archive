#include "common.hpp"

namespace bve::parsers::csv_rw_route {
	void CSVRwRouteInstructionIOClass::operator()(const instructions::track::Back& inst) {
		start(inst, "Track.Back");

		PRINT_VALUE(background_texture_index);

		end();
	}
	void CSVRwRouteInstructionIOClass::operator()(const instructions::track::Fog& inst) {
		start(inst, "Track.Fog");

		PRINT_VALUE(starting_distance);
		PRINT_VALUE(ending_distance);
		PRINT_COLOR(color);

		end();
	}
	void CSVRwRouteInstructionIOClass::operator()(const instructions::track::Brightness& inst) {
		start(inst, "Track.Brightness");

		PRINT_BYTE(value);

		end();
	}
	void CSVRwRouteInstructionIOClass::operator()(const instructions::track::Marker& inst) {
		start(inst, "Track.Marker");

		PRINT_VALUE(filename);
		PRINT_VALUE(distance);

		end();
	}
	void CSVRwRouteInstructionIOClass::operator()(const instructions::track::MarkerXML& inst) {
		start(inst, "Track.MarkerXML");

		PRINT_VALUE(filename);

		end();
	}
	void CSVRwRouteInstructionIOClass::operator()(const instructions::track::TextMarker& inst) {
		start(inst, "Track.TextMarker");

		PRINT_VALUE(text);
		PRINT_VALUE(distance);
		PRINT_ENUM8(font_color, track::TextMarker::FontColor, black, gray, white, red, orange, green, blue, magenta);

		end();
	}
	void CSVRwRouteInstructionIOClass::operator()(const instructions::track::PointOfInterest& inst) {
		start(inst, "Track.PointOfInterest");

		PRINT_VALUE(rail_index);
		PRINT_VALUE(x_offset);
		PRINT_VALUE(y_offset);
		PRINT_VALUE(yaw);
		PRINT_VALUE(pitch);
		PRINT_VALUE(roll);
		PRINT_VALUE(text);

		end();
	}
	void CSVRwRouteInstructionIOClass::operator()(const instructions::track::PreTrain& inst) {
		start(inst, "Track.PreTrain");

		PRINT_TIME(time)

		end();
	}
	void CSVRwRouteInstructionIOClass::operator()(const instructions::track::Announce& inst) {
		start(inst, "Track.Announce");

		PRINT_VALUE(filename);
		PRINT_VALUE(speed);

		end();
	}
	void CSVRwRouteInstructionIOClass::operator()(const instructions::track::Doppler& inst) {
		start(inst, "Track.Doppler");

		PRINT_VALUE(filename);
		PRINT_VALUE(x_offset);
		PRINT_VALUE(y_offset);

		end();
	}
	void CSVRwRouteInstructionIOClass::operator()(const instructions::track::Buffer& inst) {
		start(inst, "Track.Buffer");

		end();
	}
} // namespace bve::parsers::csv_rw_route
