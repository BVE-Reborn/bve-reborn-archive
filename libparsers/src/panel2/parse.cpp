//
// Created by kevin Cybura on 9/1/18.
//
#include "ini.hpp"
#include "panel2_cfg.h"
#include <utils.hpp>

using namespace std::string_literals;

namespace parsers {
namespace panel2 {
	sections::this_s create_this_section(ini::ini_section_t const& section_this) {
		sections::this_s this_section;
		for (auto& values : section_this.key_value_pairs) {
			if (values.key == "Resolution"s) {
				this_section.resolution = util::parse_loose_float(values.value);
			} else if (values.key == "Left"s){
				this_section.left = util::parse_loose_float(values.value);
			} else if (values.key == "Right"s){
				this_section.right = util::parse_loose_float(values.value);
			} else if (values.key == "Top"s){
				this_section.top = util::parse_loose_float(values.value);
			} else if (values.key == "Bottom"s) {
				this_section.bottom= util::parse_loose_float(values.value);
			}

		}
		return this_section;
	}
	parsed_panel2_cfg_t create_sections(ini::parsed_ini_object const& obj) {
		parsed_panel2_cfg_t panel;
		for (size_t i = 0; i < obj.size(); i++) {
			if (obj[i].name == "This") {
				panel.section_this = create_this_section(obj[i]);
			}
		}
		return panel;
	}

	parsed_panel2_cfg_t parse_panel2_cfg(std::string const& file_contents) {
		const auto ini = ini::parse(file_contents);
		const auto panel2 = create_sections(ini);
	}
} // namespace panel2
} // namespace parsers