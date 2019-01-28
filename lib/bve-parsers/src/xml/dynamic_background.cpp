#include "parsers/xml/dynamic_background.hpp"
#include "parsers/utils.hpp"
#include "xml_node_helpers.hpp"
#include <gsl/gsl_util>
#include <rapidxml_ns.hpp>
#include <sstream>
#include <string>

using namespace std::string_literals;

namespace bve::parsers::xml::dynamic_background {
	ParsedDynamicBackground parse(
	    const std::string& filename,
	    std::string input_string, // NOLINT(performance-unnecessary-value-param)
	    errors::MultiError& errors,
	    const RelativeFileFunc& get_relative_file) {
		// This is always an vector of texture backgrounds, the object code
		// short-circuit this variable and returns a newly constructed object
		ParsedDynamicBackground db = std::vector<TextureBackgroundInfo>{};

		rapidxml_ns::xml_document<> doc;
		doc.parse<rapidxml_ns::parse_default>(&input_string[0]);

		// OpenBVE Node is Optional
		auto* openbve_node = doc.first_node("openbve", 0, false);
		rapidxml_ns::xml_node<char>* current_section;
		if (openbve_node != nullptr) {
			current_section = openbve_node->first_node("background", 0, false);
		}
		else {
			current_section = doc.first_node("background", 0, false);
		}

		for (; current_section != nullptr;
		     current_section = current_section->next_sibling("background", 0, false)) {
			auto* object = current_section->first_node("object", 0, false);

			// Only one object background allowed, and it takes priority
			if (object != nullptr) {
				auto const object_filename = get_node_value(object);

				auto const absolute = get_relative_file(filename, object_filename);

				current_section = current_section->next_sibling();
				// If multiple object_backgrounds specified add an error.
				if (current_section != nullptr) {
					add_error(
					    errors, filename, 0,
					    "Multiple Object backgrounds: only one object background is allowed."s);
				}
				return ObjectBackgroundInfo{absolute};
			}

			TextureBackgroundInfo tbi;

			auto* texture = current_section->first_node("texture", 0, false);
			auto* repetitions = current_section->first_node("repetitions", 0, false);
			auto* mode = current_section->first_node("mode", 0, false);
			auto* transition_time = current_section->first_node("transitiontime", 0, false);
			auto* time = current_section->first_node("time", 0, false);

			if (texture != nullptr) {
				auto const texture_filename = get_node_value(texture);

				tbi.filename = get_relative_file(filename, texture_filename);
			}

			if (repetitions != nullptr) {
				try {
					tbi.repetitions = gsl::narrow<std::size_t>(
					    util::parse_loose_integer(get_node_value(repetitions)));
				}
				catch (const std::exception& e) {
					add_error(errors, filename, 0, e.what());
				}
			}

			if (mode != nullptr) {
				auto const mode_text = util::lower_copy(get_node_value(mode));

				if (mode_text == "fadein"s) {
					tbi.transition_mode = TextureBackgroundInfo::fade_in;
				}
				else if (mode_text == "fadeout"s) {
					tbi.transition_mode = TextureBackgroundInfo::fade_out;
				}
				else {
					if (mode_text != "none"s) {
						std::ostringstream err;
						err << "Unrecognized texture mode: \"" << get_node_value(mode)
						    << R"(" assuming "None")";
						add_error(errors, filename, 0, err.str());
					}
					tbi.transition_mode = TextureBackgroundInfo::none;
				}
			}

			if (transition_time != nullptr) {
				try {
					tbi.transition_time = gsl::narrow<std::size_t>(
					    util::parse_loose_integer(get_node_value(transition_time)));
				}
				catch (const std::invalid_argument& e) {
					add_error(errors, filename, 0, e.what());
				}
			}

			if (time != nullptr) {
				try {
					tbi.time = gsl::narrow<std::size_t>(util::parse_time(get_node_value(time)));
				}
				catch (const std::invalid_argument& e) {
					add_error(errors, filename, 0, e.what());
				}
			}

			// No need to check as we know the type for sure within this
			// function (see above)
			db.get_unchecked<std::vector<TextureBackgroundInfo>>().emplace_back(std::move(tbi));
		}

		return db;
	}
} // namespace bve::parsers::xml::dynamic_background
