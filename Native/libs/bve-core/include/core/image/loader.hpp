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
		bool valid() const noexcept;
		~Loader();

	  private:
		glm::ivec2 dimensions_;
		float* data_;
	};
} // namespace bve::core::image
