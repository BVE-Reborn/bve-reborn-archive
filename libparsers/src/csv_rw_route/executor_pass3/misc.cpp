#include "../executor_pass3.hpp"

namespace parsers {
namespace csv_rw_route {
	void pass3_executor::operator()(const instructions::track::Back& instr) {}
	void pass3_executor::operator()(const instructions::track::Fog& instr) {}
	void pass3_executor::operator()(const instructions::track::Brightness& instr) {}
	void pass3_executor::operator()(const instructions::track::Marker& instr) {}
	void pass3_executor::operator()(const instructions::track::MarkerXML& instr) {}
	void pass3_executor::operator()(const instructions::track::TextMarker& instr) {}
	void pass3_executor::operator()(const instructions::track::PointOfInterest& instr) {}
	void pass3_executor::operator()(const instructions::track::PreTrain& instr) {}
	void pass3_executor::operator()(const instructions::track::Announce& instr) {}
	void pass3_executor::operator()(const instructions::track::Doppler& instr) {}
	void pass3_executor::operator()(const instructions::track::Buffer& instr) {}
} // namespace csv_rw_route
} // namespace parsers
