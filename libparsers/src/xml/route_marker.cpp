#include "parsers/xml/route_marker.hpp"
#include "utils.hpp"
#include <map>
#include <rapidxml/rapidxml.hpp>
#include <sstream>
#include <string>

using namespace std::string_literals;

namespace parsers {
namespace xml {
	namespace route_marker {
		namespace {
			///////////////////////////////
			// Parsing a Text Color Node //
			///////////////////////////////

			text_marker::color parse_text_color(rapidxml::xml_node<char>* test_color_node) {
				static std::map<std::string, text_marker::color> text_mapping{
				    //
				    {"black", text_marker::color::Black},    {"gray", text_marker::color::Gray},
				    {"grey", text_marker::color::Gray},      {"white", text_marker::color::White},
				    {"red", text_marker::color::Red},        {"orange", text_marker::color::Orange},
				    {"green", text_marker::color::Green},    {"blue", text_marker::color::Blue},
				    {"magenta", text_marker::color::Magenta}
				    //
				};

				auto find_mapping_iter = text_mapping.find(
				    util::lower_copy(std::string(test_color_node->value(), test_color_node->value_size())));
				if (find_mapping_iter != text_mapping.end()) {
					return find_mapping_iter->second;
				}

				return text_marker::color::Black;
			}

			//////////////////////////////
			// Parsing Early/Late Nodes //
			//////////////////////////////

			template <bool early, bool text>
			auto parse_early_late_impl(rapidxml::xml_node<char>* start_node) {
				auto* time_node = start_node->first_node("time", 0, false);
				auto* data_node = start_node->first_node((text ? "text"s : "image"s).c_str(), 0, false);

				if (time_node == nullptr || data_node == nullptr) {
					std::string err;
					if (early) {
						if (text) {
							err = "XML node <Early> must have a <Time> AND a <Text> node."s;
						}
						else {
							err = "XML node <Early> must have a <Time> AND a <Image> node."s;
						}
					}
					else {
						if (text) {
							err = "XML node <Late> must have a <Time> AND a <Text> node."s;
						}
						else {
							err = "XML node <Late> must have a <Time> AND a <Image> node."s;
						}
					}

					throw std::invalid_argument(err);
				}

				if (text) {
					auto* text_color_node = start_node->first_node("color", 0, false);

					if (text_color_node != nullptr) {
						return std::make_tuple(util::parse_time(time_node->value()),
						                       std::string(data_node->name(), data_node->name_size()),
						                       parse_text_color(text_color_node));
					}
					return std::make_tuple(util::parse_time(time_node->value()),
					                       std::string(data_node->name(), data_node->name_size()),
					                       text_marker::color::Black);
				}
				return std::make_tuple(util::parse_time(time_node->value()),
				                       std::string(data_node->name(), data_node->name_size()),
				                       text_marker::color::Black);
			}

			auto parse_text_early(rapidxml::xml_node<char>* start_node) {
				return parse_early_late_impl<true, true>(start_node);
			}
			auto parse_text_late(rapidxml::xml_node<char>* start_node) {
				return parse_early_late_impl<false, true>(start_node);
			}

			auto parse_image_early(rapidxml::xml_node<char>* start_node) {
				return parse_early_late_impl<true, false>(start_node);
			}
			auto parse_image_late(rapidxml::xml_node<char>* start_node) {
				return parse_early_late_impl<false, false>(start_node);
			}

			//////////////////////////
			// Parsing OnTime Nodes //
			//////////////////////////

			template <bool text>
			std::string parse_on_time_impl(rapidxml::xml_node<char>* on_time_node) {
				auto* data_node = on_time_node->first_node((text ? "text"s : "image"s).c_str(), 0, false);

				if (data_node == nullptr) {
					std::string err;

					if (text) {
						err = "XML node <OnTime> must have a <Text> node."s;
					}
					else {
						err = "XML node <OnTime> must have a <Image> node."s;
					}

					throw std::invalid_argument(err);
				}

				return std::string(data_node->value(), data_node->value_size());
			}

			std::string parse_text_on_time(rapidxml::xml_node<char>* on_time_node) {
				return parse_on_time_impl<true>(on_time_node);
			}
			std::string parse_image_on_time(rapidxml::xml_node<char>* on_time_node) {
				return parse_on_time_impl<false>(on_time_node);
			}

			////////////////////////////
			// Parsing Distance Nodes //
			////////////////////////////

			float parse_distance(rapidxml::xml_node<char>* distance_node) {
				try {
					return util::parse_loose_float(std::string(distance_node->value(), distance_node->value_size()));
				}
				catch (const std::invalid_argument& e) {
					throw e;
				}
			}

			///////////////////////////
			// Parsing Timeout Nodes //
			///////////////////////////

			std::intmax_t parse_timeout(rapidxml::xml_node<char>* timeout_node) {
				try {
					return util::parse_loose_integer(std::string(timeout_node->value(), timeout_node->value_size()));
				}
				catch (const std::invalid_argument& e) {
					throw e;
				}
			}

			/////////////////////////
			// Parsing Train Nodes //
			/////////////////////////

			std::vector<std::string> parse_trains(rapidxml::xml_node<char>* train_node) {
				return util::split_text(std::string(train_node->value(), train_node->value_size()), ';', true);
			}

			image_marker parse_image_marker(const std::string& filename,
			                                rapidxml::xml_node<char>* start_node,
			                                const find_relative_file_func& get_relative_file) {
				image_marker marker;

				auto* early_node = start_node->first_node("early", 0, false);
				auto* on_time_node = start_node->first_node("ontime", 0, false);
				auto* late_node = start_node->first_node("late", 0, false);
				auto* distance_node = start_node->first_node("distance", 0, false);
				auto* timeout_node = start_node->first_node("timeout", 0, false);
				auto* trains_node = start_node->first_node("trains", 0, false);

				if (on_time_node == nullptr || (distance_node == nullptr && timeout_node == nullptr)) {
					throw std::invalid_argument(
					    "An <ImageMarker> section must have a <OnTime> and either a <Distance> or <Timeout>");
				}

				if (early_node != nullptr) {
					auto tuple = parse_image_early(early_node);
					marker.using_early = true;
					marker.early_time = std::get<0>(tuple);
					marker.early_filename = get_relative_file(filename, std::get<1>(tuple));
				}
				if (on_time_node != nullptr) {
					auto val = parse_image_on_time(on_time_node);
					marker.on_time_filename = get_relative_file(filename, val);
				}
				if (late_node != nullptr) {
					auto tuple = parse_image_late(late_node);
					marker.using_late = true;
					marker.late_time = std::get<0>(tuple);
					marker.late_filename = get_relative_file(filename, std::get<1>(tuple));
				}
				if (distance_node != nullptr) {
					marker.distance = parse_distance(distance_node);
				}
				if (timeout_node != nullptr) {
					marker.timeout = parse_timeout(timeout_node);
				}
				if (trains_node != nullptr) {
					marker.allowed_trains = parse_trains(trains_node);
				}

				return marker;
			}

			text_marker parse_text_marker(const std::string& filename,
			                              rapidxml::xml_node<char>* start_node,
			                              const find_relative_file_func& get_relative_file) {
				text_marker marker;

				auto* early_node = start_node->first_node("early", 0, false);
				auto* on_time_node = start_node->first_node("ontime", 0, false);
				auto* late_node = start_node->first_node("late", 0, false);
				auto* distance_node = start_node->first_node("distance", 0, false);
				auto* timeout_node = start_node->first_node("timeout", 0, false);
				auto* trains_node = start_node->first_node("trains", 0, false);

				if (on_time_node == nullptr || (distance_node == nullptr && timeout_node == nullptr)) {
					throw std::invalid_argument(
					    "An <TextMarker> section must have a <OnTime> and either a <Distance> or <Timeout>");
				}

				if (early_node != nullptr) {
					auto tuple = parse_text_early(early_node);
					marker.using_early = true;
					marker.early_time = std::get<0>(tuple);
					marker.early_text = get_relative_file(filename, std::get<1>(tuple));
					marker.early_color = std::get<2>(tuple);
				}
				if (on_time_node != nullptr) {
					auto val = parse_text_on_time(on_time_node);
					marker.on_time_text = get_relative_file(filename, val);
				}
				if (late_node != nullptr) {
					auto tuple = parse_text_late(late_node);
					marker.using_late = true;
					marker.late_time = std::get<0>(tuple);
					marker.late_text = get_relative_file(filename, std::get<1>(tuple));
					marker.late_color = std::get<2>(tuple);
				}
				if (distance_node != nullptr) {
					marker.distance = parse_distance(distance_node);
				}
				if (timeout_node != nullptr) {
					marker.timeout = parse_timeout(timeout_node);
				}
				if (trains_node != nullptr) {
					marker.allowed_trains = parse_trains(trains_node);
				}

				return marker;
			}
		} // namespace

		parsed_route_marker parse(const std::string& filename,
		                          std::string input_string,
		                          const find_relative_file_func& get_relative_file) {
			rapidxml::xml_document<> doc;
			doc.parse<rapidxml::parse_default>(&input_string[0]);

			// OpenBVE Node is Optional
			auto* openbve_node = doc.first_node("openbve", 0, false);
			rapidxml::xml_node<char>* primary_node;
			if (openbve_node != nullptr) {
				primary_node = openbve_node->first_node();
			}
			else {
				primary_node = doc.first_node();
			}

			auto primary_node_name = std::string(primary_node->name(), primary_node->name_size());
			auto primary_node_name_lower = util::lower_copy(primary_node_name);

			bool is_image_marker = primary_node_name == "imagemarker"s;
			bool is_text_marker = primary_node_name == "textmarker"s;

			if (!is_image_marker || !is_text_marker) {
				std::ostringstream err;

				err << "XML node named: " << primary_node_name << " is not a valid XML marker tag.";

				throw std::invalid_argument(err.str());
			}

			if (is_image_marker) {
				return parse_image_marker(filename, primary_node, get_relative_file);
			}

			return parse_text_marker(filename, primary_node, get_relative_file);
		}
	} // namespace route_marker
} // namespace xml
} // namespace parsers
