#pragma once

#include <boost/variant.hpp>
#include <cinttypes>
#include <iosfwd>
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

		struct CreateMeshBuilder {};

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
			bool two;
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
			float X = 0;
			float Y = 0;
			float Z = 0;
			bool all;
		};

		// Scale and ScaleAll
		struct Scale {
			float X = 1;
			float Y = 1;
			float Z = 1;
			bool all;
		};

		// Rotate and RotateAll
		struct Rotate {
			float X = 0;
			float Y = 0;
			float Z = 0;
			float Angle = 0;
			bool all;
		};

		// Shear and ShearAll
		struct Shear {
			float dX = 0;
			float dY = 0;
			float dZ = 0;
			float sX = 0;
			float sY = 0;
			float sZ = 0;
			float r = 0;
			bool all;
		};

		struct SetColor {
			uint8_t Red = 255;
			uint8_t Green = 255;
			uint8_t Blue = 255;
			uint8_t Alpha = 255;
		};

		struct SetEmissiveColor {
			uint8_t Red = 0;
			uint8_t Green = 0;
			uint8_t Blue = 0;
		};

		struct SetBlendMode {
			enum {
				Normal,
				Additive

			} BlendMode = Normal;
			uint16_t GlowHalfDistance = 0;
			enum { DivideExponent2, DivideExponent4 } GlowAttenuationMode = DivideExponent4;
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

		std::ostream& operator<<(std::ostream& os, const CreateMeshBuilder& rhs);
		std::ostream& operator<<(std::ostream& os, const AddVertex& rhs);
		std::ostream& operator<<(std::ostream& os, const AddFace& rhs);
		std::ostream& operator<<(std::ostream& os, const Cube& rhs);
		std::ostream& operator<<(std::ostream& os, const Cylinder& rhs);
		std::ostream& operator<<(std::ostream& os, const Translate& rhs);
		std::ostream& operator<<(std::ostream& os, const Scale& rhs);
		std::ostream& operator<<(std::ostream& os, const Rotate& rhs);
		std::ostream& operator<<(std::ostream& os, const Shear& rhs);
		std::ostream& operator<<(std::ostream& os, const SetColor& rhs);
		std::ostream& operator<<(std::ostream& os, const SetEmissiveColor& rhs);
		std::ostream& operator<<(std::ostream& os, const SetBlendMode& rhs);
		std::ostream& operator<<(std::ostream& os, const LoadTexture& rhs);
		std::ostream& operator<<(std::ostream& os, const SetDecalTransparentColor& rhs);
		std::ostream& operator<<(std::ostream& os, const SetTextureCoordinates& rhs);

	} // namespace instructions
	using instruction =
	    boost::variant<instructions::CreateMeshBuilder, instructions::AddVertex, instructions::AddFace,
	                   instructions::Cube, instructions::Cylinder, instructions::Translate, instructions::Scale,
	                   instructions::Rotate, instructions::Shear, instructions::SetColor,
	                   instructions::SetEmissiveColor, instructions::SetBlendMode, instructions::LoadTexture,
	                   instructions::SetDecalTransparentColor, instructions::SetTextureCoordinates>;

	using instruction_list = std::vector<instruction>;

	instruction_list create_instructions(const std::string& text);
} // namespace csv_object
} // namespace parsers
