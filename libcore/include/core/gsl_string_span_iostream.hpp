#pragma once

#include <gsl/string_span>
#include <ostream>
#include <type_traits>

template <class CharT, std::ptrdiff_t Extant>
auto& operator<<(std::basic_ostream<std::remove_const_t<CharT>>& os,
                 gsl::basic_string_span<CharT, Extant> const& rhs) {
	os.write(rhs.data(), rhs.size());
	return os;
}
