#include "parsers/config/sound_cfg.hpp"
#include "gsl/gsl_util"
#include "ini.hpp"
#include "utils.hpp"

namespace parsers {
namespace config {
	namespace sound_cfg {
		namespace {
			template <class F, class Ret>
			auto parse_number_filename_pairs(F& get_filename,
			                                 errors::errors_t& errors,
			                                 ini::ini_section_t const& section) {
				static_assert(std::is_same<decltype(F(std::declval<std::string>())),
				                           filename_iterator>::value,
				              "get_filename must take a std::string and return a "
				              "filename_iterator");

				std::unordered_map<std::size_t, Ret> ret;

				for (auto const& kvp : section.key_value_pairs) {
					std::size_t index;
					try {
						index = gsl::narrow<std::size_t>(util::parse_loose_integer(kvp.key));
					}
					catch (std::exception const& e) {
						errors::add_error(errors, kvp.line, e.what());
						continue;
					}

					ret[index].filename = get_filename(kvp.value);
				}

				return ret;
			}
		} // namespace
	}     // namespace sound_cfg
} // namespace config
} // namespace parsers