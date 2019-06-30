#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

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
	bool Loader::valid() const noexcept {
		return data_ != nullptr;
	}
	Loader::~Loader() {
		stbi_image_free(data_);
	}

} // namespace bve::core::image
