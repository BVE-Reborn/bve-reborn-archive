#pragma once

#include "parsers/b3d_csv.hpp"
#include <iosfwd>
#include <mapbox/variant.hpp>
#include <string>
#include <utility>
#include <vector>

namespace parsers {
// ReSharper disable once CppInconsistentNaming
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
		SetBlendMode, { Normal | Additive }, GlowHalfDistance, { DivideExponent2
		| DivideExponent4 } LoadTexture, DaytimeTexture, NighttimeTexture
		SetDecalTransparentColor, Red, Green, Blue
		SetTextureCoordinates, VertexIndex, X, Y
		*/

		enum class sides { one, two };

		enum class apply_to { single_mesh, all_meshes };

		struct error {
			std::string cause;
			std::size_t line = 0;
			error() = default;
			explicit error(std::string desc) : cause(std::move(desc)) {}
		};

		struct create_mesh_builder {
			std::size_t line = 0;
		};

		struct add_vertex {
			float v_x = 0;
			float v_y = 0;
			float v_z = 0;
			float n_x = 0;
			float n_y = 0;
			float n_z = 0;

			std::size_t line = 0;
		};

		// AddFace and AddFace2
		struct add_face {
			std::vector<std::size_t> vertices;
			sides side_count;

			std::size_t line = 0;
		};

		struct cube {
			float half_width;
			float half_height;
			float half_depth;

			std::size_t line = 0;
		};

		struct cylinder {
			std::size_t sides;
			float upper_radius;
			float lower_radius;
			float height;

			std::size_t line = 0;
		};

		// Translate and TranslateAll
		struct translate {
			float x = 0;
			float y = 0;
			float z = 0;
			apply_to applies_to;

			std::size_t line = 0;
		};

		// Scale and ScaleAll
		struct scale {
			float x = 1;
			float y = 1;
			float z = 1;
			apply_to applies_to;

			std::size_t line = 0;
		};

		// Rotate and RotateAll
		struct rotate {
			float x = 0;
			float y = 0;
			float z = 0;
			float angle = 0;
			apply_to applies_to;

			std::size_t line = 0;
		};

		// Shear and ShearAll
		struct shear {
			float d_x = 0;
			float d_y = 0;
			float d_z = 0;
			float s_x = 0;
			float s_y = 0;
			float s_z = 0;
			float r = 0;
			apply_to applies_to;

			std::size_t line = 0;
		};

		// Mirror and MirrorAll
		struct mirror {
			bool x = false;
			bool y = false;
			bool z = false;
			apply_to applies_to;

			std::size_t line = 0;
		};

		struct set_color {
			uint8_t red = 255;
			uint8_t green = 255;
			uint8_t blue = 255;
			uint8_t alpha = 255;

			std::size_t line = 0;
		};

		struct set_emissive_color {
			uint8_t red = 0;
			uint8_t green = 0;
			uint8_t blue = 0;

			std::size_t line = 0;
		};

		struct set_blend_mode {
			mesh_t::blend_mode_t blend_mode = mesh_t::blend_mode_t::normal;
			std::uint16_t glow_half_distance = 0;
			mesh_t::glow_attenuation_mode_t glow_attenuation_mode =
			    mesh_t::glow_attenuation_mode_t::divide_exponent4;

			std::size_t line = 0;
		};

		struct load_texture {
			std::string daytime_texture;
			std::string nighttime_texture;

			std::size_t line = 0;
		};

		struct set_decal_transparent_color {
			uint8_t red;
			uint8_t green;
			uint8_t blue;

			std::size_t line = 0;
		};

		struct set_texture_coordinates {
			std::size_t vertex_index;
			float x;
			float y;

			std::size_t line = 0;
		};

		// Defined in csv_object_instruction_iostream.cpp
		std::ostream& operator<<(std::ostream& os, const error& rhs);
		std::ostream& operator<<(std::ostream& os, const create_mesh_builder& rhs);
		std::ostream& operator<<(std::ostream& os, const add_vertex& rhs);
		std::ostream& operator<<(std::ostream& os, const add_face& rhs);
		std::ostream& operator<<(std::ostream& os, const cube& rhs);
		std::ostream& operator<<(std::ostream& os, const cylinder& rhs);
		std::ostream& operator<<(std::ostream& os, const translate& rhs);
		std::ostream& operator<<(std::ostream& os, const scale& rhs);
		std::ostream& operator<<(std::ostream& os, const rotate& rhs);
		std::ostream& operator<<(std::ostream& os, const shear& rhs);
		std::ostream& operator<<(std::ostream& os, const mirror& rhs);
		std::ostream& operator<<(std::ostream& os, const set_color& rhs);
		std::ostream& operator<<(std::ostream& os, const set_emissive_color& rhs);
		std::ostream& operator<<(std::ostream& os, const set_blend_mode& rhs);
		std::ostream& operator<<(std::ostream& os, const load_texture& rhs);
		std::ostream& operator<<(std::ostream& os, const set_decal_transparent_color& rhs);
		std::ostream& operator<<(std::ostream& os, const set_texture_coordinates& rhs);

		struct parsed_csv_object_builder {
			parsed_b3d_csv_object_t pso;

			// More data is needed for the faces before we convert them to
			// internal format all of this data has to be consistant within an
			// internal mesh, so part of the parsing process is to split up
			// based on this data.
			struct extended_face_data_t : face_data_t {
				std::string texture;
				bvereborn::datatypes::color8_rgb decal_transparent_color = {0, 0, 0};
				bool has_decal_transparent_color = false;

				mesh_t::blend_mode_t blend_mode = mesh_t::blend_mode_t::normal;
				mesh_t::glow_attenuation_mode_t glow_attenuation_mode =
				    mesh_t::glow_attenuation_mode_t::divide_exponent4;
				uint16_t glow_half_distance = 0;

				bool back_visible = false;
			};

			struct untriangulated_face_t {
				std::vector<std::size_t> indices;
				extended_face_data_t data;
			};

			std::vector<vertex_t> vertices;
			std::vector<untriangulated_face_t> untriangulated_faces;

			void add_mesh_builder();

			void operator()(const error& arg);
			void operator()(const create_mesh_builder& arg);
			void operator()(const add_vertex& arg);
			void operator()(const add_face& arg);
			void operator()(const cube& arg);
			void operator()(const cylinder& arg);
			void operator()(const translate& arg);
			void operator()(const scale& arg);
			void operator()(const rotate& arg);
			void operator()(const shear& arg);
			void operator()(const mirror& arg);
			void operator()(const set_color& arg);
			void operator()(const set_emissive_color& arg);
			void operator()(const set_blend_mode& arg);
			void operator()(const load_texture& arg);
			void operator()(const set_decal_transparent_color& arg);
			void operator()(const set_texture_coordinates& arg);
		};

	} // namespace instructions
	using instruction = mapbox::util::variant<instructions::error,
	                                          instructions::create_mesh_builder,
	                                          instructions::add_vertex,
	                                          instructions::add_face,
	                                          instructions::cube,
	                                          instructions::cylinder,
	                                          instructions::translate,
	                                          instructions::scale,
	                                          instructions::rotate,
	                                          instructions::shear,
	                                          instructions::mirror,
	                                          instructions::set_color,
	                                          instructions::set_emissive_color,
	                                          instructions::set_blend_mode,
	                                          instructions::load_texture,
	                                          instructions::set_decal_transparent_color,
	                                          instructions::set_texture_coordinates>;

	using instruction_list = std::vector<instruction>;

	// ReSharper disable once CppInconsistentNaming
	enum class file_type { b3d, csv };

	// Defined in csv_object_instruction_generator.cpp
	instruction_list create_instructions(std::string text, file_type ft);

	// Defined in csv_object_generator.cpp
	parsed_b3d_csv_object_t run_csv_instructions(const instruction_list& /*ilist*/);
} // namespace b3d_csv_object
} // namespace parsers
