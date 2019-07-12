#pragma once

#include <glm/vec2.hpp>
#include <string>

namespace bve::core::image {
	class Loader {
	  public:
		explicit Loader(std::string filename);
		glm::ivec2 dimensions() const noexcept;
		const float* data() const noexcept;
		void applyScreendoor(std::uint8_t r, std::uint8_t g, std::uint8_t b);
		void applyScreendoor(float r, float g, float b);
		void multiply(std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a);
		void multiply(float r, float g, float b, float a);
		bool valid() const noexcept;
		~Loader();

		static float ldr_to_hdr(uint8_t);

	  private:
		glm::ivec2 dimensions_;
		float* data_;
	};
} // namespace bve::core::image
