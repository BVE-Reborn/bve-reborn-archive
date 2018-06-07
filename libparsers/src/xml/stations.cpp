#include "parsers/xml/stations.hpp"
#include "utils.hpp"
#include <gsl/gsl_util>
#include <rapidxml_ns.hpp>
#include <sstream>
#include <string>
#include <unordered_map>

using namespace std::string_literals;

namespace parsers {
namespace xml {
	namespace stations {
		namespace {
			parsed_station_marker::doors parse_doors_text(std::string const& filename,
			                                              rapidxml_ns::xml_node<char>* doors_node,
			                                              errors::multi_error_t& errors) {
				static std::unordered_map<std::string, parsed_station_marker::doors> text_mapping{
				    //
				    {"left", parsed_station_marker::doors::left},
				    {"right", parsed_station_marker::doors::right},
				    {"none", parsed_station_marker::doors::none}
				    //
				};
				std::string door;
				try {
					door = util::lower_copy(
					    std::string(doors_node->value(), doors_node->value_size()));
				}
				catch (std::exception const& e) {
					add_error(errors, filename, 0, e.what());
					door = "none"s;
				}
				auto const parsed_doors = text_mapping.find(door);

				if (parsed_doors != text_mapping.end()) {
					return parsed_doors->second;
				}
				add_error(errors, filename, 0, "Error: <Doors> given an invalid option");
				return parsed_station_marker::doors::none;
			}

			request_stop_marker::behaviour parse_ai_behaviour(
			    std::string const& filename,
			    rapidxml_ns::xml_node<char>* ai_behaviour_node,
			    errors::multi_error_t& errors) {
				static std::unordered_map<std::string, request_stop_marker::behaviour> text_mapping{
				    //
				    {"fullspeed", request_stop_marker::behaviour::fullspeed},
				    {"normalbrake", request_stop_marker::behaviour::normalbrake}
				    //
				};
				std::string behaviour;
				try {
					behaviour = util::lower_copy(
					    std::string(ai_behaviour_node->value(), ai_behaviour_node->value_size()));
				}
				catch (std::exception const& e) {
					add_error(errors, filename, 0, e.what());
					behaviour = "normalbrake"s;
				}
				auto const parsed_behaviour = text_mapping.find(behaviour);

				if (parsed_behaviour != text_mapping.end()) {
					return parsed_behaviour->second;
				}

				add_error(errors, filename, 0, "Error: <AIBehaviour> given invalid behaviour");
				return request_stop_marker::behaviour::normalbrake;
			}

			message parse_messages_nodes(rapidxml_ns::xml_node<char>* parent_node) {
				auto* early_node = parent_node->first_node("early", 0, false);
				auto* on_time_node = parent_node->first_node("ontime", 0, false);
				auto* late_node = parent_node->first_node("late", 0, false);
				message sub_nodes;

				// There are no sub nodes so return a struct with values of "parent_node".
				if (early_node == nullptr && on_time_node == nullptr && late_node == nullptr) {
					sub_nodes.early = std::string(parent_node->value(), parent_node->value_size());
					sub_nodes.ontime = std::string(parent_node->value(), parent_node->value_size());
					sub_nodes.late = std::string(parent_node->value(), parent_node->value_size());
					return sub_nodes;
				}

				if (early_node != nullptr) {
					sub_nodes.early = std::string(early_node->value(), early_node->value_size());
				}
				else {
					sub_nodes.early = std::string{};
				}

				if (on_time_node != nullptr) {
					sub_nodes.ontime =
					    std::string(on_time_node->value(), on_time_node->value_size());
				}
				else {
					sub_nodes.ontime = std::string{};
				}

				if (late_node != nullptr) {
					sub_nodes.late = std::string(late_node->value(), late_node->value_size());
				}
				else {
					sub_nodes.late = std::string{};
				}

				return sub_nodes;
			}

			probabilities parse_probabilities_nodes(std::string const& filename,
			                                        rapidxml_ns::xml_node<char>* parent_node,
			                                        errors::multi_error_t& errors) {
				auto* early_node = parent_node->first_node("early", 0, false);
				auto* on_time_node = parent_node->first_node("ontime", 0, false);
				auto* late_node = parent_node->first_node("late", 0, false);
				probabilities sub_nodes;

				// There are no sub nodes so return a struct with values of "parent_node".
				if (early_node == nullptr && on_time_node == nullptr && late_node == nullptr) {
					std::intmax_t no_sub_values;
					try {
						no_sub_values = util::parse_loose_integer(
						    std::string(parent_node->value(), parent_node->value_size()));
					}
					catch (std::exception const& e) {
						no_sub_values = 0;
						add_error(errors, filename, 0, e.what());
					}
					if (no_sub_values < 0 || no_sub_values > 100) {
						std::ostringstream err;

						err << "<Probability> has to be between 0 and 100 but found"
						    << no_sub_values;
						add_error(errors, filename, 0, err.str());
						no_sub_values = 0;
					}
					sub_nodes.early = gsl::narrow_cast<std::uint8_t>(no_sub_values);
					sub_nodes.ontime = gsl::narrow_cast<std::uint8_t>(no_sub_values);
					sub_nodes.late = gsl::narrow_cast<std::uint8_t>(no_sub_values);
					return sub_nodes;
				}
				// Sets the probabilities.
				auto set_prob = [&](rapidxml_ns::xml_node<char> const* const node,
				                    std::uint8_t& prob, std::string const& time) {
					if (node != nullptr) {
						std::intmax_t temp_prob;
						try {
							temp_prob = util::parse_loose_integer(
							    std::string(node->value(), node->value_size()));
						}
						catch (std::exception const& e) {
							add_error(errors, filename, 0, e.what());
							temp_prob = 0;
						}

						if (temp_prob < 0 || temp_prob > 100) {
							std::ostringstream err;
							err << time << "has to be between 0 and 100 but found" << temp_prob;
							add_error(errors, filename, 0, err.str());
							temp_prob = 0;
						}
						prob = gsl::narrow_cast<std::uint8_t>(temp_prob);
					}
					else {
						prob = 0;
					}
				}; // End of set_prob.

				set_prob(early_node, sub_nodes.early, "<Early>"s);
				set_prob(on_time_node, sub_nodes.ontime, "<OnTime>"s);
				set_prob(late_node, sub_nodes.late, "<Late>"s);

				return sub_nodes;
			}

			//////////////////////////////////
			////PARSE REQUEST STOP MARKER////
			//////////////////////////////////
			request_stop_marker parse_request_stop_marker(
			    std::string const& filename,
			    rapidxml_ns::xml_node<char>* start_node,
			    errors::multi_error_t& errors,
			    find_relative_file_func const& /*get_relative_file*/) {
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
						rs.early_time = util::parse_time(
						    std::string(early_time_node->value(), early_time_node->value_size()));
						rs.using_early = true;
					}
					catch (std::exception const& e) {
						add_error(errors, filename, 0, e.what());
						rs.early_time = 0;
						rs.using_early = false;
					}
				}

				if (late_time_node != nullptr) {
					try {
						rs.late_time = util::parse_time(
						    std::string(late_time_node->value(), late_time_node->value_size()));
						rs.using_late = true;
					}
					catch (std::exception const& e) {
						add_error(errors, filename, 0, e.what());
						rs.late_time = 0;
						rs.using_late = false;
					}
				}

				if (distance_node != nullptr) {
					try {
						rs.distance = util::parse_loose_float(
						    std::string(distance_node->value(), distance_node->value_size()));
					}
					catch (std::exception const& e) {
						add_error(errors, filename, 0, e.what());
						rs.distance = 0;
					}
				}

				if (stop_message_node != nullptr) {
					rs.stop_message = parse_messages_nodes(stop_message_node);
				}

				if (pass_message_node != nullptr) {
					rs.pass_message = parse_messages_nodes(pass_message_node);
				}

				if (probability_node != nullptr) {
					rs.probability = parse_probabilities_nodes(filename, probability_node, errors);
				}

				if (max_cars_node != nullptr) {
					try {
						auto cars = util::parse_loose_integer(
						    std::string(max_cars_node->value(), max_cars_node->value_size()));
						if (cars < 0) {
							add_error(errors, filename, 0,
							          "Error: <MaxCars> should have be a non negative intger");
							cars = 0;
						}
						rs.max_cars = gsl::narrow<std::uint8_t>(cars);
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
		    std::string const& filename,
		    rapidxml_ns::xml_node<char>* start_node,
		    errors::multi_error_t& errors,
		    find_relative_file_func const& get_relative_file) {
			parsed_station_marker sm;
			auto* name_node = start_node->first_node("name", 0, false);
			auto* arrival_time_node = start_node->first_node("arrivaltime", 0, false);
			auto* departure_time_node = start_node->first_node("departuretime", 0, false);
			auto* doors_node = start_node->first_node("doors", 0, false);
			auto* red_signal_node = start_node->first_node("forcedredsignal", 0, false);
			auto* passenger_ratio_node = start_node->first_node("passengerratio", 0, false);
			auto* arrival_sound_node = start_node->first_node("arrivalsound", 0, false);
			auto* departure_sound_node = start_node->first_node("departuresound", 0, false);
			auto* stop_duration_node = start_node->first_node("stopduration", 0, false);
			auto* time_table_index_node = start_node->first_node("timetableindex", 0, false);
			auto* request_stop_node = start_node->first_node("requeststop", 0, false);

			if (name_node != nullptr) {
				sm.station_name = std::string(name_node->value(), name_node->value_size());
			}

			if (arrival_time_node != nullptr) {
				try {
					sm.arrival_time = util::parse_time(
					    std::string(arrival_time_node->value(), arrival_time_node->value_size()));
				}
				catch (std::exception const& e) {
					sm.arrival_time = 0;
					add_error(errors, filename, 0, e.what());
				}
			}

			if (departure_time_node != nullptr) {
				try {
					sm.departure_time =
					    util::parse_time(std::string(departure_time_node->value(),
					                                 departure_time_node->value_size()));
				}
				catch (std::exception const& e) {
					sm.departure_time = 0;
					add_error(errors, filename, 0, e.what());
				}
			}

			if (doors_node != nullptr) {
				sm.door = parse_doors_text(filename, doors_node, errors);
			}

			if (red_signal_node != nullptr) {
				std::string red_signal = util::lower_copy(red_signal_node->value());
				sm.force_red_signal = "true"s == red_signal;
			}

			if (passenger_ratio_node != nullptr) {
				try {
					auto ratio =
					    util::parse_loose_integer(std::string(passenger_ratio_node->value(),
					                                          passenger_ratio_node->value_size()));
					if (ratio < 0 || ratio > 250) {
						add_error(
						    errors, filename, 0,
						    "Out of bounds Error: PassengerRatio has to be a integer between 0 and 250."s);
						ratio = 0;
					}
					sm.passenger_ratio = gsl::narrow_cast<std::uint8_t>(ratio);
				}
				catch (std::exception const& e) {
					sm.passenger_ratio = 0;
					add_error(errors, filename, 0, e.what());
				}
			}

			if (arrival_sound_node != nullptr) {
				sm.arrival_sound_file =
				    get_relative_file(filename, std::string(arrival_sound_node->value(),
				                                            arrival_sound_node->value_size()));
			}

			if (departure_sound_node != nullptr) {
				sm.departure_sound_file =
				    get_relative_file(filename, std::string(departure_sound_node->value(),
				                                            departure_sound_node->value_size()));
			}

			if (stop_duration_node != nullptr) {
				try {
					auto duration =
					    util::parse_loose_integer(std::string(stop_duration_node->value(),
					                                          stop_duration_node->value_size()),
					                              15);
					sm.stop_duration = gsl::narrow<std::uintmax_t>(duration);
				}
				catch (std::exception const& e) {
					sm.stop_duration = 0;
					add_error(errors, filename, 0, e.what());
				}
			}
			if (time_table_index_node != nullptr) {
				auto index =
				    util::parse_loose_integer(std::string(time_table_index_node->value(),
				                                          time_table_index_node->value_size()));
				if (index < 0) {
					add_error(errors, filename, 0,
					          "<TimeTableIndex> should have non negative values"s);
					index = 0;
				}

				sm.time_table_index = gsl::narrow_cast<std::size_t>(index);
			}
			if (request_stop_node != nullptr) {
				stations::request_stop_marker rs =
				    parse_request_stop_marker(filename, request_stop_node, errors,
				                              get_relative_file);
				sm.request_stop = rs;
			}
			return sm;
		}

		// Main parse function
		parsed_station_marker parse(std::string const& filename,
		                            std::string& input_string,
		                            errors::multi_error_t& errors,
		                            find_relative_file_func const& get_relative_file) {
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

			if (primary_node->name() == "Station"s) {
				return parse_station_marker(filename, primary_node, errors, get_relative_file);
			}
			std::ostringstream err;

			err << "XML node named: " << primary_node->value() << " is not a valid XML marker tag.";
			add_error(errors, filename, 0, err.str());
			return parsed_station_marker{};
		}
	} // namespace stations
} // namespace xml
} // namespace parsers
