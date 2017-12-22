#pragma once

namespace openbve2 {
namespace math {
	template <class T, class A, class B>
	inline T clamp(T value, A min, B max) {
		return value > max ? T(max) : (value < min ? T(min) : value);
	}
} // namespace math
} // namespace openbve2