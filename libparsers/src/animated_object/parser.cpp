#include "ini.hpp"
#include "parsers/parsers.hpp"
#include "utils.hpp"

namespace parsers {
namespace animated_object {
	static void parse_include_section(parsed_animated_object& pso, const ini::ini_section_t& section) {
		std::vector<std::string> files;
		glm::vec3 position;

		for (auto& file : section.values) {
			files.emplace_back(file.value);
		}

		for (auto& kvp : section.key_value_pairs) {
			if (util::match_text(kvp.key, "position")) {
				auto split = util::split_text(kvp.value, ',');

				if (split.size() != 3) {
					pso.errors.emplace_back(errors::error_t{kvp.line, "Postion must have 3 arguments"});
				}

				try {
					switch (split.size()) {
						default:
						case 3:
							position.z = util::parse_loose_float(split[2]);
							// fall through
						case 2:
							position.y = util::parse_loose_float(split[1]);
							// fall through
						case 1:
							position.x = util::parse_loose_float(split[0]);
							// fall through
						case 0:
							break;
					}
				}
				catch (const std::invalid_argument& e) {
					pso.errors.emplace_back(errors::error_t{kvp.line, e.what()});
				}
			}
			else {
				pso.errors.emplace_back(
				    errors::error_t{kvp.line, "No other key may be set besides position inside an include section"});
			}
		}

		for (auto& file : files) {
			pso.includes.emplace_back(animated_include{file, position});
		}
	}

	parsed_animated_object parse(const std::string& file) {
		parsed_animated_object pao;

		auto ini = ini::parse(file);

		for (auto& section : ini) {
			// "" section is before any named section
			if (section.name == "") {
				for (auto& value : section.values) {
					pao.errors.emplace_back(
					    errors::error_t{value.line, "Animated files must have all commands within sections"});
				}

				for (auto& kvp : section.key_value_pairs) {
					pao.errors.emplace_back(
					    errors::error_t{kvp.line, "Animated files must have all commands within sections"});
				}
			}

			// all includes to the file
			else if (util::match_text(section.name, "includes")) {
				parse_include_section(pao, section);
			}

			// add object
			else if (util::match_text(section.name, "object")) {
			}
			else {
				pao.errors.emplace_back(
				    errors::error_t{section.line, "Animated files may only have \"Include\" and \"Object\" sections"});
			}
		}

		return pao;
	}
} // namespace animated_object
} // namespace parsers