#include "csv_object.hpp"

namespace parsers {
namespace csv_object {
	namespace instructions {

		std::ostream& operator<<(std::ostream& os, const CreateMeshBuilder& rhs) {
			(void) rhs;
			os << "(CreateMeshBuilder)";
			return os;
		}

		std::ostream& operator<<(std::ostream& os, const AddVertex& rhs) {
			os << "(AddVertex, "
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
			os << (rhs.all ? "(TranslateAll" : "(Translate") //
			   << "X = " << rhs.X << ", "
			   << "Y = " << rhs.Y << ", "
			   << "Z = " << rhs.Z << ")";
			return os;
		}

		std::ostream& operator<<(std::ostream& os, const Scale& rhs) {
			os << (rhs.all ? "(ScaleAll" : "(Scale") //
			   << "X = " << rhs.X << ", "
			   << "Y = " << rhs.Y << ", "
			   << "Z = " << rhs.Z << ")";
			return os;
		}

		std::ostream& operator<<(std::ostream& os, const Rotate& rhs) {
			os << (rhs.all ? "(RotateAll" : "(Rotate") //
			   << "X = " << rhs.X << ", "
			   << "Y = " << rhs.Y << ", "
			   << "Z = " << rhs.Z << ", "
			   << "Angle = " << rhs.Angle << ")";
			return os;
		}

		std::ostream& operator<<(std::ostream& os, const Shear& rhs) {
			os << (rhs.all ? "(ShearAll" : "(Shear") //
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
			os << "(SetColor" //
			   << "Red = " << rhs.Red << ", "
			   << "Green = " << rhs.Green << ", "
			   << "Blue = " << rhs.Blue << ", "
			   << "Alpha = " << rhs.Alpha << ")";
			return os;
		}

		std::ostream& operator<<(std::ostream& os, const SetEmissiveColor& rhs) {
			os << "(SetEmissiveColor" //
			   << "Red = " << rhs.Red << ", "
			   << "Green = " << rhs.Green << ", "
			   << "Blue = " << rhs.Blue << ")";
			return os;
		}

		std::ostream& operator<<(std::ostream& os, const SetBlendMode& rhs) {
			os << "(SetBlendMode" //
			   << "BlendMode = " << (rhs.BlendMode == SetBlendMode::Normal ? "Normal" : "Additive") << ", "
			   << "GlowHalfDistance = " << rhs.GlowHalfDistance << ", "
			   << "GlowAttenuationMode = "
			   << (rhs.GlowAttenuationMode == SetBlendMode::DivideExponent2 ? "DivideExponent2" : "DivideExponent4")
			   << ")";
			return os;
		}

		std::ostream& operator<<(std::ostream& os, const LoadTexture& rhs) {
			os << "(LoadTexture, "
			   << "DaytimeTexture = \"" << rhs.DaytimeTexture << "\", "
			   << "NighttimeTexture = \"" << rhs.NighttimeTexture << "\")";
			return os;
		}

		std::ostream& operator<<(std::ostream& os, const SetDecalTransparentColor& rhs) {
			os << "(SetDecalTransparentColor" //
			   << "Red = " << rhs.Red << ", "
			   << "Green = " << rhs.Green << ", "
			   << "Blue = " << rhs.Blue << ")";
			return os;
		}

		std::ostream& operator<<(std::ostream& os, const SetTextureCoordinates& rhs) {
			os << "(SetTextureCoordinates" //
			   << "VertexIndex = " << rhs.VertexIndex << ", "
			   << "X = " << rhs.X << ", "
			   << "Y = " << rhs.Y << ")";
			return os;
		}
	} // namespace instructions
} // namespace csv_object
} // namespace parsers