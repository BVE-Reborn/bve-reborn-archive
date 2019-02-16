#include "common.hpp"

namespace bve::parsers::csv_rw_route {
	void CSVRwRouteInstructionIOClass::operator()(const instructions::texture::BackgroundLoad& inst) {
		start(inst, "Texture.Background_Load");

		PRINT_VALUE(background_texture_index);
		PRINT_VALUE(filename);

		end();
	};
	void CSVRwRouteInstructionIOClass::operator()(const instructions::texture::BackgroundX& inst) {
		start(inst, "Texture.Background_X");

		PRINT_VALUE(background_texture_index);
		PRINT_VALUE(repetition_count);

		end();
	};
	void CSVRwRouteInstructionIOClass::operator()(const instructions::texture::BackgroundAspect& inst) {
		start(inst, "Texture.Background_Aspect");

		PRINT_VALUE(background_texture_index);
		PRINT_ENUM2(mode, texture::BackgroundAspect::Mode, fixed, aspect);

		end();
	};
} // namespace bve::parsers::csv_rw_route