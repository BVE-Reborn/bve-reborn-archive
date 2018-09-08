#pragma once

#ifdef _MSC_VER
#	define FORCE_INLINE __forceinline inline
#elif defined(__GNUC__) || defined(__clang__)
#	define FORCE_INLINE __attribute__((always_inline)) inline
#else
#	define FORCE_INLINE inline
#endif
