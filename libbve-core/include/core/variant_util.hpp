#pragma once

#include <mapbox/variant.hpp>
#include <ostream>
#ifdef __GNUC__
#	include <cxxabi.h>
#endif

namespace bve::core::variant_util {
	namespace {
		template <std::size_t Num, class... Args>
		void print_sizes(std::ostream& os, std::tuple<Args...> const& val) {
			(void) val;

			auto name = typeid(std::get<Num>(val)).name();

#ifdef __GNUC__
			name = abi::__cxa_demangle(name, nullptr, nullptr, nullptr);
#endif

			os << name << " " << sizeof(std::get<Num>(val)) << '\n';

#ifdef __GNUC__
			free(const_cast<char*>(name)); // NOLINT
#endif
		}

		template <std::size_t Num>
		struct print_sizes_struct {
			template <class... Args>
			static void func(std::ostream& os, const std::tuple<Args...>& val) {
				print_sizes<Num>(os, val);
				print_sizes_struct<Num - 1>::func(os, val);
			}
		};

		template <>
		struct print_sizes_struct<0> {
			template <class... Args>
			static void func(std::ostream& os, const std::tuple<Args...>& val) {
				print_sizes<0>(os, val);
			}
		};
	} // namespace

	template <class... Args>
	void print_sizes(std::ostream& os, const mapbox::util::variant<Args...>& val) {
		(void) val;

		auto name = typeid(val).name();

#ifdef __GNUC__
		name = abi::__cxa_demangle(name, nullptr, nullptr, nullptr);
#endif

		os << name << " " << sizeof(val) << '\n';

#ifdef __GNUC__
		free(const_cast<char*>(name)); // NOLINT
#endif

		print_sizes_struct<std::tuple_size<typename mapbox::util::variant<Args...>::types>::value
		                   - 1>::func(os, typename mapbox::util::variant<Args...>::types{});
	}
} // namespace bve::bve::core::variant_util
