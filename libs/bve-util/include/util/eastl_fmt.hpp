#pragma once

#include <EASTL/string.h>
#include <fmt/format.h>

template <>
struct fmt::formatter<eastl::string> : formatter<string_view> {
	template <typename FormatContext, typename T, typename Alloc>
	auto format(eastl::basic_string<T, Alloc> str, FormatContext& ctx) {
		basic_string_view<T> str_view(str.c_str(), str.size());
		return formatter<string_view>::format(str_view, ctx);
	}
};
