#include "parsers/xml/stations_and_request_stops.hpp"
#include "utils.hpp"
#include <gsl/gsl_util>
#include <rapidxml_ns.hpp>
#include <sstream>
#include <string>
#include <unordered_map>

using namespace std::string_literals;

namespace parsers {
namespace xml {
	namespace stations_and_stops {
		namespace {
			station_marker::doors parse_doors_text(std::string const& filename, rapidxml_ns::xml_node<char>* doors_node, errors::multi_error_t& errors) {
				static std::unordered_map<std::string, station_marker::doors> text_mapping{
				    //
				    {"left", station_marker::doors::left},
				    {"right", station_marker::doors::right},
				    {"none", station_marker::doors::none}
				    //
				};
				std::string door;
				try{
					door = doors_node->value();
					util::lower(door);
				}
				catch (std::exception const& e){
					add_error(errors, filename, 0 , e.what());
					door = "none";
				}
				auto const parsed_doors =
				    text_mapping.find(door);

				if (parsed_doors != text_mapping.end())
					return parsed_doors->second;

				add_error(errors, filename, 0, "Error: <Doors> given and invalid option");
				return station_marker::doors::none;
			}
			request_stop_marker::behaviour parse_ai_behaviour( std::string const& filename,
			    rapidxml_ns::xml_node<char>* ai_behaviour_node, errors::multi_error_t& errors) {
				static std::unordered_map<std::string, request_stop_marker::behaviour>
				    text_mapping{{"fullspeed", request_stop_marker::behaviour::fullspeed},
				                 {"normalbrake", request_stop_marker::behaviour::normalbrake}};
				std::string behaviour;
				try{
					behaviour = ai_behaviour_node->value();
					util::lower(behaviour);
				}
				catch (std::exception const& e){
					add_error(errors, filename, 0, e.what());
					behaviour = "normalbrake"s;
				}
				auto const parsed_behaviour = text_mapping.find(behaviour);

				if (parsed_behaviour != text_mapping.end())
					return parsed_behaviour->second;

				add_error(errors, filename, 0 , "Error: <AIBehaviour> given invalid behaviour");
				return request_stop_marker::behaviour::normalbrake;
			}

			sub_message parse_sub_messages_nodes(rapidxml_ns::xml_node<char>* parent_node){
				auto* early_node = parent_node->first_node("early", 0, false);
				auto* on_time_node = parent_node->first_node("ontime", 0, false);
				auto* late_node = parent_node->first_node("late", 0, false);
				sub_message sub_nodes;

				// There are no sub nodes so return a struct with values of "parent_node".
				if(early_node == nullptr && on_time_node == nullptr && late_node == nullptr){
					sub_nodes.early = parent_node->value();
					sub_nodes.ontime = parent_node->value();
					sub_nodes.late = parent_node->value();
					return sub_nodes;
				}

				if(early_node != nullptr){
					sub_nodes.early = early_node->value();
				}
				else{
					sub_nodes.early = std::string{};
				}

				if(on_time_node != nullptr){
					sub_nodes.ontime = on_time_node->value();
				}
				else{
					sub_nodes.ontime = std::string{};
				}

				if(late_node != nullptr){
					sub_nodes.late = late_node->value();
				}
				else{
					sub_nodes.late = std::string{};
				}

				return sub_nodes;
			}

			probabilities parse_probabilities_nodes(std::string const& filename, rapidxml_ns::xml_node<char>* parent_node, errors::multi_error_t& errors){
				auto* early_node = parent_node->first_node("early", 0, false);
				auto* on_time_node = parent_node->first_node("ontime", 0, false);
				auto* late_node = parent_node->first_node("late", 0, false);
				probabilities sub_nodes;

				// There are no sub nodes so return a struct with values of "parent_node".
				if(early_node == nullptr && on_time_node == nullptr && late_node == nullptr){

					std::intmax_t no_sub_values;
					try{
						no_sub_values = util::parse_loose_integer(parent_node->value());

					}
					catch (std::exception const& e){
						no_sub_values = 0;
						add_error(errors,filename, 0, e.what());
					}
					if (no_sub_values< 0 || no_sub_values > 100) {
						add_error(errors, filename, 0,
								  "<Probability> has to be between 0 and 100 but found" + no_sub_values);
						no_sub_values = 0;
					}
					sub_nodes.early = gsl::narrow<std::uint8_t >(no_sub_values);
					sub_nodes.ontime = gsl::narrow<std::uint8_t >(no_sub_values);
					sub_nodes.late = gsl::narrow<std::uint8_t >(no_sub_values);
					return sub_nodes;
				}

				if(early_node != nullptr){
				    std::intmax_t prob;
					try {
						prob = util::parse_loose_integer(early_node->value());
					}
					catch (std::exception const& e){
						add_error(errors, filename, 0, e.what());
						prob = 0;
					}

					if (prob < 0 || prob > 100) {
						add_error(errors, filename, 0,
								  "<Probability> has to be between 0 and 100 but found" + prob);
						prob = 0;
					}
					sub_nodes.early = gsl::narrow<std::uint8_t >(prob);
				}
				else{
					sub_nodes.early = std::uint8_t{};
				}

				if(on_time_node != nullptr){
					std::intmax_t prob;
					try {
						prob = util::parse_loose_integer(on_time_node->value());
					}
					catch (std::exception const& e){
						add_error(errors, filename, 0, e.what());
						prob = 0;
					}

					if (prob < 0 || prob > 100) {
						add_error(errors, filename, 0,
								  "<Probability> has to be between 0 and 100 but found" + prob);
						prob = 0;
					}
					sub_nodes.ontime = gsl::narrow<std::uint8_t >(prob);
				}
				else{
					sub_nodes.ontime = std::uint8_t {};
				}

				if(late_node != nullptr){
					std::intmax_t prob;
					try {
						prob = util::parse_loose_integer(late_node->value());
					}
					catch (std::exception const& e){
						add_error(errors, filename, 0, e.what());
						prob = 0;
					}

					if (prob < 0 || prob > 100) {
						add_error(errors, filename, 0,
								  "<Probability> has to be between 0 and 100 but found" + prob);
						prob = 0;
					}
					sub_nodes.late = gsl::narrow<std::uint8_t >(prob);
				}
				else{
					sub_nodes.late = std::uint8_t {};
				}

				return sub_nodes;
			}
			//////////////////////////////////
			////PARSE REQUEST STOP MARKER////
			//////////////////////////////////
			request_stop_marker parse_request_stop_marker(
					const std::string filename,
					rapidxml_ns::xml_node<char>* start_node,
					errors::multi_error_t& errors,
					const find_relative_file_func& /*get_relative_file*/) {
				request_stop_marker rs;

				auto* early_time_node = start_node->first_node("earlytime", 0, false);
				auto* late_time_node = start_node->first_node("latetime", 0, false);
				auto* distance_node = start_node->first_node("distance", 0, false);
				auto* stop_message_node = start_node->first_node("stopmessage", 0, false);
				auto* pass_message_node = start_node->first_node("passmessage", 0, false);
				auto* probability_node = start_node->first_node("probability", 0, false);
				auto* max_cars_node = start_node->first_node("maxcars", 0, false);
				auto* ai_behaviour_node = start_node->first_node("aibehaviour", 0, false);

				if (early_time_node != nullptr) {
					try {
						rs.early_time = util::parse_time(early_time_node->value());
					}
					catch (std::exception const& e) {
						add_error(errors, filename, 0, e.what());
						rs.early_time = 0;
					}
				}

				if (late_time_node != nullptr) {
					try {
						rs.late_time = util::parse_time(late_time_node->value());
					}
					catch (std::exception const& e) {
						add_error(errors, filename, 0, e.what());
						rs.late_time = 0;
					}
				}

				if (distance_node != nullptr) {
					try {
						rs.distance = util::parse_loose_float(distance_node->value());
					}
					catch (std::exception const& e) {
						add_error(errors, filename, 0, e.what());
						rs.distance = 0;
					}
				}

				if (stop_message_node != nullptr) {
					rs.stop_message = parse_sub_messages_nodes(stop_message_node);
				}

				if (pass_message_node != nullptr) {
					rs.pass_message = parse_sub_messages_nodes(pass_message_node);
				}

				if (probability_node != nullptr) {
					rs.probability = parse_probabilities_nodes(filename, probability_node, errors);
				}

				if (max_cars_node != nullptr) {
					try {
						auto cars = util::parse_loose_integer(max_cars_node->value());
						if(cars < 0){
							add_error(errors, filename, 0 ,"Error: <MaxCars> should have be a non negative intger");
							cars = 0;
						}
						rs.max_cars = cars;

					}
					catch (std::exception const& e) {
						add_error(errors, filename, 0, e.what());
						rs.max_cars = 0;
					}
				}

				if (ai_behaviour_node != nullptr) {
					rs.ai_behaviour = parse_ai_behaviour(filename, ai_behaviour_node, errors);
				}
				return rs;
			}
		} // namespace

			/////////////////////////////
			//// PARSE STATION MARKER////
			/////////////////////////////
			parsed_station_marker parse_station_marker(
			    const std::string filename,
			    rapidxml_ns::xml_node<char>* start_node,
			    errors::multi_error_t& errors,
			    const find_relative_file_func& get_relative_file) {
				station_marker sm;
				auto* name_node = start_node->first_node("name", 0, false);
				auto* arrival_time_node = start_node->first_node("arrivaltime", 0, false);
				auto* departure_time_node = start_node->first_node("departuretime", 0, false);
				auto* doors_node = start_node->first_node("doors", 0, false);
				auto* red_signal_node = start_node->first_node("forcedredsignal",0,false);
				auto* passenger_ratio_node = start_node->first_node("passengerratio", 0, false);
				auto* arrival_sound_node = start_node->first_node("arrivalsound", 0, false);
				auto* departure_sound_node = start_node->first_node("depaturesound", 0, false);
				auto* stop_duration_node = start_node->first_node("stopduration", 0, false);
				auto* time_table_index_node = start_node->first_node("timetableindex", 0, false);
				auto* request_stop_node = start_node->first_node("requeststop", 0, false);

				if (name_node != nullptr) {
					sm.station_name = std::string(name_node->value(), name_node->value_size());
				}

				if (arrival_time_node != nullptr) {
					try {
						sm.arrival_time = util::parse_time(arrival_time_node->value());
					}
					catch (std::exception const& e) {
						sm.arrival_time = 0;
						add_error(errors, filename, 0, e.what());
					}
				}

				if (departure_time_node != nullptr) {
					try {
						sm.departure_time = util::parse_time(departure_time_node->value());
					}
					catch (std::exception const& e) {
						sm.departure_time = 0;
						add_error(errors, filename, 0, e.what());
					}
				}

				if (doors_node != nullptr) {
					sm.door = parse_doors_text(filename, doors_node, errors);
				}

				if(red_signal_node != nullptr){
					std::string red_signal = red_signal_node->value();
					util::lower(red_signal);
					sm.force_red_signal = "true"s == red_signal;
				}

				if (passenger_ratio_node != nullptr) {
					try {
						auto ratio = util::parse_loose_integer(passenger_ratio_node->value());
						if(ratio < 0 || ratio > 250){
							add_error(errors, filename, 0, "Out of bounds Error: PassengerRatio has to be a integer between 0 and 250.");
							ratio = 0;
						}
						sm.passenger_ratio = gsl::narrow<std::uint8_t>(ratio);
					}
					catch (std::exception const& e) {
						sm.passenger_ratio = 0;
						add_error(errors, filename, 0, e.what());
					}
				}

				if (arrival_sound_node != nullptr) {
					sm.arrival_sound_file =
					    get_relative_file(filename, arrival_sound_node->value());
				}

				if (departure_sound_node != nullptr) {
					sm.departure_sound_file =
					    get_relative_file(filename, departure_sound_node->value());
				}

				if (stop_duration_node != nullptr) {
					try {
						sm.stop_duration =
						    util::parse_loose_integer(stop_duration_node->value(), 15);
					}
					catch (std::exception const& e) {
						sm.stop_duration = 0;
						add_error(errors, filename, 0, e.what());
					}
				}
				if (time_table_index_node != nullptr) {
					auto index = util::parse_loose_integer((time_table_index_node->value()));
					if (index < 0) {
						add_error(errors, filename, 0,
						          "<TimeTableIndex> should have non negative values"s);
						index = 0;
					}

					sm.time_table_index = gsl::narrow<std::uintmax_t>(index);
				}
				if(request_stop_node != nullptr){
					stations_and_stops::request_stop_marker rs = parse_request_stop_marker(filename, request_stop_node, errors, get_relative_file);
					sm.request_stop = rs;
				}
				return sm;
			}
		// Main parse function
		parsed_station_marker parse(const std::string& filename,
		                            std::string& input_string,
		                            errors::multi_error_t& errors,
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
				return parse_station_marker(filename, primary_node, errors, get_relative_file);
			}
			return parse_request_stop_marker(filename, primary_node, errors, get_relative_file);
		}
	} // namespace stations_and_stops
} // namespace xml
} // namespace parsers