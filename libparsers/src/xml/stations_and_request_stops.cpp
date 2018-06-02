#include "parsers/xml/stations_and_request_stops.hpp"
#include "utils.hpp"
#include <map>
#include <rapidxml_ns.hpp>
#include <sstream>
#include <string>

using namespace std::string_literals;

namespace parsers {
namespace xml {
	namespace stations_and_stops {
		namespace {
			parsed_station_marker parse_station_marker(const std::string filename,
			                                           rapidxml_ns::xml_node<char>* start_node,
			                                           errors::multi_error_t errors) {
				station_marker sm;
				auto* name_node = start_node->first_node("name", 0, false);
				auto* arrival_time_node = start_node->first_node("arraivaltime",0, false);
				auto* departure_time_node = start_node->first_node("departuretime", 0, false);
				auto* doors_node = start_node->first_node("doors",0,false);
				auto* passenger_ratio_node = start_node->first_node("passengerratio", 0, false);

				return station_marker{};
			}
		} // namespace

		// Main parse function
		parsed_station_marker parse(const std::string& filename,
		                            std::string& input_string,
		                            errors::multi_error_t errors,
		                            const find_relative_file_func& get_relative_file) {
			rapidxml_ns::xml_document<> doc;
			doc.parse<rapidxml_ns::parse_default>(&input_string[0]);

			// OpenBVE node is optional
			auto* openbve_node = doc.first_node("openbve", 0, false);
			rapidxml_ns::xml_node<char>* primary_node;
			if (openbve_node != nullptr) {
				primary_node = openbve_node->first_node();
			}
			else {
				primary_node = doc.first_node();
			}
			auto const primary_node_name =
			    std::string(primary_node->name(), primary_node->name_size());
			auto const primary_node_lower_name = util::lower_copy(primary_node_name);

			auto const is_station_marker = primary_node_lower_name == "station"s;
			auto const is_request_stop = primary_node_lower_name == "requeststop"s;

			if (!is_request_stop && !is_station_marker) {
				std::ostringstream err;

				err << "XML node named: " << primary_node_name << " is not a valid XML marker tag.";
				add_error(errors, filename, 0, err.str());
			}

			if (is_station_marker) {
				return parse_station_marker(filename, primary_node, errors);
			}
		}
	} // namespace stations_and_stops
} // namespace xml
} // namespace parsers