#pragma once

#include <glm/vec2.hpp>
#include <string>

namespace bve::core::image {
	class Loader {
	  public:
		explicit Loader(std::string filename);
		glm::ivec2 dimensions() const noexcept;
		const uint8_t* data() const noexcept;
		bool valid() const noexcept;
		~Loader();

	  private:
		glm::ivec2 dimensions_;
		uint8_t* data_;
	};
} // namespace bve::core::image
