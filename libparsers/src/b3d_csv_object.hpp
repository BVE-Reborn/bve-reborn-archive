#pragma once

#include "parsers/b3d_csv.hpp"
#include <cinttypes>
#include <iosfwd>
#include <mapbox/variant.hpp>
#include <string>
#include <vector>

namespace parsers {
namespace b3d_csv_object {
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

		struct base_instruction {
			std::size_t line;
		};

		struct Error : public base_instruction {
			std::string cause;
			Error() = default;
			Error(const std::string& desc) : cause(desc){};
		};

		struct CreateMeshBuilder : public base_instruction {};

		struct AddVertex : public base_instruction {
			float vX = 0;
			float vY = 0;
			float vZ = 0;
			float nX = 0;
			float nY = 0;
			float nZ = 0;
		};

		// AddFace and AddFace2
		struct AddFace : public base_instruction {
			std::vector<std::size_t> vertices;
			bool two;
		};

		struct Cube : public base_instruction {
			float HalfWidth;
			float HalfHeight;
			float HalfDepth;
		};

		struct Cylinder : public base_instruction {
			std::size_t sides;
			float UpperRadius;
			float LowerRadius;
			float Height;
		};

		// Translate and TranslateAll
		struct Translate : public base_instruction {
			float X = 0;
			float Y = 0;
			float Z = 0;
			bool all;
		};

		// Scale and ScaleAll
		struct Scale : public base_instruction {
			float X = 1;
			float Y = 1;
			float Z = 1;
			bool all;
		};

		// Rotate and RotateAll
		struct Rotate : public base_instruction {
			float X = 0;
			float Y = 0;
			float Z = 0;
			float Angle = 0;
			bool all;
		};

		// Shear and ShearAll
		struct Shear : public base_instruction {
			float dX = 0;
			float dY = 0;
			float dZ = 0;
			float sX = 0;
			float sY = 0;
			float sZ = 0;
			float r = 0;
			bool all;
		};

		struct SetColor : public base_instruction {
			uint8_t Red = 255;
			uint8_t Green = 255;
			uint8_t Blue = 255;
			uint8_t Alpha = 255;
		};

		struct SetEmissiveColor : public base_instruction {
			uint8_t Red = 0;
			uint8_t Green = 0;
			uint8_t Blue = 0;
		};

		struct SetBlendMode : public base_instruction {
			mesh_t::BlendMode_t BlendMode = mesh_t::BlendMode_t::Normal;
			uint16_t GlowHalfDistance = 0;
			mesh_t::GlowAttenuationMode_t GlowAttenuationMode = mesh_t::GlowAttenuationMode_t::DivideExponent4;
		};

		struct LoadTexture : public base_instruction {
			std::string DaytimeTexture;
			std::string NighttimeTexture;
		};

		struct SetDecalTransparentColor : public base_instruction {
			uint8_t Red;
			uint8_t Green;
			uint8_t Blue;
		};

		struct SetTextureCoordinates : public base_instruction {
			std::size_t VertexIndex;
			float X;
			float Y;
		};

		// Defined in csv_object_instruction_iostream.cpp
		std::ostream& operator<<(std::ostream& os, const Error& rhs);
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

		struct parsed_csv_object_builder {
			parsed_b3d_csv_object_t pso;

			// More data is needed for the faces before we convert them to internal format
			// all of this data has to be consistant within an internal mesh, so part of the
			// parsing process is to split up based on this data.
			struct extended_face_data_t : face_data_t {
				std::string texture;
				openbve2::datatypes::color8_rgb decal_transparent_color = {0, 0, 0};
				bool has_decal_transparent_color = false;

				mesh_t::BlendMode_t BlendMode = mesh_t::BlendMode_t::Normal;
				mesh_t::GlowAttenuationMode_t GlowAttenuationMode = mesh_t::GlowAttenuationMode_t::DivideExponent4;
				uint16_t GlowHalfDistance = 0;

				bool back_visible = false;
			};

			struct untriangulated_face_t {
				std::vector<std::size_t> indices;
				extended_face_data_t data;
			};

			std::vector<vertex_t> vertices;
			std::vector<untriangulated_face_t> untriangulated_faces;

			void add_mesh_builder();

			void operator()(const Error& arg);
			void operator()(const CreateMeshBuilder& arg);
			void operator()(const AddVertex& arg);
			void operator()(const AddFace& arg);
			void operator()(const Cube& arg);
			void operator()(const Cylinder& arg);
			void operator()(const Translate& arg);
			void operator()(const Scale& arg);
			void operator()(const Rotate& arg);
			void operator()(const Shear& arg);
			void operator()(const SetColor& arg);
			void operator()(const SetEmissiveColor& arg);
			void operator()(const SetBlendMode& arg);
			void operator()(const LoadTexture& arg);
			void operator()(const SetDecalTransparentColor& arg);
			void operator()(const SetTextureCoordinates& arg);
		};

	} // namespace instructions
	using instruction = mapbox::util::variant<
	    instructions::Error, instructions::CreateMeshBuilder, instructions::AddVertex, instructions::AddFace,
	    instructions::Cube, instructions::Cylinder, instructions::Translate, instructions::Scale, instructions::Rotate,
	    instructions::Shear, instructions::SetColor, instructions::SetEmissiveColor, instructions::SetBlendMode,
	    instructions::LoadTexture, instructions::SetDecalTransparentColor, instructions::SetTextureCoordinates>;

	using instruction_list = std::vector<instruction>;

	enum class file_type { b3d, csv };

	// Defined in csv_object_instruction_generator.cpp
	instruction_list create_instructions(std::string text, file_type ft);

	// Defined in csv_object_generator.cpp
	parsed_b3d_csv_object_t run_csv_instructions(const instruction_list&);
} // namespace b3d_csv_object
} // namespace parsers
