#pragma once

#include <cinttypes>
#include <string>
#include <vector>

namespace parsers {
namespace csv_obj {
	namespace instructions {
		struct AddVertex {
			float vX = 0;
			float vY = 0;
			float vZ = 0;
			float nX = 0;
			float nY = 0;
			float nZ = 0;
		};

		// AddFace and AddFace2
		struct AddFace {
			std::vector<std::size_t> vertices;
		};

		struct Cube {
			float HalfWidth;
			float HalfHeight;
			float HalfDepth;
		};

		struct Cylinder {
			std::size_t sides;
			float UpperRadius;
			float LowerRadius;
			float Height;
		};

		// Translate and TranslateAll
		struct Translate {
			float X;
			float Y;
			float Z;
		};

		// Scale and ScaleAll
		struct Rotate {
			float X;
			float Y;
			float Z;
			float Angle;
		};

		// Shear and ShearAll
		struct Shear {
			float dX;
			float dY;
			float dZ;
			float sX;
			float sY;
			float sZ;
			float r;
		};

		struct SetColor {
			uint8_t Red;
			uint8_t Green;
			uint8_t Blue;
			uint8_t Alpha;
		};

		struct SetEmissiveColor {
			uint8_t Red;
			uint8_t Green;
			uint8_t Blue;
		};

		struct SetBlendMode {
			enum {
				Normal,
				Additive

			} BlendMode;
			uint16_t GlowHalfDistance;
			enum { DivideExponent2, DivideExponent4 } GlowAttenuationMode;
		};

		struct LoadTexture {
			std::string DaytimeTexture;
			std::string NighttimeTexture;
		};

		struct SetDecalTransparentColor {
			uint8_t Red;
			uint8_t Green;
			uint8_t Blue;
		};

		struct SetTextureCoordinates {
			std::size_t VertexIndex;
			float X;
			float Y;
		};
	} // namespace instructions
} // namespace csv_obj
} // namespace parsers
