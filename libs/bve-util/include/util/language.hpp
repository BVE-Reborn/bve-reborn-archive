#pragma once

#include <cinttypes>
#include <immintrin.h>

// ReSharper disable CppInconsistentNaming
#define cast static_cast

using i8 = std::int8_t;
using i16 = std::int16_t;
using i32 = std::int32_t;
using i64 = std::int64_t;
using iSize = std::ptrdiff_t;
using i128 = __m128i;
using i256 = __m256i;

using u8 = std::uint8_t;
using u16 = std::uint16_t;
using u32 = std::uint32_t;
using u64 = std::uint64_t;
using uSize = std::size_t;

using f32 = float;
using f64 = double;
using f80 = long double;

using vi128 = __m128i;
using vi256 = __m256i;
using v4xf32 = __m128;
using v8xf32 = __m256;
using v2xf64 = __m128d;
using v4xf64 = __m256d;
// ReSharper restore CppInconsistentNaming
