#include "b3d_csv_object.hpp"
// ReSharper disable once CppUnusedIncludeDirective
#include <iostream>

using namespace std::string_literals;

namespace parsers::b3d_csv_object::instructions {
	std::ostream& operator<<(std::ostream& os, const error& rhs) {
		os << "(ERROR, line = " << rhs.line << ", cause = " << rhs.cause << ")";
		return os;
	}

	std::ostream& operator<<(std::ostream& os, const create_mesh_builder& rhs) {
		os << "(CreateMeshBuilder, line = " << rhs.line << ")";
		return os;
	}

	std::ostream& operator<<(std::ostream& os, const add_vertex& rhs) {
		os << "(AddVertex, line = " << rhs.line << ", "
		   << "vX = " << rhs.v_x << ", "
		   << "vY = " << rhs.v_y << ", "
		   << "vZ = " << rhs.v_z << ", "
		   << "nX = " << rhs.n_x << ", "
		   << "nY = " << rhs.n_y << ", "
		   << "nZ = " << rhs.n_z << ")";
		return os;
	}

	std::ostream& operator<<(std::ostream& os, const add_face& rhs) {
		os << (rhs.side_count == sides::two ? "(AddFace2" : "(AddFace");
		for (auto& vertex_num : rhs.vertices) {
			os << ", " << vertex_num;
		}
		os << ")";
		return os;
	}

	std::ostream& operator<<(std::ostream& os, const cube& rhs) {
		os << "(Cube, "
		   << "HalfWidth = " << rhs.half_width << ", "
		   << "HalfHeight = " << rhs.half_height << ", "
		   << "HalfDepth = " << rhs.half_depth << ")";
		return os;
	}

	std::ostream& operator<<(std::ostream& os, const cylinder& rhs) {
		os << "(Cylinder, "
		   << "side_count = " << rhs.sides << ", "
		   << "UpperRadius = " << rhs.upper_radius << ", "
		   << "LowerRadius = " << rhs.lower_radius << ", "
		   << "Height = " << rhs.height << ")";
		return os;
	}

	std::ostream& operator<<(std::ostream& os, const translate& rhs) {
		os << (rhs.applies_to == apply_to::all_meshes ? "(TranslateAll, " : "(Translate, ") //
		   << "X = " << rhs.x << ", "
		   << "Y = " << rhs.y << ", "
		   << "Z = " << rhs.z << ")";
		return os;
	}

	std::ostream& operator<<(std::ostream& os, const scale& rhs) {
		os << (rhs.applies_to == apply_to::all_meshes ? "(ScaleAll, " : "(Scale, ") //
		   << "X = " << rhs.x << ", "
		   << "Y = " << rhs.y << ", "
		   << "Z = " << rhs.z << ")";
		return os;
	}

	std::ostream& operator<<(std::ostream& os, const rotate& rhs) {
		os << (rhs.applies_to == apply_to::all_meshes ? "(RotateAll, " : "(Rotate, ") //
		   << "X = " << rhs.x << ", "
		   << "Y = " << rhs.y << ", "
		   << "Z = " << rhs.z << ", "
		   << "Angle = " << rhs.angle << ")";
		return os;
	}

	std::ostream& operator<<(std::ostream& os, const shear& rhs) {
		os << (rhs.applies_to == apply_to::all_meshes ? "(ShearAll, " : "(Shear, ") //
		   << "dX = " << rhs.d_x << ", "
		   << "dY = " << rhs.d_y << ", "
		   << "dZ = " << rhs.d_z << ", "
		   << "sX = " << rhs.s_x << ", "
		   << "sY = " << rhs.s_y << ", "
		   << "sZ = " << rhs.s_z << ", "
		   << "r = " << rhs.r << ")";
		return os;
	}

	std::ostream& operator<<(std::ostream& os, const mirror& rhs) {
		os << (rhs.applies_to == apply_to::all_meshes ? "(MirrorAll, " : "(Mirror, ") //
		   << std::boolalpha                                                          //
		   << "x = " << rhs.x << ", "
		   << "y = " << rhs.y << ", "
		   << "z = " << rhs.z << ", ";
		return os;
	}

	std::ostream& operator<<(std::ostream& os, const set_color& rhs) {
		os << "(SetColor, line = " << rhs.line << ", " //
		   << "Red = " << static_cast<int>(rhs.red) << ", "
		   << "Green = " << static_cast<int>(rhs.green) << ", "
		   << "Blue = " << static_cast<int>(rhs.blue) << ", "
		   << "Alpha = " << static_cast<int>(rhs.alpha) << ")";
		return os;
	}

	std::ostream& operator<<(std::ostream& os, const set_emissive_color& rhs) {
		os << "(SetEmissiveColor, line = " << rhs.line << ", " //
		   << "Red = " << static_cast<int>(rhs.red) << ", "
		   << "Green = " << static_cast<int>(rhs.green) << ", "
		   << "Blue = " << static_cast<int>(rhs.blue) << ")";
		return os;
	}

	std::ostream& operator<<(std::ostream& os, const set_blend_mode& rhs) {
		os << "(SetBlendMode, line = " << rhs.line << ", " //
		   << "BlendMode = "
		   << (rhs.blend_mode == mesh_t::blend_mode_t::normal ? "Normal"s : "Additive"s) << ", "
		   << "GlowHalfDistance = " << rhs.glow_half_distance << ", "
		   << "GlowAttenuationMode = "
		   << (rhs.glow_attenuation_mode == mesh_t::glow_attenuation_mode_t::divide_exponent2
		           ? "DivideExponent2"s
		           : "DivideExponent4"s)
		   << ")";
		return os;
	}

	std::ostream& operator<<(std::ostream& os, const load_texture& rhs) {
		os << "(LoadTexture, line = " << rhs.line << ", "
		   << "DaytimeTexture = \"" << rhs.daytime_texture << "\", "
		   << "NighttimeTexture = \"" << rhs.nighttime_texture << "\")";
		return os;
	}

	std::ostream& operator<<(std::ostream& os, const set_decal_transparent_color& rhs) {
		os << "(SetDecalTransparentColor, line = " << rhs.line << ", " //
		   << "Red = " << static_cast<int>(rhs.red) << ", "
		   << "Green = " << static_cast<int>(rhs.green) << ", "
		   << "Blue = " << static_cast<int>(rhs.blue) << ")";
		return os;
	}

	std::ostream& operator<<(std::ostream& os, const set_texture_coordinates& rhs) {
		os << "(SetTextureCoordinates, line = " << rhs.line << ", " //
		   << "VertexIndex = " << rhs.vertex_index << ", "
		   << "X = " << rhs.x << ", "
		   << "Y = " << rhs.y << ")";
		return os;
	}
} // namespace parsers::b3d_csv_object::instructions