#include "parsers/xml/route_marker.hpp"
#include "utils.hpp"
#include <rapidxml/rapidxml.hpp>
#include <sstream>
#include <string>

using namespace std::string_literals;

namespace parsers {
namespace xml {
	namespace route_marker {
		namespace {
			template <bool early, bool text>
			std::pair<openbve2::datatypes::time, std::string> parse_early_late_node_impl(
			    rapidxml::xml_node<char>* start_node,
			    errors::errors_t& file_errors) {
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

					errors::add_error(file_errors, 0, err);
					throw std::invalid_argument(err);
				}

				return std::make_pair(util::parse_time(time_node->value()),
				                      std::string(data_node->name(), data_node->name_size()));
			}

			std::pair<openbve2::datatypes::time, std::string> parse_text_early_node(
				rapidxml::xml_node<char>* start_node,
				errors::errors_t& file_errors) {
				return parse_early_late_node_impl<true, true>(start_node, file_errors);
			}
			std::pair<openbve2::datatypes::time, std::string> parse_text_late_node(
				rapidxml::xml_node<char>* start_node,
				errors::errors_t& file_errors) {
				return parse_early_late_node_impl<false, true>(start_node, file_errors);
			}

			std::pair<openbve2::datatypes::time, std::string> parse_image_early_node(
				rapidxml::xml_node<char>* start_node,
				errors::errors_t& file_errors) {
				return parse_early_late_node_impl<true, false>(start_node, file_errors);
			}
			std::pair<openbve2::datatypes::time, std::string> parse_image_late_node(
				rapidxml::xml_node<char>* start_node,
				errors::errors_t& file_errors) {
				return parse_early_late_node_impl<false, false>(start_node, file_errors);
			}

			template <bool text>
			std::string parse_on_time_node_impl(rapidxml::xml_node<char>* start_node, errors::errors_t& file_errors) {
				auto* data_node = start_node->first_node((text ? "text"s : "image"s).c_str(), 0, false);

				if (data_node == nullptr) {
					std::string err;

					if (text) {
						err = "XML node <OnTime> must have a <Text> node."s;
					}
					else {
						err = "XML node <OnTime> must have a <Image> node."s;
					}

					errors::add_error(file_errors, 0, err);
					throw std::invalid_argument(err);
				}

				return std::string(data_node->value(), data_node->value_size());
			}

			std::string parse_text_on_time_node(rapidxml::xml_node<char>* start_node, errors::errors_t& file_errors) {
				return parse_on_time_node_impl<true>(start_node, file_errors);
			}
			std::string parse_image_on_time_node(rapidxml::xml_node<char>* start_node, errors::errors_t& file_errors) {
				return parse_on_time_node_impl<false>(start_node, file_errors);
			}

			image_marker parse_image_marker(const std::string& filename,
			                                rapidxml::xml_node<char>* start_node,
			                                errors::errors_t& file_errors,
			                                const find_relative_file_func& get_relative_file) {
				image_marker marker;

				auto* early_node = start_node->first_node("early", 0, false);
				auto* on_time_node = start_node->first_node("ontime", 0, false);
				auto* late_node = start_node->first_node("late", 0, false);
				auto* distance_node = start_node->first_node("distance", 0, false);
				auto* timeout_node = start_node->first_node("timeout", 0, false);
				auto* trains_node = start_node->first_node("trains", 0, false);

				if (early_node != nullptr) {
					auto pair = parse_image_early_node(early_node, file_errors);
					marker.using_early = true;
					marker.early_time = pair.first;
					marker.early_filename = get_relative_file(filename, pair.second);
				}
				if (on_time_node != nullptr) {
					auto val = parse_image_on_time_node(on_time_node, file_errors);
					marker.on_time_filename = get_relative_file(filename, val);
				}
				if (early_node != nullptr) {
					auto pair = parse_image_late_node(early_node, file_errors);
					marker.using_late = true;
					marker.late_time = pair.first;
					marker.late_filename = get_relative_file(filename, pair.second);
				}
			}
		} // namespace

		parsed_route_marker parse(const std::string& filename,
		                          std::string input_string,
		                          errors::errors_t& file_errors,
		                          const find_relative_file_func& get_relative_file) {
			rapidxml::xml_document<> doc;
			doc.parse<rapidxml::parse_default>(&input_string[0]);

			auto* primary_node = doc.first_node();
			auto primary_node_name = std::string(primary_node->name(), primary_node->name_size());
			auto primary_node_name_lower = util::lower_copy(primary_node_name);

			bool is_image_marker = primary_node_name == "imagemarker"s;
			bool is_text_marker = primary_node_name == "textmarker"s;

			if (!is_image_marker || !is_text_marker) {
				std::ostringstream err;

				err << "XML node named: " << primary_node_name << " is not a valid XML marker tag.";

				errors::add_error(file_errors, 0, err);

				throw std::invalid_argument(err.str());
			}

			if (is_image_marker) {
				return parse_image_marker(filename, primary_node, file_errors, get_relative_file);
			}

			if (is_text_marker) {
			}
		}
	} // namespace route_marker
} // namespace xml
} // namespace parsers
