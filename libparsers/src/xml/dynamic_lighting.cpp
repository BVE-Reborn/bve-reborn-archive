#include "parsers/xml/dynamic_lighting.hpp"
#include "utils.hpp"
#include <rapidxml/rapidxml.hpp>

namespace parsers {
namespace xml {
	namespace dynamic_lighting {
		std::vector<lighting_info> parse(const std::string& filename, errors::multi_error& errors) {
			auto file = util::load_from_file_utf8_bom(filename);

			std::vector<lighting_info> retvalue;

			rapidxml::xml_document<> doc;
			doc.parse<rapidxml::parse_default>(&file[0]);

			auto* current_section = doc.first_node("brightness", 0, false);

			for (; current_section != nullptr;
			     current_section = current_section->next_sibling("brightness", 0, false)) {
				lighting_info info;

				auto* time = current_section->first_node("time", 0, false);
				auto* ambient_light = current_section->first_node("ambientlight", 0, false);
				auto* directional_light = current_section->first_node("directionallight", 0, false);
				auto* light_direction = current_section->first_node("lightdirection", 0, false);
				auto* cab_lighting = current_section->first_node("cablighting", 0, false);

				if (time) {
					try {
						info.time = util::parse_time(time->value());
					}
					catch (const std::invalid_argument& e) {
						errors[filename].emplace_back<errors::error_t>({0, e.what()});
					}
				}

				if (ambient_light) {
					auto pairs = util::split_text(ambient_light->value());
					if (pairs.size() >= 3) {
						try {
							info.ambient.r = uint8_t(util::parse_loose_integer(pairs[0]));
							info.ambient.g = uint8_t(util::parse_loose_integer(pairs[1]));
							info.ambient.b = uint8_t(util::parse_loose_integer(pairs[2]));
						}
						catch (const std::invalid_argument& e) {
							errors[filename].emplace_back<errors::error_t>({0, e.what()});
						}
					}
					if (pairs.size() != 3) {
						errors[filename].emplace_back<errors::error_t>(
						    {0, "<AmbientLight> must have exactly 3 arguments"});
					}
				}

				if (directional_light) {
					auto pairs = util::split_text(ambient_light->value());
					if (pairs.size() >= 3) {
						try {
							info.directional_lighting.r = uint8_t(util::parse_loose_integer(pairs[0]));
							info.directional_lighting.g = uint8_t(util::parse_loose_integer(pairs[1]));
							info.directional_lighting.b = uint8_t(util::parse_loose_integer(pairs[2]));
						}
						catch (const std::invalid_argument& e) {
							errors[filename].emplace_back<errors::error_t>({0, e.what()});
						}
					}
					if (pairs.size() != 3) {
						errors[filename].emplace_back<errors::error_t>(
						    {0, "<DirectionalLight> must have exactly 3 arguments"});
					}
				}

				if (light_direction) {
					auto pairs = util::split_text(ambient_light->value());
					if (pairs.size() >= 3) {
						try {
							info.light_direction.x = util::parse_loose_float(pairs[0]);
							info.light_direction.y = util::parse_loose_float(pairs[1]);
							info.light_direction.z = util::parse_loose_float(pairs[2]);
						}
						catch (const std::invalid_argument& e) {
							errors[filename].emplace_back<errors::error_t>({0, e.what()});
						}
					}
					if (pairs.size() != 3) {
						errors[filename].emplace_back<errors::error_t>(
						    {0, "<LightDirection> must have exactly 3 arguments"});
					}
				}

				if (cab_lighting) {
					try {
						info.cablighting = uint8_t(util::parse_loose_integer(cab_lighting->value()));
					}
					catch (const std::invalid_argument& e) {
						errors[filename].emplace_back<errors::error_t>({0, e.what()});
					}
				}

				retvalue.emplace_back(std::move(info));
			}

			return retvalue;
		}
	} // namespace dynamic_lighting
} // namespace xml
} // namespace parsers
