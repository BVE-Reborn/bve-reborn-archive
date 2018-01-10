#include "../executor_pass3.hpp"

namespace parsers {
namespace csv_rw_route {
	void pass3_executor::operator()(const instructions::track::Back& inst) {}
	void pass3_executor::operator()(const instructions::track::Fog& inst) {}
	void pass3_executor::operator()(const instructions::track::Brightness& inst) {}
	void pass3_executor::operator()(const instructions::track::Marker& inst) {}
	void pass3_executor::operator()(const instructions::track::MarkerXML& inst) {}
	void pass3_executor::operator()(const instructions::track::TextMarker& inst) {}
	void pass3_executor::operator()(const instructions::track::PointOfInterest& inst) {}
	void pass3_executor::operator()(const instructions::track::PreTrain& inst) {}
	void pass3_executor::operator()(const instructions::track::Announce& inst) {}
	void pass3_executor::operator()(const instructions::track::Doppler& inst) {}
	void pass3_executor::operator()(const instructions::track::Buffer& inst) {}
} // namespace csv_rw_route
} // namespace parsers
