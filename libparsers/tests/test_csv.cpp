#include "csv.hpp"
#include <iostream>

void test_csv() {
	const char* test_csv_ptr =
	    "CreateMeshBuilder,,,,, \n"
	    "AddVertex, vX, vY, vZ, nX, nY, nZ\n"
	    "AddFace, v1, v2, v3, ..., vn\n"
	    "AddFace2, v1, v2, v3, ..., vn\n"
	    "Cube, HalfWidth, HalfHeight, HalfDepth\n"
	    "Cylinder, n, UpperRadius, LowerRadius, Height\n"
	    "Translate, X, Y, Z\n"
	    "TranslateAll, X, Y, Z\n"
	    "Scale, X, Y, Z\n"
	    "ScaleAll, X, Y, Z\n"
	    "Rotate, X, Y, Z, Angle\n"
	    "RotateAll, X, Y, Z, Angle\n"
	    "Shear, dX, dY, dZ, sX, sY, sZ, Ratio\n"
	    "ShearAll, dX, dY, dZ, sX, sY, sZ, Ratio\n"
	    "SetColor, Red, Green, Blue, Alpha\n"
	    "SetEmissiveColor, Red, Green, Blue\n"
	    "SetBlendMode, { Normal | Additive }, GlowHalfDistance, { DivideExponent2 | DivideExponent4 }\n"
	    "LoadTexture, DaytimeTexture, NighttimeTexture\n"
	    "SetDecalTransparentColor, Red, Green, Blue\n"
	    "SetTextureCoordinates, VertexIndex, X, Y\n";

	std::string test_csv(test_csv_ptr);

	auto result = parsers::csv::parse(test_csv);

	std::cout << result;
}