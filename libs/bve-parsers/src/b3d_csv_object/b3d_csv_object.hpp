#pragma once

#include "parsers/b3d_csv.hpp"
#include <iosfwd>
#include <mapbox/variant.hpp>
#include <string>
#include <utility>
#include <vector>

// ReSharper disable once CppInconsistentNaming
namespace bve::parsers::b3d_csv_object {
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
		SetBlendMode, { Normal | Additive }, GlowHalfDistance, { DivideExponent2
		| DivideExponent4 } LoadTexture, DaytimeTexture, NighttimeTexture
		SetDecalTransparentColor, Red, Green, Blue
		SetTextureCoordinates, VertexIndex, X, Y
		*/

		enum class Sides { one, two };

		enum class ApplyTo { single_mesh, all_meshes };

		// TODO(cwfitzgerald): Use unified errors
		struct Error {
			std::string cause;
			std::size_t line = 0;
			Error() = default;
			explicit Error(std::string desc) : cause(std::move(desc)) {}
		};

		struct CreateMeshBuilder {
			std::size_t line = 0;
		};

		struct AddVertex {
			float v_x = 0;
			float v_y = 0;
			float v_z = 0;
			float n_x = 0;
			float n_y = 0;
			float n_z = 0;

			std::size_t line = 0;
		};

		// AddFace and AddFace2
		struct AddFace {
			std::vector<std::size_t> vertices;
			Sides side_count;

			std::size_t line = 0;
		};

		struct Cube {
			float half_width{};
			float half_height{};
			float half_depth{};

			std::size_t line = 0;
		};

		struct Cylinder {
			std::size_t sides{};
			float upper_radius{};
			float lower_radius{};
			float height{};

			std::size_t line = 0;
		};

		// Translate and TranslateAll
		struct Translate {
			float x = 0;
			float y = 0;
			float z = 0;
			ApplyTo applies_to;

			std::size_t line = 0;
		};

		// Scale and ScaleAll
		struct Scale {
			float x = 1;
			float y = 1;
			float z = 1;
			ApplyTo applies_to;

			std::size_t line = 0;
		};

		// Rotate and RotateAll
		struct Rotate {
			float x = 0;
			float y = 0;
			float z = 0;
			float angle = 0;
			ApplyTo applies_to;

			std::size_t line = 0;
		};

		// Shear and ShearAll
		struct Shear {
			float d_x = 0;
			float d_y = 0;
			float d_z = 0;
			float s_x = 0;
			float s_y = 0;
			float s_z = 0;
			float r = 0;
			ApplyTo applies_to;

			std::size_t line = 0;
		};

		// Mirror and MirrorAll
		struct Mirror {
			bool x = false;
			bool y = false;
			bool z = false;
			ApplyTo applies_to;

			std::size_t line = 0;
		};

		struct SetColor {
			uint8_t red = 255;
			uint8_t green = 255;
			uint8_t blue = 255;
			uint8_t alpha = 255;

			std::size_t line = 0;
		};

		struct SetEmissiveColor {
			uint8_t red = 0;
			uint8_t green = 0;
			uint8_t blue = 0;

			std::size_t line = 0;
		};

		struct SetBlendMode {
			Mesh::BlendMode blend_mode = Mesh::BlendMode::normal;
			std::uint16_t glow_half_distance = 0;
			Mesh::GlowAttenuationMode glow_attenuation_mode = Mesh::GlowAttenuationMode::divide_exponent4;

			std::size_t line = 0;
		};

		struct LoadTexture {
			std::string daytime_texture;
			std::string nighttime_texture;

			std::size_t line = 0;
		};

		struct SetDecalTransparentColor {
			uint8_t red{};
			uint8_t green{};
			uint8_t blue{};

			std::size_t line = 0;
		};

		struct SetTextureCoordinates {
			std::size_t vertex_index{};
			float x{};
			float y{};

			std::size_t line = 0;
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
		std::ostream& operator<<(std::ostream& os, const Mirror& rhs);
		std::ostream& operator<<(std::ostream& os, const SetColor& rhs);
		std::ostream& operator<<(std::ostream& os, const SetEmissiveColor& rhs);
		std::ostream& operator<<(std::ostream& os, const SetBlendMode& rhs);
		std::ostream& operator<<(std::ostream& os, const LoadTexture& rhs);
		std::ostream& operator<<(std::ostream& os, const SetDecalTransparentColor& rhs);
		std::ostream& operator<<(std::ostream& os, const SetTextureCoordinates& rhs);

		struct ParsedCSVObjectBuilder {
			ParsedB3DCSVObject pso;

			// More data is needed for the faces before we convert them to
			// internal format all of this data has to be consistent within an
			// internal mesh, so part of the parsing process is to split up
			// based on this data.
			struct ExtendedFaceData : FaceData {
				std::string texture;
				util::datatypes::Color8RGB decal_transparent_color = {0, 0, 0};
				bool has_decal_transparent_color = false;

				Mesh::BlendMode blend_mode = Mesh::BlendMode::normal;
				Mesh::GlowAttenuationMode glow_attenuation_mode = Mesh::GlowAttenuationMode::divide_exponent4;
				uint16_t glow_half_distance = 0;

				bool back_visible = false;
			};

			struct UntriangulatedFace {
				std::vector<std::size_t> indices;
				ExtendedFaceData data;
			};

			std::vector<Vertex> vertices;
			std::vector<UntriangulatedFace> untriangulated_faces;

			void addMeshBuilder();

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
			void operator()(const Mirror& arg);
			void operator()(const SetColor& arg);
			void operator()(const SetEmissiveColor& arg);
			void operator()(const SetBlendMode& arg);
			void operator()(const LoadTexture& arg);
			void operator()(const SetDecalTransparentColor& arg);
			void operator()(const SetTextureCoordinates& arg);
		};

	} // namespace instructions

	using Instruction = mapbox::util::variant<instructions::Error,
	                                          instructions::CreateMeshBuilder,
	                                          instructions::AddVertex,
	                                          instructions::AddFace,
	                                          instructions::Cube,
	                                          instructions::Cylinder,
	                                          instructions::Translate,
	                                          instructions::Scale,
	                                          instructions::Rotate,
	                                          instructions::Shear,
	                                          instructions::Mirror,
	                                          instructions::SetColor,
	                                          instructions::SetEmissiveColor,
	                                          instructions::SetBlendMode,
	                                          instructions::LoadTexture,
	                                          instructions::SetDecalTransparentColor,
	                                          instructions::SetTextureCoordinates>;

	using InstructionList = std::vector<Instruction>;

	enum class FileType {
		// ReSharper disable CppInconsistentNaming
		b3d,
		csv
	};

	InstructionList create_instructions(std::string text, FileType ft);

	ParsedB3DCSVObject run_csv_instructions(const InstructionList& /*ilist*/);
} // namespace bve::parsers::b3d_csv_object
