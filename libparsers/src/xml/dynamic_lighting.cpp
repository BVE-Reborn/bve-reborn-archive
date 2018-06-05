#include "parsers/xml/dynamic_lighting.hpp"
#include "utils.hpp"
#include <rapidxml_ns.hpp>

namespace parsers {
namespace xml {
	namespace dynamic_lighting {
		std::vector<lighting_info> parse(const std::string& filename,
		                                 std::string input_string, // NOLINT(performance-unnecessary-value-param)
		                                 errors::multi_error_t& errors) {
			std::vector<lighting_info> retvalue;

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
				lighting_info info;

				auto* time = current_section->first_node("time", 0, false);
				auto* ambient_light = current_section->first_node("ambientlight", 0, false);
				auto* directional_light = current_section->first_node("directionallight", 0, false);
				auto* light_direction = current_section->first_node("lightdirection", 0, false);
				auto* cab_lighting = current_section->first_node("cablighting", 0, false);

				if (time != nullptr) {
					try {
						info.time = util::parse_time(time->value());
					}
					catch (const std::invalid_argument& e) {
						add_error(errors, filename, 0, e.what());
					}
				}

				if (ambient_light != nullptr) {
					auto pairs = util::split_text(ambient_light->value());
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
						add_error(errors, filename, 0,
						          "<AmbientLight> must have exactly 3 arguments");
					}
				}

				if (directional_light != nullptr) {
					auto pairs = util::split_text(directional_light->value());
					if (pairs.size() >= 3) {
						try {
							info.directional_lighting.r =
							    uint8_t(util::parse_loose_integer(pairs[0]));
							info.directional_lighting.g =
							    uint8_t(util::parse_loose_integer(pairs[1]));
							info.directional_lighting.b =
							    uint8_t(util::parse_loose_integer(pairs[2]));
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
					auto pairs = util::split_text(light_direction->value());
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
						info.cablighting =
						    uint8_t(util::parse_loose_integer(cab_lighting->value()));
					}
					catch (const std::invalid_argument& e) {
						add_error(errors, filename, 0, e.what());
					}
				}

				retvalue.emplace_back(std::move(info));
			}

			return retvalue;
		}
	} // namespace dynamic_lighting
} // namespace xml
} // namespace parsers
