#pragma once

#include <boost/variant.hpp>
#include <cinttypes>
#include <string>
#include <vector>

namespace parsers {
namespace csv_object {
	namespace instructions {
		/*
		http://openbve-project.net/documentation/HTML/object_csv_quick.html
		CreateMeshBuilder
		AddVertex, vX, vY, vZ, nX, nY, nZ
		AddFace, v1, v2, v3, ..., vn
		AddFace2, v1, v2, v3, ..., vn
		Cube, HalfWidth, HalfHeight, HalfDepth
		Cylinder, n, UpperRadius, LowerRadius, Height
		Translate, X, Y, Z
		TranslateAll, X, Y, Z
		Scale, X, Y, Z
		ScaleAll, X, Y, Z
		Rotate, X, Y, Z, Angle
		RotateAll, X, Y, Z, Angle
		Shear, dX, dY, dZ, sX, sY, sZ, Ratio
		ShearAll, dX, dY, dZ, sX, sY, sZ, Ratio
		SetColor, Red, Green, Blue, Alpha
		SetEmissiveColor, Red, Green, Blue
		SetBlendMode, { Normal | Additive }, GlowHalfDistance, { DivideExponent2 | DivideExponent4 }
		LoadTexture, DaytimeTexture, NighttimeTexture
		SetDecalTransparentColor, Red, Green, Blue
		SetTextureCoordinates, VertexIndex, X, Y
		*/

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

		using csv_instruction =
		    boost::variant<AddVertex, AddFace, Cube, Cylinder, Translate, Rotate, Shear, SetColor, SetEmissiveColor,
		                   SetBlendMode, LoadTexture, SetDecalTransparentColor, SetTextureCoordinates>;

		using csv_instruction_list = std::vector<csv_instruction>;
	} // namespace instructions
} // namespace csv_object
} // namespace parsers
