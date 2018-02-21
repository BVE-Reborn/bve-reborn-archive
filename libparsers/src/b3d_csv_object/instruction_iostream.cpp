#include "b3d_csv_object.hpp"
#include <iostream>

using namespace std::string_literals;

namespace parsers {
namespace b3d_csv_object {
	namespace instructions {
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
			   << "vX = " << rhs.vX << ", "
			   << "vY = " << rhs.vY << ", "
			   << "vZ = " << rhs.vZ << ", "
			   << "nX = " << rhs.nX << ", "
			   << "nY = " << rhs.nY << ", "
			   << "nZ = " << rhs.nZ << ")";
			return os;
		}

		std::ostream& operator<<(std::ostream& os, const AddFace& rhs) {
			os << (rhs.two ? "(AddFace2" : "(AddFace");
			for (auto& vertex_num : rhs.vertices) {
				os << ", " << vertex_num;
			}
			os << ")";
			return os;
		}

		std::ostream& operator<<(std::ostream& os, const Cube& rhs) {
			os << "(Cube, "
			   << "HalfWidth = " << rhs.HalfWidth << ", "
			   << "HalfHeight = " << rhs.HalfHeight << ", "
			   << "HalfDepth = " << rhs.HalfDepth << ")";
			return os;
		}

		std::ostream& operator<<(std::ostream& os, const Cylinder& rhs) {
			os << "(Cylinder, "
			   << "sides = " << rhs.sides << ", "
			   << "UpperRadius = " << rhs.UpperRadius << ", "
			   << "LowerRadius = " << rhs.LowerRadius << ", "
			   << "Height = " << rhs.Height << ")";
			return os;
		}

		std::ostream& operator<<(std::ostream& os, const Translate& rhs) {
			os << (rhs.all ? "(TranslateAll, " : "(Translate, ") //
			   << "X = " << rhs.X << ", "
			   << "Y = " << rhs.Y << ", "
			   << "Z = " << rhs.Z << ")";
			return os;
		}

		std::ostream& operator<<(std::ostream& os, const Scale& rhs) {
			os << (rhs.all ? "(ScaleAll, " : "(Scale, ") //
			   << "X = " << rhs.X << ", "
			   << "Y = " << rhs.Y << ", "
			   << "Z = " << rhs.Z << ")";
			return os;
		}

		std::ostream& operator<<(std::ostream& os, const Rotate& rhs) {
			os << (rhs.all ? "(RotateAll, " : "(Rotate, ") //
			   << "X = " << rhs.X << ", "
			   << "Y = " << rhs.Y << ", "
			   << "Z = " << rhs.Z << ", "
			   << "Angle = " << rhs.Angle << ")";
			return os;
		}

		std::ostream& operator<<(std::ostream& os, const Shear& rhs) {
			os << (rhs.all ? "(ShearAll, " : "(Shear, ") //
			   << "dX = " << rhs.dX << ", "
			   << "dY = " << rhs.dY << ", "
			   << "dZ = " << rhs.dZ << ", "
			   << "sX = " << rhs.sX << ", "
			   << "sY = " << rhs.sY << ", "
			   << "sZ = " << rhs.sZ << ", "
			   << "r = " << rhs.r << ")";
			return os;
		}

		std::ostream& operator<<(std::ostream& os, const SetColor& rhs) {
			os << "(SetColor, line = " << rhs.line << ", " //
			   << "Red = " << int(rhs.Red) << ", "
			   << "Green = " << int(rhs.Green) << ", "
			   << "Blue = " << int(rhs.Blue) << ", "
			   << "Alpha = " << int(rhs.Alpha) << ")";
			return os;
		}

		std::ostream& operator<<(std::ostream& os, const SetEmissiveColor& rhs) {
			os << "(SetEmissiveColor, line = " << rhs.line << ", " //
			   << "Red = " << int(rhs.Red) << ", "
			   << "Green = " << int(rhs.Green) << ", "
			   << "Blue = " << int(rhs.Blue) << ")";
			return os;
		}

		std::ostream& operator<<(std::ostream& os, const SetBlendMode& rhs) {
			os << "(SetBlendMode, line = " << rhs.line << ", " //
			   << "BlendMode = " << (rhs.BlendMode == mesh_t::BlendMode_t::Normal ? "Normal"s : "Additive"s) << ", "
			   << "GlowHalfDistance = " << rhs.GlowHalfDistance << ", "
			   << "GlowAttenuationMode = "
			   << (rhs.GlowAttenuationMode == mesh_t::GlowAttenuationMode_t::DivideExponent2 ? "DivideExponent2"s
			                                                                                 : "DivideExponent4"s)
			   << ")";
			return os;
		}

		std::ostream& operator<<(std::ostream& os, const LoadTexture& rhs) {
			os << "(LoadTexture, line = " << rhs.line << ", "
			   << "DaytimeTexture = \"" << rhs.DaytimeTexture << "\", "
			   << "NighttimeTexture = \"" << rhs.NighttimeTexture << "\")";
			return os;
		}

		std::ostream& operator<<(std::ostream& os, const SetDecalTransparentColor& rhs) {
			os << "(SetDecalTransparentColor, line = " << rhs.line << ", " //
			   << "Red = " << int(rhs.Red) << ", "
			   << "Green = " << int(rhs.Green) << ", "
			   << "Blue = " << int(rhs.Blue) << ")";
			return os;
		}

		std::ostream& operator<<(std::ostream& os, const SetTextureCoordinates& rhs) {
			os << "(SetTextureCoordinates, line = " << rhs.line << ", " //
			   << "VertexIndex = " << rhs.VertexIndex << ", "
			   << "X = " << rhs.X << ", "
			   << "Y = " << rhs.Y << ")";
			return os;
		}
	} // namespace instructions
} // namespace b3d_csv_object
} // namespace parsers
