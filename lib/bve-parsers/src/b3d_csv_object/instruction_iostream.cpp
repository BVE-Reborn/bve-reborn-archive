#include "b3d_csv_object.hpp"
// ReSharper disable once CppUnusedIncludeDirective
#include <iostream>

using namespace std::string_literals;

namespace bve::parsers::b3d_csv_object::instructions {
	std::ostream& operator<<(std::ostream& os, const Error& rhs) {
		os << "(ERROR, line = " << rhs.line << ", cause = " << rhs.cause << ")";
		return os;
	}

	std::ostream& operator<<(std::ostream& os, const CreateMeshBuilder& rhs) {
		os << "(CreateMeshBuilder, line = " << rhs.line << ")";
		return os;
	}

	std::ostream& operator<<(std::ostream& os, const AddVertex& rhs) {
		os << "(AddVertex, line = " << rhs.line << ", "
		   << "vX = " << rhs.v_x << ", "
		   << "vY = " << rhs.v_y << ", "
		   << "vZ = " << rhs.v_z << ", "
		   << "nX = " << rhs.n_x << ", "
		   << "nY = " << rhs.n_y << ", "
		   << "nZ = " << rhs.n_z << ")";
		return os;
	}

	std::ostream& operator<<(std::ostream& os, const AddFace& rhs) {
		os << (rhs.side_count == Sides::two ? "(AddFace2" : "(AddFace");
		for (auto& vertex_num : rhs.vertices) {
			os << ", " << vertex_num;
		}
		os << ")";
		return os;
	}

	std::ostream& operator<<(std::ostream& os, const Cube& rhs) {
		os << "(Cube, "
		   << "HalfWidth = " << rhs.half_width << ", "
		   << "HalfHeight = " << rhs.half_height << ", "
		   << "HalfDepth = " << rhs.half_depth << ")";
		return os;
	}

	std::ostream& operator<<(std::ostream& os, const Cylinder& rhs) {
		os << "(Cylinder, "
		   << "side_count = " << rhs.sides << ", "
		   << "UpperRadius = " << rhs.upper_radius << ", "
		   << "LowerRadius = " << rhs.lower_radius << ", "
		   << "Height = " << rhs.height << ")";
		return os;
	}

	std::ostream& operator<<(std::ostream& os, const Translate& rhs) {
		os << (rhs.applies_to == ApplyTo::all_meshes ? "(TranslateAll, " : "(Translate, ") //
		   << "X = " << rhs.x << ", "
		   << "Y = " << rhs.y << ", "
		   << "Z = " << rhs.z << ")";
		return os;
	}

	std::ostream& operator<<(std::ostream& os, const Scale& rhs) {
		os << (rhs.applies_to == ApplyTo::all_meshes ? "(ScaleAll, " : "(Scale, ") //
		   << "X = " << rhs.x << ", "
		   << "Y = " << rhs.y << ", "
		   << "Z = " << rhs.z << ")";
		return os;
	}

	std::ostream& operator<<(std::ostream& os, const Rotate& rhs) {
		os << (rhs.applies_to == ApplyTo::all_meshes ? "(RotateAll, " : "(Rotate, ") //
		   << "X = " << rhs.x << ", "
		   << "Y = " << rhs.y << ", "
		   << "Z = " << rhs.z << ", "
		   << "Angle = " << rhs.angle << ")";
		return os;
	}

	std::ostream& operator<<(std::ostream& os, const Shear& rhs) {
		os << (rhs.applies_to == ApplyTo::all_meshes ? "(ShearAll, " : "(Shear, ") //
		   << "dX = " << rhs.d_x << ", "
		   << "dY = " << rhs.d_y << ", "
		   << "dZ = " << rhs.d_z << ", "
		   << "sX = " << rhs.s_x << ", "
		   << "sY = " << rhs.s_y << ", "
		   << "sZ = " << rhs.s_z << ", "
		   << "r = " << rhs.r << ")";
		return os;
	}

	std::ostream& operator<<(std::ostream& os, const Mirror& rhs) {
		os << (rhs.applies_to == ApplyTo::all_meshes ? "(MirrorAll, " : "(Mirror, ") //
		   << std::boolalpha                                                         //
		   << "x = " << rhs.x << ", "
		   << "y = " << rhs.y << ", "
		   << "z = " << rhs.z << ", ";
		return os;
	}

	std::ostream& operator<<(std::ostream& os, const SetColor& rhs) {
		os << "(SetColor, line = " << rhs.line << ", " //
		   << "Red = " << static_cast<int>(rhs.red) << ", "
		   << "Green = " << static_cast<int>(rhs.green) << ", "
		   << "Blue = " << static_cast<int>(rhs.blue) << ", "
		   << "Alpha = " << static_cast<int>(rhs.alpha) << ")";
		return os;
	}

	std::ostream& operator<<(std::ostream& os, const SetEmissiveColor& rhs) {
		os << "(SetEmissiveColor, line = " << rhs.line << ", " //
		   << "Red = " << static_cast<int>(rhs.red) << ", "
		   << "Green = " << static_cast<int>(rhs.green) << ", "
		   << "Blue = " << static_cast<int>(rhs.blue) << ")";
		return os;
	}

	std::ostream& operator<<(std::ostream& os, const SetBlendMode& rhs) {
		os << "(SetBlendMode, line = " << rhs.line << ", " //
		   << "BlendMode = " << (rhs.blend_mode == Mesh::BlendMode::normal ? "Normal"s : "Additive"s) << ", "
		   << "GlowHalfDistance = " << rhs.glow_half_distance << ", "
		   << "GlowAttenuationMode = "
		   << (rhs.glow_attenuation_mode == Mesh::GlowAttenuationMode::divide_exponent2 ? "DivideExponent2"s : "DivideExponent4"s) << ")";
		return os;
	}

	std::ostream& operator<<(std::ostream& os, const LoadTexture& rhs) {
		os << "(LoadTexture, line = " << rhs.line << ", "
		   << "DaytimeTexture = \"" << rhs.daytime_texture << "\", "
		   << "NighttimeTexture = \"" << rhs.nighttime_texture << "\")";
		return os;
	}

	std::ostream& operator<<(std::ostream& os, const SetDecalTransparentColor& rhs) {
		os << "(SetDecalTransparentColor, line = " << rhs.line << ", " //
		   << "Red = " << static_cast<int>(rhs.red) << ", "
		   << "Green = " << static_cast<int>(rhs.green) << ", "
		   << "Blue = " << static_cast<int>(rhs.blue) << ")";
		return os;
	}

	std::ostream& operator<<(std::ostream& os, const SetTextureCoordinates& rhs) {
		os << "(SetTextureCoordinates, line = " << rhs.line << ", " //
		   << "VertexIndex = " << rhs.vertex_index << ", "
		   << "X = " << rhs.x << ", "
		   << "Y = " << rhs.y << ")";
		return os;
	}
} // namespace bve::parsers::b3d_csv_object::instructions
