#include "parsers/xml/dynamic_lighting.hpp"
#include "utils.hpp"
#include "xml_node_helpers.hpp"
#include <rapidxml_ns.hpp>

using namespace std::string_literals;

namespace bve::parsers::xml::dynamic_lighting {
	std::vector<LightingInfo> parse(
	    const std::string& filename,
	    std::string input_string, // NOLINT(performance-unnecessary-value-param)
	    errors::MultiError& errors) {
		std::vector<LightingInfo> ret;

		rapidxml_ns::xml_document<> doc;
		doc.parse<rapidxml_ns::parse_default>(&input_string[0]);

		// OpenBVE Node is Optional
		auto* openbve_node = doc.first_node("openbve", 0, false);
		rapidxml_ns::xml_node<char>* current_section;
		if (openbve_node != nullptr) {
			current_section = openbve_node->first_node("brightness", 0, false);
		}
		else {
			current_section = doc.first_node("brightness", 0, false);
		}

		for (; current_section != nullptr;
		     current_section = current_section->next_sibling("brightness", 0, false)) {
			LightingInfo info;

			auto* time = current_section->first_node("time", 0, false);
			auto* ambient_light = current_section->first_node("ambientlight", 0, false);
			auto* directional_light = current_section->first_node("directionallight", 0, false);
			auto* light_direction = current_section->first_node("lightdirection", 0, false);
			auto* cab_lighting = current_section->first_node("cablighting", 0, false);

			if (time != nullptr) {
				try {
					info.time = util::parse_time(get_node_value(time));
				}
				catch (const std::invalid_argument& e) {
					add_error(errors, filename, 0, e.what());
				}
			}

			if (ambient_light != nullptr) {
				auto pairs = util::split_text(get_node_value(ambient_light));
				if (pairs.size() >= 3) {
					try {
						info.ambient.r = uint8_t(util::parse_loose_integer(pairs[0]));
						info.ambient.g = uint8_t(util::parse_loose_integer(pairs[1]));
						info.ambient.b = uint8_t(util::parse_loose_integer(pairs[2]));
					}
					catch (const std::invalid_argument& e) {
						add_error(errors, filename, 0, e.what());
					}
				}
				if (pairs.size() != 3) {
					add_error(errors, filename, 0, "<AmbientLight> must have exactly 3 arguments");
				}
			}

			if (directional_light != nullptr) {
				auto pairs = util::split_text(get_node_value(directional_light));
				if (pairs.size() >= 3) {
					try {
						info.directional_lighting.r = uint8_t(util::parse_loose_integer(pairs[0]));
						info.directional_lighting.g = uint8_t(util::parse_loose_integer(pairs[1]));
						info.directional_lighting.b = uint8_t(util::parse_loose_integer(pairs[2]));
					}
					catch (const std::invalid_argument& e) {
						add_error(errors, filename, 0, e.what());
					}
				}
				if (pairs.size() != 3) {
					add_error(errors, filename, 0,
					          "<DirectionalLight> must have "
					          "exactly 3 "
					          "arguments");
				}
			}

			if (light_direction != nullptr) {
				auto pairs = util::split_text(get_node_value(light_direction));
				if (pairs.size() >= 3) {
					try {
						info.light_direction.x = util::parse_loose_float(pairs[0]);
						info.light_direction.y = util::parse_loose_float(pairs[1]);
						info.light_direction.z = util::parse_loose_float(pairs[2]);
					}
					catch (const std::invalid_argument& e) {
						add_error(errors, filename, 0, e.what());
					}
				}
				if (pairs.size() != 3) {
					add_error(errors, filename, 0,
					          "<LightDirection> must have exactly 3 arguments");
				}
			}

			if (cab_lighting != nullptr) {
				try {
					info.cab_lighting =
					    uint8_t(util::parse_loose_integer(get_node_value(cab_lighting)));
				}
				catch (const std::invalid_argument& e) {
					add_error(errors, filename, 0, e.what());
				}
			}

			ret.emplace_back(std::move(info));
		}

		if (ret.empty()) {
			add_error(errors, filename, 0,
			          "XML dynamic lighting must have at least one <brightness> node."s);
		}

		return ret;
	}
} // namespace bve::parsers::xml::dynamic_lighting
