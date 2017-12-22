#pragma once

#include <cinttypes>

namespace openbve2 {
namespace datatypes {
	template <class ColorType>
	struct color_r {
		ColorType r;
	};

	template <class ColorType>
	struct color_rg {
		ColorType r;
		ColorType g;
	};

	template <class ColorType>
	struct color_rgb {
		ColorType r;
		ColorType g;
		ColorType b;
	};

	template <class ColorType>
	struct color_rgba {
		ColorType r;
		ColorType g;
		ColorType b;
		ColorType a;
	};

	using color8_r = color_r<std::uint8_t>;
	using color8_rg = color_rg<std::uint8_t>;
	using color8_rgb = color_rgb<std::uint8_t>;
	using color8_rgba = color_rgba<std::uint8_t>;

	using color16_r = color_r<std::uint16_t>;
	using color16_rg = color_rg<std::uint16_t>;
	using color16_rgb = color_rgb<std::uint16_t>;
	using color16_rgba = color_rgba<std::uint16_t>;

	using colorF_r = color_r<float>;
	using colorF_rg = color_rg<float>;
	using colorF_rgb = color_rgb<float>;
	using colorF_rgba = color_rgba<float>;
} // namespace datatypes
} // namespace openbve2