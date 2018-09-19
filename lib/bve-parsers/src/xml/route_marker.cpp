#include "parsers/xml/route_marker.hpp"
#include "utils.hpp"
#include "xml_node_helpers.hpp"
#include <map>
#include <rapidxml_ns.hpp>
#include <sstream>
#include <string>
using namespace std::string_literals;

namespace bve::parsers::xml::route_marker {
	namespace {
		///////////////////////////////
		// Parsing a Text Color Node //
		///////////////////////////////

		text_marker::color parse_text_color(rapidxml_ns::xml_node<char>* test_color_node,
		                                    errors::Errors& /*errors*/) {
			static std::map<std::string, text_marker::color> text_mapping{
			    //
			    {"black", text_marker::color::black},    {"gray", text_marker::color::gray},
			    {"grey", text_marker::color::gray},      {"white", text_marker::color::white},
			    {"red", text_marker::color::red},        {"orange", text_marker::color::orange},
			    {"green", text_marker::color::green},    {"blue", text_marker::color::blue},
			    {"magenta", text_marker::color::magenta}
			    //
			};

			auto const find_mapping_iter =
			    text_mapping.find(util::lower_copy(get_node_value(test_color_node)));
			if (find_mapping_iter != text_mapping.end()) {
				return find_mapping_iter->second;
			}

			return text_marker::color::black;
		}

		//////////////////////////////
		// Parsing Early/Late Nodes //
		//////////////////////////////

		template <bool Early, bool Text>
		auto parse_early_late_impl(rapidxml_ns::xml_node<char>* start_node,
		                           errors::Errors& errors) {
			auto* time_node = start_node->first_node("time", 0, false);
			auto const data_node = start_node->first_node((Text ? "text"s : "image"s).c_str(), 0, false);

			if (time_node == nullptr || data_node == nullptr) {
				std::string err;
				if (Early) {
					if (Text) {
						err = "XML node <Early> must have a <Time> AND a <Text> node."s;
					}
					else {
						err = "XML node <Early> must have a <Time> AND a <Image> node."s;
					}
				}
				else {
					if (Text) {
						err = "XML node <Late> must have a <Time> AND a <Text> node."s;
					}
					else {
						err = "XML node <Late> must have a <Time> AND a <Image> node."s;
					}
				}
				add_error(errors, 0, err);
				return std::make_tuple(bve::core::datatypes::time{0}, ""s,
				                       text_marker::color::black, false);
			}
			bve::core::datatypes::time time_parsed;
			bool using_early_late = true;
			try {
				time_parsed = util::parse_time(get_node_value(time_node));
			}
			catch (const std::exception& e) {
				add_error(errors, 0, e.what());
				time_parsed = 0;
				using_early_late = false;
			}
			if (Text) {
				auto* text_color_node = start_node->first_node("color", 0, false);

				if (text_color_node != nullptr) {
					return std::make_tuple(time_parsed, get_node_value(data_node),
					                       parse_text_color(text_color_node, errors),
					                       using_early_late);
				}
				return std::make_tuple(time_parsed, get_node_value(data_node),
				                       text_marker::color::black, using_early_late);
			}
			return std::make_tuple(time_parsed, get_node_value(data_node),
			                       text_marker::color::black, using_early_late);
		}

		auto parse_text_early(rapidxml_ns::xml_node<char>* start_node, errors::Errors& errors) {
			return parse_early_late_impl<true, true>(start_node, errors);
		}
		auto parse_text_late(rapidxml_ns::xml_node<char>* start_node, errors::Errors& errors) {
			return parse_early_late_impl<false, true>(start_node, errors);
		}

		auto parse_image_early(rapidxml_ns::xml_node<char>* start_node, errors::Errors& errors) {
			return parse_early_late_impl<true, false>(start_node, errors);
		}
		auto parse_image_late(rapidxml_ns::xml_node<char>* start_node, errors::Errors& errors) {
			return parse_early_late_impl<false, false>(start_node, errors);
		}

		//////////////////////////
		// Parsing OnTime Nodes //
		//////////////////////////

		template <bool Text>
		auto parse_on_time_impl(rapidxml_ns::xml_node<char>* on_time_node,
		                        errors::Errors& errors) {
			auto const data_node =
			    on_time_node->first_node((Text ? "text"s : "image"s).c_str(), 0, false);

			if (data_node == nullptr) {
				std::string err;

				if (Text) {
					err = "XML node <OnTime> must have a <Text> node."s;
				}
				else {
					err = "XML node <OnTime> must have a <Image> node."s;
				}
				add_error(errors, 0, err);
				return std::make_tuple(std::string{}, false);
			}

			return std::make_tuple(get_node_value(data_node), true);
		}

		auto parse_text_on_time(rapidxml_ns::xml_node<char>* on_time_node,
		                        errors::Errors& errors) {
			return parse_on_time_impl<true>(on_time_node, errors);
		}
		auto parse_image_on_time(rapidxml_ns::xml_node<char>* on_time_node,
		                         errors::Errors& errors) {
			return parse_on_time_impl<false>(on_time_node, errors);
		}

		////////////////////////////
		// Parsing Distance Nodes //
		////////////////////////////

		float parse_distance(rapidxml_ns::xml_node<char>* distance_node, errors::Errors& errors) {
			try {
				return util::parse_loose_float(get_node_value(distance_node));
			}
			catch (const std::exception& e) {
				add_error(errors, 0, e.what());
				return 0;
			}
		}

		///////////////////////////
		// Parsing Timeout Nodes //
		///////////////////////////

		std::intmax_t parse_timeout(rapidxml_ns::xml_node<char>* timeout_node,
		                            errors::Errors& errors) {
			try {
				auto const time = util::parse_loose_integer(get_node_value(timeout_node));
				if (time < 0) {
					add_error(errors, 0, "Timeout node should not have a negative time"s);
					return 0;
				}
				return time;
			}
			catch (const std::exception& e) {
				add_error(errors, 0, e.what());
				return 0;
			}
		}

		/////////////////////////
		// Parsing Train Nodes //
		/////////////////////////

		std::vector<std::string> parse_trains(rapidxml_ns::xml_node<char>* train_node,
		                                      errors::Errors& /*errors*/) {
			return util::split_text(get_node_value(train_node), ';', true);
		}

		image_marker parse_image_marker(const std::string& filename,
		                                rapidxml_ns::xml_node<char>* start_node,
		                                errors::MultiError& errors,
		                                const find_relative_file_func& get_relative_file) {
			image_marker marker;

			auto& err = errors[filename];
			auto* early_node = start_node->first_node("early", 0, false);
			auto* on_time_node = start_node->first_node("ontime", 0, false);
			auto* late_node = start_node->first_node("late", 0, false);
			auto* distance_node = start_node->first_node("distance", 0, false);
			auto* timeout_node = start_node->first_node("timeout", 0, false);
			auto* trains_node = start_node->first_node("trains", 0, false);

			if (on_time_node == nullptr || (distance_node == nullptr && timeout_node == nullptr)) {
				add_error(errors, filename, 0,
				          "An <ImageMarker> section must have a <OnTime> and "
				          "either a <Distance> or <Timeout>"s);
			}

			if (early_node != nullptr) {
				auto tuple = parse_image_early(early_node, err);
				marker.early_time = std::get<0>(tuple);
				marker.early_filename = get_relative_file(filename, std::get<1>(tuple));
				marker.using_early = std::get<3>(tuple);
			}
			if (on_time_node != nullptr) {
				auto const tuple = parse_image_on_time(on_time_node, err);
				marker.using_ontime = std::get<1>(tuple);
				marker.on_time_filename = get_relative_file(filename, std::get<0>(tuple));
			}
			if (late_node != nullptr) {
				auto tuple = parse_image_late(late_node, err);
				marker.late_time = std::get<0>(tuple);
				marker.late_filename = get_relative_file(filename, std::get<1>(tuple));
				marker.using_late = std::get<3>(tuple);
			}
			if (distance_node != nullptr) {
				marker.distance = parse_distance(distance_node, err);
			}
			if (timeout_node != nullptr) {
				marker.timeout = parse_timeout(timeout_node, err);
			}
			if (trains_node != nullptr) {
				marker.allowed_trains = parse_trains(trains_node, err);
			}

			return marker;
		}

		text_marker parse_text_marker(const std::string& filename,
		                              rapidxml_ns::xml_node<char>* start_node,
		                              errors::MultiError& errors,
		                              const find_relative_file_func& /*get_relative_file*/) {
			text_marker marker;

			auto& err = errors[filename];
			auto* early_node = start_node->first_node("early", 0, false);
			auto* on_time_node = start_node->first_node("ontime", 0, false);
			auto* late_node = start_node->first_node("late", 0, false);
			auto* distance_node = start_node->first_node("distance", 0, false);
			auto* timeout_node = start_node->first_node("timeout", 0, false);
			auto* trains_node = start_node->first_node("trains", 0, false);

			if (on_time_node == nullptr || (distance_node == nullptr && timeout_node == nullptr)) {
				add_error(errors, filename, 0,
				          "An <TextMarker> section must have a <OnTime> and "
				          "either a <Distance> or <Timeout>"s);
			}

			if (early_node != nullptr) {
				auto tuple = parse_text_early(early_node, err);
				marker.early_time = std::get<0>(tuple);
				marker.early_text = std::get<1>(tuple);
				marker.early_color = std::get<2>(tuple);
				marker.using_early = std::get<3>(tuple);
			}
			if (on_time_node != nullptr) {
				auto tuple = parse_text_on_time(on_time_node, err);
				marker.on_time_text = std::get<0>(tuple);
				marker.using_ontime = std::get<1>(tuple);
			}
			if (late_node != nullptr) {
				auto tuple = parse_text_late(late_node, err);
				marker.late_time = std::get<0>(tuple);
				marker.late_text = std::get<1>(tuple);
				marker.late_color = std::get<2>(tuple);
				marker.using_late = std::get<3>(tuple);
			}
			if (distance_node != nullptr) {
				marker.distance = parse_distance(distance_node, err);
			}
			if (timeout_node != nullptr) {
				marker.timeout = parse_timeout(timeout_node, err);
			}
			if (trains_node != nullptr) {
				marker.allowed_trains = parse_trains(trains_node, err);
			}

			return marker;
		}
	} // namespace

	parsed_route_marker parse(
	    const std::string& filename,
	    std::string input_string, // NOLINT(performance-unnecessary-value-param)
	    errors::MultiError& errors,
	    const find_relative_file_func& get_relative_file) {
		rapidxml_ns::xml_document<> doc;
		doc.parse<rapidxml_ns::parse_default>(&input_string[0]);

		// OpenBVE Node is Optional
		auto* openbve_node = doc.first_node("openbve", 0, false);
		rapidxml_ns::xml_node<char>* primary_node;
		if (openbve_node != nullptr) {
			primary_node = openbve_node->first_node();
		}
		else {
			primary_node = doc.first_node();
		}

		auto const primary_node_name = get_node_name(primary_node);
		auto const primary_node_name_lower = util::lower_copy(primary_node_name);

		auto const is_image_marker = primary_node_name_lower == "imagemarker"s;
		auto const is_text_marker = primary_node_name_lower == "textmarker"s;

		if (!is_image_marker && !is_text_marker) {
			std::ostringstream err;

			err << "XML node named: " << primary_node_name << " is not a valid XML marker tag.";
			add_error(errors, filename, 0, err);
		}

		if (is_image_marker) {
			return parse_image_marker(filename, primary_node, errors, get_relative_file);
		}

		return parse_text_marker(filename, primary_node, errors, get_relative_file);
	}
} // namespace bve::parsers::xml::route_marker
