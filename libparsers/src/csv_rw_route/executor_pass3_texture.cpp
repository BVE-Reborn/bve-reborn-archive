#include "executor_pass3.hpp"
#include "utils.hpp"
#include <string>

using namespace std::string_literals;

namespace parsers {
namespace csv_rw_route {
	void pass3_executor::operator()(const instructions::texture::Background_Load& instr) {
		using texture_vector = std::vector<xml::dynamic_backgrounds::texture_background_info>;

		auto issuer_filename = get_filename(instr.file_index);

		// Hacky way to find the file extant of the file in order to determine if XML. Works due to OpenBVE1's
		// strictness on filenames
		auto period_index = instr.filename.find_last_of('.');

		// If there is no period, assuming non xml file
		bool is_XML = false;
		if (period_index != std::string::npos) {
			auto extant = instr.filename.substr(period_index + 1);
			util::lower(extant);

			is_XML = extant == "xml"s;
		}

		auto filename = _get_relative_file(issuer_filename, instr.filename);

		if (is_XML) {
			// this will only be true if the background hasn't has been touched before
			// as if the vector is non empty it has been added to, and only the xml can write an object background
			if (_route_data.background.is<texture_vector>() &&
			    !_route_data.background.get_unchecked<texture_vector>().empty()) {
				_errors[issuer_filename].emplace_back<errors::error_t>(
				    {instr.line,
				     "Texture.Background(XML) is overwriting all prior calls the Texture Background functions"s});
			}

			try {
				_route_data.background = xml::dynamic_backgrounds::parse(filename, _errors, _get_relative_file);
			}
			catch (const std::exception& e) {
				_errors[issuer_filename].emplace_back<errors::error_t>({instr.line, e.what()});
			}
			used_dynamic_background = true;
			return;
		}

		// non xml file

		if (used_dynamic_background) {
			_errors[issuer_filename].emplace_back<errors::error_t>(
			    {instr.line, "Texture.Background(XML) has already been used, ignoring Texture.Background(Image)"s});
			return;
		}

		if (!_route_data.background.is<texture_vector>()) {
			// initalize it with 1 sized vector (needed for original functions)
			_route_data.background = texture_vector(1, xml::dynamic_backgrounds::texture_background_info{});
		}
		else if (_route_data.background.get_unchecked<texture_vector>().empty()) {
			_route_data.background.get_unchecked<texture_vector>().resize(1);
		}

		_route_data.background.get_unchecked<texture_vector>()[0].filename = instr.filename;
	}
	void pass3_executor::operator()(const instructions::texture::Background_X& instr) {}
	void pass3_executor::operator()(const instructions::texture::Background_Aspect& instr) {}
} // namespace csv_rw_route
} // namespace parsers
