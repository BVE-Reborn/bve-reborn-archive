#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <nmmintrin.h>

#include "core/image/loader.hpp"

namespace bve::core::image {
	Loader::Loader(std::string filename) : dimensions_{} {
		data_ = stbi_loadf(filename.c_str(), &dimensions_.x, &dimensions_.y, nullptr, 4);
	}
	glm::ivec2 Loader::dimensions() const noexcept {
		return dimensions_;
	}
	const float* Loader::data() const noexcept {
		return data_;
	}

	void Loader::applyScreendoor(std::uint8_t const r, std::uint8_t const g, std::uint8_t const b) {
		// Exact same conversion done by stbi__ldr_to_hdr
		applyScreendoor(ldr_to_hdr(r), ldr_to_hdr(g), ldr_to_hdr(b));
	}

	// ReSharper disable once CppMemberFunctionMayBeConst
	void Loader::applyScreendoor(float const r, float const g, float const b) {
		__m128 const filter = _mm_set_ps(1, b, g, r);
		__m128 const eq_mask = _mm_castsi128_ps(_mm_set_epi32(0xFFFFFFFF, 0, 0, 0));
		__m128 const assign_mask = _mm_castsi128_ps(_mm_set_epi32(0, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF));

		std::size_t const pixel_count = dimensions_.x * dimensions_.y;
		for (std::size_t i = 0; i < pixel_count; ++i) {
			__m128 const pixels = _mm_load_ps(&data_[4 * i]);
			__m128 const equality = _mm_cmpeq_ps(filter, pixels);
			__m128 const true_equal = _mm_or_ps(equality, eq_mask);
			bool const v = _mm_test_all_ones(_mm_castps_si128(true_equal)) == 1;
			if (v) {
				__m128 const doored = _mm_and_ps(pixels, assign_mask);
				_mm_store_ps(&data_[4 * i], doored);
			}
		}
	}

	void Loader::multiply(std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a) {
		multiply(ldr_to_hdr(r), ldr_to_hdr(g), ldr_to_hdr(b), static_cast<float>(a) / 255.0f);
	}

	void Loader::multiply(float r, float g, float b, float a) {
		__m128 const multiplicand = _mm_set_ps(a, b, g, r);

		std::size_t const pixel_count = dimensions_.x * dimensions_.y;
		for (std::size_t i = 0; i < pixel_count; ++i) {
			__m128 pixel = _mm_load_ps(&data_[4 * i]);
			__m128 result = _mm_mul_ps(pixel, multiplicand);
			_mm_store_ps(&data_[4 * i], result);
		}
	}

	bool Loader::valid() const noexcept {
		return data_ != nullptr;
	}
	Loader::~Loader() {
		stbi_image_free(data_);
	}

	float Loader::ldr_to_hdr(uint8_t v) {
		return static_cast<float>(pow(float(v) / 255.0f, stbi__l2h_gamma) * stbi__l2h_scale);
	}


} // namespace bve::core::image
