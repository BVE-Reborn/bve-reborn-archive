#pragma once

#include <type_traits>

namespace openbve2 {
namespace math {
	template <class T, class A, class B>
	inline T clamp(T value, A min, B max) {
		return value > max ? T(max) : (value < min ? T(min) : value);
	}

	template <class R = void, class A, class B>
	inline auto max(A a, B b) ->
	    typename std::conditional<std::is_same<R, void>::value,
	                              typename std::make_signed<typename std::common_type<A, B>::type>::type, R>::type {
		using t = decltype(max<R, A, B>(std::declval<A>(), std::declval<B>()));
		return t(a) > t(b) ? t(a) : t(b);
	}

	template <class R = void, class A, class B>
	inline auto min(A a, B b) ->
	    typename std::conditional<std::is_same<R, void>::value,
	                              typename std::make_signed<typename std::common_type<A, B>::type>::type, R>::type {
		using t = decltype(max<R, A, B>(std::declval<A>(), std::declval<B>()));
		return t(a) < t(b) ? t(a) : t(b);
	}
} // namespace math
} // namespace openbve2