#pragma once

#include <mapbox/variant.hpp>
#include <ostream>
#ifdef __GNUC__
#include <cxxabi.h>
#endif

namespace openbve2 {
	namespace variant_util {
		namespace {
			template<std::size_t num, class... Args >
			void _print_sizes(std::ostream& os, const std::tuple<Args...>& val) {
				auto name = typeid(std::get<num>(val)).name();

#ifdef __GNUC__
				name = abi::__cxa_demangle(name, nullptr, nullptr, nullptr);
#endif

				os << name << " " << sizeof(std::get<num>(val)) << '\n';

#ifdef __GNUC__
				free(const_cast<char*>(name)); // NOLINT
#endif
			 }

			template<std::size_t num>
			struct _print_sizes_struct {
				template<class... Args>
				static void func(std::ostream& os, const std::tuple<Args...>& val) {
					_print_sizes<num>(os, val);
					_print_sizes_struct<num - 1>::func(os, val);
				}
			};

			template<>
			struct _print_sizes_struct <0> {
				template<class... Args>
				static void func(std::ostream& os, const std::tuple<Args...>& val) {
					_print_sizes<0>(os, val);
				}
			};
		}  // namespace

		template <class... Args>
		void print_sizes(std::ostream& os, const mapbox::util::variant<Args...>& val) {
			auto name = typeid(val).name();

#ifdef __GNUC__
			name = abi::__cxa_demangle(name, nullptr, nullptr, nullptr);
#endif

			os << name << " " << sizeof(val) << '\n';

#ifdef __GNUC__
			free(const_cast<char*>(name)); // NOLINT
#endif

			_print_sizes_struct<std::tuple_size<typename mapbox::util::variant<Args...>::types>::value - 1>::func(os, typename mapbox::util::variant<Args...>::types{});
		}
	}  // namespace variant_util
}  // namespace openbve2
