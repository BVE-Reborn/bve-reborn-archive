//
// Created by kevin Cybura on 9/1/18.
//
#include "include/config/panel2_cfg.h"
#include "ini.hpp"
#include <utils.hpp>

using namespace std::string_literals;

namespace parsers {
namespace panel2 {
	sections::linear_gauge_s create_linear_gauge_section(ini::ini_section_t const& linear,
	                                                     std::string const& file_name,
	                                                     errors::errors_t& errors) {
	    sections::linear_gauge_s gauge;

	    for (auto& values : linear.key_value_pairs) {
	    	if (values.key == "Subject"s) {

	    	}
			else if (values.key == "Location"s) {
				try {
					auto split = util::split_text(values.value);
					gauge.location.first = util::parse_loose_float(split[0]);
					gauge.location.second = util::parse_loose_float(split[1]);
				}
				catch (std::invalid_argument err) {
					gauge.location.first = 0;
					gauge.location.second = 0;
					errors::add_error(errors, 0, err.what());
				}
			}
			else if (values.key == "Minimum"s) {
				try {
					gauge.min = util::parse_loose_float(values.value);
				}
				catch (std::invalid_argument err) {
					gauge.min = 0;
					errors::add_error(errors, 0, err.what());
				}
			}
			else if (values.key == "Maximum"s) {
				try {
					gauge.max = util::parse_loose_float(values.value);
				}
				catch (std::invalid_argument err) {
					gauge.max = 0;
					errors::add_error(errors, 0, err.what());
				}
			}
			else if (values.key == "Direction"s) {
				try {
					auto split = util::split_text(values.value);
					gauge.direction.first = util::parse_loose_float(split[0]);
					gauge.direction.second = util::parse_loose_float(split[1]);
				}
				catch (std::invalid_argument err) {
					gauge.direction.first = 0;
					gauge.direction.second = 0;
					errors::add_error(errors, 0, err.what());
				}
			}
			else if (values.key == "Width"s) {
				try {
					gauge.width = util::parse_loose_integer(values.value);
				}
				catch (std::invalid_argument err) {
					gauge.width = 0;
					errors::add_error(errors, 0, err.what());
				}
			}
			else if (values.key == "Layer"s) {
				try {
					gauge.layer = util::parse_loose_integer(values.value);
				}
				catch (std::invalid_argument err) {
					gauge.layer = 0;
					errors::add_error(errors, 0, err.what());
				}
			}

	    }

	    return gauge;
	}

	sections::digital_gauge_s create_digital_gauge_section(ini::ini_section_t const& digital,
	                                                       std::string const& file_name,
	                                                       errors::errors_t& errors) {
		sections::digital_gauge_s gauge;
		for (auto& values : digital.key_value_pairs) {
			if (values.key == "Subject") {
			}
			else if (values.key == "Location"s) {
				try {
					auto split = util::split_text(values.value);
					gauge.location.first = util::parse_loose_float(split[0]);
					gauge.location.second = util::parse_loose_float(split[1]);
				}
				catch (std::invalid_argument err) {
					gauge.location.first = 0;
					gauge.location.second = 0;
					errors::add_error(errors, 0, err.what());
				}
			}
			else if (values.key == "Radius"s) {
				try {
					gauge.radius = util::parse_loose_float(values.value);
				}
				catch (std::invalid_argument err) {
					gauge.radius = 0;
					errors::add_error(errors, 0, err.what());
				}
			}
			else if (values.key == "Color"s) {
				try {
					gauge.color_ = util::parse_color(values.value);
				}
				catch (std::invalid_argument err) {
					gauge.color_ = sections::color(0, 0, 0, 0xFF);
					errors::add_error(errors, 0, err.what());
				}
			}
			else if (values.key == "InitialAngle"s) {
				try {
					gauge.initial_angle = util::parse_loose_float(values.value);
				}
				catch (std::invalid_argument err) {
					gauge.initial_angle = 0;
					errors::add_error(errors, 0, err.what());
				}
			}
			else if (values.key == "LastAngle"s) {
				try {
					gauge.last_angle = util::parse_loose_float(values.value);
				}
				catch (std::invalid_argument err) {
					gauge.last_angle = 0;
					errors::add_error(errors, 0, err.what());
				}
			}
			else if (values.key == "Minimum"s) {
				try {
					gauge.min = util::parse_loose_float(values.value);
				}
				catch (std::invalid_argument err) {
					gauge.min = 0;
					errors::add_error(errors, 0, err.what());
				}
			}
			else if (values.key == "Maximum"s) {
				try {
					gauge.max = util::parse_loose_float(values.value);
				}
				catch (std::invalid_argument err) {
					gauge.max = 0;
					errors::add_error(errors, 0, err.what());
				}
			}
			else if (values.key == "Layer"s) {
				try {
					gauge.layer = util::parse_loose_integer(values.value);
				}
				catch (std::invalid_argument err) {
					gauge.layer = 0;
					errors::add_error(errors, 0, err.what());
				}
			}
			else if (values.key == "Step"s) {
				try {
					gauge.step = util::parse_loose_float(values.value);
				}
				catch (std::invalid_argument err) {
					gauge.step = 0;
					errors::add_error(errors, 0, err.what());
				}
			}
		}
		return gauge;
	}

	sections::digital_number_s create_digital_number_section(ini::ini_section_t const& digital,
	                                                         std::string const& file_name,
	                                                         errors::errors_t& errors) {
		sections::digital_number_s digital_nums;
		for (auto& values : digital.key_value_pairs) {
			if (values.key == "Subject") {
			}
			else if (values.key == "Location"s) {
				try {
					auto split = util::split_text(values.value);
					digital_nums.location.first = util::parse_loose_float(split[0]);
					digital_nums.location.second = util::parse_loose_float(split[1]);
				}
				catch (std::invalid_argument err) {
					digital_nums.location.first = 0;
					digital_nums.location.second = 0;
					errors::add_error(errors, 0, err.what());
				}
			}
			else if (values.key == "DaytimeImage"s) {
				digital_nums.day_time_image = values.value;
			}
			else if (values.key == "NighttimeImage"s) {
				digital_nums.night_time_image = values.value;
			}
			else if (values.key == "TransparentColor"s) {
				try {
					digital_nums.transparent_color = util::parse_color(values.value);
				}
				catch (std::invalid_argument err) {
					digital_nums.transparent_color = sections::color(0, 0, 0, 0xFF);
					errors::add_error(errors, 0, err.what());
				}
			}
			else if (values.key == "Interval"s) {
				try {
					digital_nums.interval = util::parse_loose_integer(values.value);
				}
				catch (std::invalid_argument err) {
					// abort() ? VERB_FATAL
					digital_nums.interval = 0;
					errors::add_error(errors, 0, err.what());
				}
			}
			else if (values.key == "Layer"s) {
				try {
					digital_nums.layer = util::parse_loose_integer(values.value);
				}
				catch (std::invalid_argument err) {
					digital_nums.layer = 0;
					errors::add_error(errors, 0, err.what());
				}
			}
		}
		if (!((digital_nums.day_time_image != ""s) && (digital_nums.night_time_image != ""s))) {
			errors::add_error(
			    errors, 0,
			    "If DaytimeImage or NighttimeImage are declare they both have to be declared"s);
			digital_nums.night_time_image = ""s;
			digital_nums.day_time_image = ""s;
		}
		return digital_nums;
	}

	sections::needle_s create_needle_section(ini::ini_section_t const& needle,
	                                         std::string const& file_name,
	                                         errors::errors_t& errors) {
		sections::needle_s needle_section;
		for (auto& values : needle.key_value_pairs) {
			if (values.key == "Subject") {
			}
			else if (values.key == "Location"s) {
				try {
					auto split = util::split_text(values.value);
					needle_section.location.first = util::parse_loose_float(split[0]);
					needle_section.location.second = util::parse_loose_float(split[1]);
				}
				catch (std::invalid_argument err) {
					needle_section.location.first = 0;
					needle_section.location.second = 0;
					errors::add_error(errors, 0, err.what());
				}
			}
			else if (values.key == "Radius"s) {
				try {
					needle_section.radius = util::parse_loose_float(values.value);
				}
				catch (std::invalid_argument err) {
					needle_section.radius = 0;
					errors::add_error(errors, 0, err.what());
				}
			}
			else if (values.key == "DaytimeImage"s) {
				needle_section.day_time_image = values.value;
			}
			else if (values.key == "NighttimeImage"s) {
				needle_section.night_time_image = values.value;
			}
			else if (values.key == "Color"s) {
				try {
					needle_section.color_ = util::parse_color(values.value);
				}
				catch (std::invalid_argument err) {
					needle_section.color_ = sections::color(0, 0, 0, 0xFF);
					errors::add_error(errors, 0, err.what());
				}
			}
			else if (values.key == "TransparentColor"s) {
				try {
					needle_section.transparent_color = util::parse_color(values.value);
				}
				catch (std::invalid_argument err) {
					needle_section.transparent_color = sections::color(0, 0, 0, 0xFF);
					errors::add_error(errors, 0, err.what());
				}
			}
			else if (values.key == "Origin"s) {
				try {
					auto split = util::split_text(values.value);
					needle_section.origin.first = util::parse_loose_float(split[0]);
					needle_section.origin.second = util::parse_loose_float(split[1]);
				}
				catch (std::invalid_argument err) {
					errors::add_error(errors, 0, err.what());
				}
			}
			else if (values.key == "InitialAngle"s) {
				try {
					needle_section.initial_angle = util::parse_loose_float(values.value);
				}
				catch (std::invalid_argument err) {
					needle_section.initial_angle = 0;
					errors::add_error(errors, 0, err.what());
				}
			}
			else if (values.key == "LastAngle"s) {
				try {
					needle_section.last_angle = util::parse_loose_float(values.value);
				}
				catch (std::invalid_argument err) {
					needle_section.last_angle = 0;
					errors::add_error(errors, 0, err.what());
				}
			}
			else if (values.key == "Minimum"s) {
				try {
					needle_section.min = util::parse_loose_float(values.value);
				}
				catch (std::invalid_argument err) {
					needle_section.min = 0;
					errors::add_error(errors, 0, err.what());
				}
			}
			else if (values.key == "Maximum"s) {
				try {
					needle_section.max = util::parse_loose_float(values.value);
				}
				catch (std::invalid_argument err) {
					needle_section.max = 0;
					errors::add_error(errors, 0, err.what());
				}
			}
			else if (values.key == "NaturalFreq"s) {
				try {
					needle_section.natural_freq = util::parse_loose_float(values.value);
				}
				catch (std::invalid_argument err) {
					needle_section.natural_freq = 0;
					errors::add_error(errors, 0, err.what());
				}
			}
			else if (values.key == "DampingRaito"s) {
				try {
					needle_section.damping_ratio = util::parse_loose_float(values.value);
				}
				catch (std::invalid_argument err) {
					needle_section.damping_ratio = 0;
					errors::add_error(errors, 0, err.what());
				}
			}
			else if (values.key == "Layer"s) {
				try {
					needle_section.layer = util::parse_loose_integer(values.value);
				}
				catch (std::invalid_argument err) {
					needle_section.layer = 0;
					errors::add_error(errors, 0, err.what());
				}
			}
		}
		if (!((needle_section.day_time_image != ""s) && (needle_section.night_time_image != ""s))) {
			errors::add_error(
			    errors, 0,
			    "If DaytimeImage or NighttimeImage are declare they both have to be declared"s);
			needle_section.night_time_image = ""s;
			needle_section.day_time_image = ""s;
		}
		return needle_section;
	}

	sections::pilot_lamp_s create_pilot_lamp_section(ini::ini_section_t const& pilot_lamp,
	                                                 std::string const& file_name,
	                                                 errors::errors_t& errors) {
		sections::pilot_lamp_s pilot_lamp_section;

		for (auto& values : pilot_lamp.key_value_pairs) {
			if (values.key == "Subject") {
			}
			else if (values.key == "Location"s) {
				try {
					auto spilt = util::split_text(values.value);
					pilot_lamp_section.location.first = util::parse_loose_float(spilt[0]);
					pilot_lamp_section.location.second = util::parse_loose_float(spilt[1]);
				}
				catch (std::invalid_argument err) {
					pilot_lamp_section.location.first = 0;
					pilot_lamp_section.location.second = 0;
					errors::add_error(errors, 0, err.what());
				}
			}
			else if (values.key == "DaytimeImage"s) {
				pilot_lamp_section.day_time_image = values.value;
			}
			else if (values.key == "NighttimeImage"s) {
				pilot_lamp_section.night_time_image = values.value;
			}
			else if (values.key == "TransparentColor"s) {
				try {
					pilot_lamp_section.transparent_color = util::parse_color(values.value);
				}
				catch (std::invalid_argument err) {
					pilot_lamp_section.transparent_color = sections::color(0, 0, 0, 0xFF);
					errors::add_error(errors, 0, err.what());
				}
			}
			else if (values.key == "Layer") {
				try {
					pilot_lamp_section.layer = util::parse_loose_integer(values.value);
				}
				catch (std::invalid_argument err) {
					pilot_lamp_section.layer = 0;
					errors::add_error(errors, 0, err.what());
				}
			}
		}
		if (!((pilot_lamp_section.day_time_image != ""s)
		      && (pilot_lamp_section.night_time_image != ""s))) {
			errors::add_error(
			    errors, 0,
			    "If DaytimeImage or NighttimeImage are declare they both have to be declared"s);
			pilot_lamp_section.night_time_image = ""s;
			pilot_lamp_section.day_time_image = ""s;
		}
		return pilot_lamp_section;
	} // namespace panel2

	sections::this_s create_this_section(ini::ini_section_t const& section_this,
	                                     std::string const& file_name,
	                                     errors::errors_t& errors) {
		sections::this_s this_section;
		for (auto& values : section_this.key_value_pairs) {
			if (values.key == "Resolution"s) {
				try {
					this_section.resolution = util::parse_loose_float(values.value);
				}
				catch (std::invalid_argument err) {
					errors::add_error(errors, 0, err.what());
					this_section.resolution = 0;
				}
			}
			else if (values.key == "Left"s) {
				try {
					this_section.left = util::parse_loose_float(values.value);
				}
				catch (std::invalid_argument err) {
					errors::add_error(errors, 0, err.what());
					this_section.left = 0;
				}
			}
			else if (values.key == "Right"s) {
				try {
					this_section.right = util::parse_loose_float(values.value);
				}
				catch (std::invalid_argument err) {
					errors::add_error(errors, 0, err.what());
					this_section.right = 0;
				}
			}
			else if (values.key == "Top"s) {
				try {
					this_section.top = util::parse_loose_float(values.value);
				}
				catch (std::invalid_argument err) {
					errors::add_error(errors, 0, err.what());
					this_section.top = 0;
				}
			}
			else if (values.key == "Bottom"s) {
				try {
					this_section.bottom = util::parse_loose_float(values.value);
				}
				catch (std::invalid_argument err) {
					errors::add_error(errors, 0, err.what());
					this_section.bottom = 0;
				}
			}
			else if (values.key == "DaytimeImage"s) {
				this_section.day_time_image = values.value;
			}
			else if (values.key == "NighttimeImage"s) {
				this_section.night_time_image = values.value;
			}
			else if (values.key == "TransparentColor"s) {
				// TODO
				try {
					this_section.transparent_color = util::parse_color(values.value);
				}
				catch (std::invalid_argument err) {
					this_section.transparent_color = sections::color(0, 0, 0, 0xFF);
					errors::add_error(errors, 0, err.what());
				}
			}
			else if (values.key == "Center"s) {
				try {
					auto split = util::split_text(values.value);
					this_section.center.first = util::parse_loose_float(split[0]);
					this_section.center.second = util::parse_loose_float(split[1]);
				}
				catch (std::invalid_argument err) {
					errors::add_error(errors, 0, err.what());
					this_section.center.first = 0;
					this_section.center.second = 0;
				}
			}
			else if (values.key == "Origin"s) {
				try {
					auto split = util::split_text(values.value);
					this_section.origin.first = util::parse_loose_float(split[0]);
					this_section.origin.second = util::parse_loose_float(split[1]);
				}
				catch (std::invalid_argument err) {
					errors::add_error(errors, 0, err.what());
				}
			}
		}
		if (!((this_section.day_time_image != ""s) && (this_section.night_time_image != ""s))) {
			errors::add_error(
			    errors, 0,
			    "If DaytimeImage or NighttimeImage are declare they both have to be declared"s);
			this_section.night_time_image = ""s;
			this_section.day_time_image = ""s;
		}
		return this_section;
	}
	parsed_panel2_cfg_t create_sections(ini::parsed_ini_object const& obj,
	                                    std::string const& file_name,
	                                    errors::multi_error_t& errors) {
		parsed_panel2_cfg_t panel;
		for (auto& section : obj) {
			if (section.name == "This"s) {
				panel.section_this = create_this_section(section, file_name, errors["This"]);
			}
			else if (section.name == "PilotLamp"s) {
				panel.pilot_lamp =
				    create_pilot_lamp_section(section, file_name, errors["PilotLamp"]);
			}
			else if (section.name == "Needle"s) {
				panel.needle = create_needle_section(section, file_name, errors["Needle"]);
			}
			else if (section.name == "DigitalNumber"s) {
				panel.number =
				    create_digital_number_section(section, file_name, errors["DigitalNumber"]);
			}
			else if (section.name == "DigitalGauge"s) {
				panel.gauge =
				    create_digital_gauge_section(section, file_name, errors["DigitalGauge"]);
			}
			else if (section.name == "LinearGauge"s) {
				panel.linear =
				    create_linear_gauge_section(section, file_name, errors["LinearGauge"]);
			}
		}
		return panel;
	}

	parsed_panel2_cfg_t parse_panel2_cfg(std::string const& file_contents,
	                                     std::string const& file_name,
	                                     errors::multi_error_t& errors) {
		const auto ini = ini::parse(file_contents);
		const auto panel2 = create_sections(ini, file_name, errors);
		return panel2;
	}
} // namespace panel2
} // namespace parsers