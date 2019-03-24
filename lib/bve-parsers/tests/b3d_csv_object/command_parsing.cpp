#include "b3d_csv_object/b3d_csv_object.hpp"
#include <doctest/doctest.h>
#include <util/testing/variant_macros.hpp>

using namespace std::string_literals;

// ReSharper disable once CppInconsistentNaming
namespace b3d = bve::parsers::b3d_csv_object;

TEST_SUITE_BEGIN("libparsers - b3d_csv_object");

TEST_CASE("libparsers - b3d_csv_object - commands - mesh builder") {
	b3d::InstructionList result;
	SUBCASE("b3d") {
		result = create_instructions("[meshbuilder]", b3d::FileType::b3d);
	}
	SUBCASE("csv") {
		result = create_instructions("createmeshbuilder", b3d::FileType::csv);
	}

	REQUIRE_EQ(result.size(), 1);
	COMPARE_VARIANT_NODES(result[0], b3d::instructions::CreateMeshBuilder{});
}

TEST_CASE("libparsers - b3d_csv_object - commands - vertex") {
	b3d::InstructionList result;
	SUBCASE("b3d") {
		result = create_instructions("Vertex 1, 2, 3, 4, 5, 6", b3d::FileType::b3d);
	}
	SUBCASE("csv") {
		result = create_instructions("AddVertex, 1, 2, 3, 4, 5, 6", b3d::FileType::csv);
	}

	REQUIRE_EQ(result.size(), 1);
	b3d::instructions::AddVertex vertex;
	vertex.v_x = 1;
	vertex.v_y = 2;
	vertex.v_z = 3;
	vertex.n_x = 4;
	vertex.n_y = 5;
	vertex.n_z = 6;
	COMPARE_VARIANT_NODES_MEMBER(result[0], vertex, v_x);
	COMPARE_VARIANT_NODES_MEMBER(result[0], vertex, v_y);
	COMPARE_VARIANT_NODES_MEMBER(result[0], vertex, v_z);
	COMPARE_VARIANT_NODES_MEMBER(result[0], vertex, n_x);
	COMPARE_VARIANT_NODES_MEMBER(result[0], vertex, n_y);
	COMPARE_VARIANT_NODES_MEMBER(result[0], vertex, n_z);
}

TEST_CASE("libparsers - b3d_csv_object - commands - face") {
	b3d::InstructionList result;
	SUBCASE("b3d") {
		result = create_instructions("Face 1, 2, 3", b3d::FileType::b3d);
	}
	SUBCASE("csv") {
		result = create_instructions("AddFace, 1, 2, 3", b3d::FileType::csv);
	}

	REQUIRE_EQ(result.size(), 1);
	b3d::instructions::AddFace face;
	face.vertices = {1, 2, 3};
	face.side_count = b3d::instructions::Sides::one;
	COMPARE_VARIANT_NODES_MEMBER(result[0], face, vertices);
	COMPARE_VARIANT_NODES_MEMBER(result[0], face, side_count);
}

TEST_CASE("libparsers - b3d_csv_object - commands - face2") {
	b3d::InstructionList result;
	SUBCASE("b3d") {
		result = create_instructions("Face2 1, 2, 3", b3d::FileType::b3d);
	}
	SUBCASE("csv") {
		result = create_instructions("AddFace2, 1, 2, 3", b3d::FileType::csv);
	}

	REQUIRE_EQ(result.size(), 1);
	b3d::instructions::AddFace face;
	face.vertices = {1, 2, 3};
	face.side_count = b3d::instructions::Sides::two;
	COMPARE_VARIANT_NODES_MEMBER(result[0], face, vertices);
	COMPARE_VARIANT_NODES_MEMBER(result[0], face, side_count);
}

TEST_CASE("libparsers - b3d_csv_object - commands - cube") {
	b3d::InstructionList result;
	SUBCASE("b3d") {
		result = create_instructions("Cube 1, 2, 3", b3d::FileType::b3d);
	}
	SUBCASE("csv") {
		result = create_instructions("Cube, 1, 2, 3", b3d::FileType::csv);
	}

	REQUIRE_EQ(result.size(), 1);
	b3d::instructions::Cube cube;
	cube.half_width = 1;
	cube.half_height = 2;
	cube.half_depth = 3;
	COMPARE_VARIANT_NODES_MEMBER(result[0], cube, half_width);
	COMPARE_VARIANT_NODES_MEMBER(result[0], cube, half_height);
	COMPARE_VARIANT_NODES_MEMBER(result[0], cube, half_depth);
}

TEST_CASE("libparsers - b3d_csv_object - commands - cylinder") {
	b3d::InstructionList result;
	SUBCASE("b3d") {
		result = create_instructions("Cylinder 1, 2, 3, 4", b3d::FileType::b3d);
	}
	SUBCASE("csv") {
		result = create_instructions("Cylinder, 1, 2, 3, 4", b3d::FileType::csv);
	}

	REQUIRE_EQ(result.size(), 1);
	b3d::instructions::Cylinder cylinder;
	cylinder.sides = 1;
	cylinder.upper_radius = 2;
	cylinder.lower_radius = 3;
	cylinder.height = 4;
	COMPARE_VARIANT_NODES_MEMBER(result[0], cylinder, sides);
	COMPARE_VARIANT_NODES_MEMBER(result[0], cylinder, upper_radius);
	COMPARE_VARIANT_NODES_MEMBER(result[0], cylinder, lower_radius);
	COMPARE_VARIANT_NODES_MEMBER(result[0], cylinder, height);
}

TEST_CASE("libparsers - b3d_csv_object - commands - generate normals") {
	b3d::InstructionList result = create_instructions("generatenormals", b3d::FileType::csv);

	REQUIRE_EQ(result.size(), 0);
}

TEST_CASE("libparsers - b3d_csv_object - commands - texture") {
	b3d::InstructionList result = create_instructions("[texture]", b3d::FileType::b3d);

	REQUIRE_EQ(result.size(), 0);
}

TEST_CASE("libparsers - b3d_csv_object - commands - translate") {
	b3d::InstructionList result;
	auto applies_to = b3d::instructions::ApplyTo::single_mesh;
	SUBCASE("regular") {
		SUBCASE("b3d") {
			result = create_instructions("Translate 1, 2, 3", b3d::FileType::b3d);
		}
		SUBCASE("csv") {
			result = create_instructions("Translate, 1, 2, 3", b3d::FileType::csv);
		}
	}
	SUBCASE("all") {
		applies_to = b3d::instructions::ApplyTo::all_meshes;
		SUBCASE("b3d") {
			result = create_instructions("TranslateAll 1, 2, 3", b3d::FileType::b3d);
		}
		SUBCASE("csv") {
			result = create_instructions("TranslateAll, 1, 2, 3", b3d::FileType::csv);
		}
	}

	REQUIRE_EQ(result.size(), 1);
	b3d::instructions::Translate translate;
	translate.applies_to = applies_to;
	translate.x = 1;
	translate.y = 2;
	translate.z = 3;
	COMPARE_VARIANT_NODES_MEMBER(result[0], translate, applies_to);
	COMPARE_VARIANT_NODES_MEMBER(result[0], translate, x);
	COMPARE_VARIANT_NODES_MEMBER(result[0], translate, y);
	COMPARE_VARIANT_NODES_MEMBER(result[0], translate, z);
}

TEST_CASE("libparsers - b3d_csv_object - commands - scale") {
	b3d::InstructionList result;
	auto applies_to = b3d::instructions::ApplyTo::single_mesh;
	SUBCASE("regular") {
		SUBCASE("b3d") {
			result = create_instructions("Scale 1, 2, 3", b3d::FileType::b3d);
		}
		SUBCASE("csv") {
			result = create_instructions("Scale, 1, 2, 3", b3d::FileType::csv);
		}
	}
	SUBCASE("all") {
		applies_to = b3d::instructions::ApplyTo::all_meshes;
		SUBCASE("b3d") {
			result = create_instructions("ScaleAll 1, 2, 3", b3d::FileType::b3d);
		}
		SUBCASE("csv") {
			result = create_instructions("ScaleAll, 1, 2, 3", b3d::FileType::csv);
		}
	}

	REQUIRE_EQ(result.size(), 1);
	b3d::instructions::Scale scale;
	scale.applies_to = applies_to;
	scale.x = 1;
	scale.y = 2;
	scale.z = 3;
	COMPARE_VARIANT_NODES_MEMBER(result[0], scale, applies_to);
	COMPARE_VARIANT_NODES_MEMBER(result[0], scale, x);
	COMPARE_VARIANT_NODES_MEMBER(result[0], scale, y);
	COMPARE_VARIANT_NODES_MEMBER(result[0], scale, z);
}

TEST_CASE("libparsers - b3d_csv_object - commands - rotate") {
	b3d::InstructionList result;
	auto applies_to = b3d::instructions::ApplyTo::single_mesh;
	SUBCASE("regular") {
		SUBCASE("b3d") {
			result = create_instructions("Rotate 1, 2, 3, 4", b3d::FileType::b3d);
		}
		SUBCASE("csv") {
			result = create_instructions("Rotate, 1, 2, 3, 4", b3d::FileType::csv);
		}
	}
	SUBCASE("all") {
		applies_to = b3d::instructions::ApplyTo::all_meshes;
		SUBCASE("b3d") {
			result = create_instructions("RotateAll 1, 2, 3, 4", b3d::FileType::b3d);
		}
		SUBCASE("csv") {
			result = create_instructions("RotateAll, 1, 2, 3, 4", b3d::FileType::csv);
		}
	}

	REQUIRE_EQ(result.size(), 1);
	b3d::instructions::Rotate rotate;
	rotate.applies_to = applies_to;
	rotate.x = 1;
	rotate.y = 2;
	rotate.z = 3;
	rotate.angle = 4;
	COMPARE_VARIANT_NODES_MEMBER(result[0], rotate, applies_to);
	COMPARE_VARIANT_NODES_MEMBER(result[0], rotate, x);
	COMPARE_VARIANT_NODES_MEMBER(result[0], rotate, y);
	COMPARE_VARIANT_NODES_MEMBER(result[0], rotate, z);
	COMPARE_VARIANT_NODES_MEMBER(result[0], rotate, angle);
}

TEST_CASE("libparsers - b3d_csv_object - commands - shear") {
	b3d::InstructionList result;
	auto applies_to = b3d::instructions::ApplyTo::single_mesh;
	SUBCASE("regular") {
		SUBCASE("b3d") {
			result = create_instructions("Shear 1, 2, 3, 4, 5, 6, 7", b3d::FileType::b3d);
		}
		SUBCASE("csv") {
			result = create_instructions("Shear, 1, 2, 3, 4, 5, 6, 7", b3d::FileType::csv);
		}
	}
	SUBCASE("all") {
		applies_to = b3d::instructions::ApplyTo::all_meshes;
		SUBCASE("b3d") {
			result = create_instructions("ShearAll 1, 2, 3, 4, 5, 6, 7", b3d::FileType::b3d);
		}
		SUBCASE("csv") {
			result = create_instructions("ShearAll, 1, 2, 3, 4, 5, 6, 7", b3d::FileType::csv);
		}
	}

	REQUIRE_EQ(result.size(), 1);
	b3d::instructions::Shear shear;
	shear.applies_to = applies_to;
	shear.d_x = 1;
	shear.d_y = 2;
	shear.d_z = 3;
	shear.s_x = 4;
	shear.s_y = 5;
	shear.s_z = 6;
	shear.r = 7;
	COMPARE_VARIANT_NODES_MEMBER(result[0], shear, applies_to);
	COMPARE_VARIANT_NODES_MEMBER(result[0], shear, d_x);
	COMPARE_VARIANT_NODES_MEMBER(result[0], shear, d_y);
	COMPARE_VARIANT_NODES_MEMBER(result[0], shear, d_z);
	COMPARE_VARIANT_NODES_MEMBER(result[0], shear, s_x);
	COMPARE_VARIANT_NODES_MEMBER(result[0], shear, s_y);
	COMPARE_VARIANT_NODES_MEMBER(result[0], shear, s_z);
	COMPARE_VARIANT_NODES_MEMBER(result[0], shear, r);
}

TEST_CASE("libparsers - b3d_csv_object - commands - mirror") {
	b3d::InstructionList result;
	auto applies_to = b3d::instructions::ApplyTo::single_mesh;
	SUBCASE("regular") {
		SUBCASE("b3d") {
			result = create_instructions("Mirror 1, 2, 3", b3d::FileType::b3d);
		}
		SUBCASE("csv") {
			result = create_instructions("Mirror, 1, 2, 3", b3d::FileType::csv);
		}
	}
	SUBCASE("all") {
		applies_to = b3d::instructions::ApplyTo::all_meshes;
		SUBCASE("b3d") {
			result = create_instructions("MirrorAll 1, 2, 3", b3d::FileType::b3d);
		}
		SUBCASE("csv") {
			result = create_instructions("MirrorAll, 1, 2, 3", b3d::FileType::csv);
		}
	}

	REQUIRE_EQ(result.size(), 1);
	b3d::instructions::Mirror mirror;
	mirror.applies_to = applies_to;
	mirror.x = true;
	mirror.y = true;
	mirror.z = true;
	COMPARE_VARIANT_NODES_MEMBER(result[0], mirror, applies_to);
	COMPARE_VARIANT_NODES_MEMBER(result[0], mirror, x);
	COMPARE_VARIANT_NODES_MEMBER(result[0], mirror, y);
	COMPARE_VARIANT_NODES_MEMBER(result[0], mirror, z);
}

TEST_CASE("libparsers - b3d_csv_object - commands - color") {
	b3d::InstructionList result;
	SUBCASE("b3d") {
		result = create_instructions("Color 1, 2, 3, 4", b3d::FileType::b3d);
	}
	SUBCASE("csv") {
		result = create_instructions("SetColor, 1, 2, 3, 4", b3d::FileType::csv);
	}

	REQUIRE_EQ(result.size(), 1);
	b3d::instructions::SetColor color;
	color.red = 1;
	color.green = 2;
	color.blue = 3;
	color.alpha = 4;
	COMPARE_VARIANT_NODES_MEMBER(result[0], color, red);
	COMPARE_VARIANT_NODES_MEMBER(result[0], color, green);
	COMPARE_VARIANT_NODES_MEMBER(result[0], color, blue);
	COMPARE_VARIANT_NODES_MEMBER(result[0], color, alpha);
}

TEST_CASE("libparsers - b3d_csv_object - commands - emissive color") {
	b3d::InstructionList result;
	SUBCASE("b3d") {
		result = create_instructions("EmissiveColor 1, 2, 3", b3d::FileType::b3d);
	}
	SUBCASE("csv") {
		result = create_instructions("SetEmissiveColor, 1, 2, 3", b3d::FileType::csv);
	}

	REQUIRE_EQ(result.size(), 1);
	b3d::instructions::SetEmissiveColor emissive_color;
	emissive_color.red = 1;
	emissive_color.green = 2;
	emissive_color.blue = 3;
	COMPARE_VARIANT_NODES_MEMBER(result[0], emissive_color, red);
	COMPARE_VARIANT_NODES_MEMBER(result[0], emissive_color, green);
	COMPARE_VARIANT_NODES_MEMBER(result[0], emissive_color, blue);
}

TEST_CASE("libparsers - b3d_csv_object - commands - blend mode") {
	b3d::InstructionList result;
	b3d::instructions::SetBlendMode blend_mode;

	blend_mode.glow_half_distance = 2;

	// (b3d/csv) * (normal/additive) * (divide_exponent2/divide_exponent4)
	SUBCASE("b3d") {
		SUBCASE("normal blend") {
			SUBCASE("divide exponent 2") {
				blend_mode.blend_mode = bve::parsers::b3d_csv_object::Mesh::normal;
				blend_mode.glow_attenuation_mode = bve::parsers::b3d_csv_object::Mesh::divide_exponent2;
				result = create_instructions("BlendMode, Normal, 2, DivideExponent2", b3d::FileType::b3d);
			}
			SUBCASE("divide exponent 4") {
				blend_mode.blend_mode = bve::parsers::b3d_csv_object::Mesh::normal;
				blend_mode.glow_attenuation_mode = bve::parsers::b3d_csv_object::Mesh::divide_exponent4;
				result = create_instructions("BlendMode, Normal, 2, DivideExponent4", b3d::FileType::b3d);
			}
		}
		SUBCASE("additive blend") {
			SUBCASE("divide exponent 2") {
				blend_mode.blend_mode = bve::parsers::b3d_csv_object::Mesh::additive;
				blend_mode.glow_attenuation_mode = bve::parsers::b3d_csv_object::Mesh::divide_exponent2;
				result = create_instructions("BlendMode, Additive, 2, DivideExponent2", b3d::FileType::b3d);
			}
			SUBCASE("divide exponent 4") {
				blend_mode.blend_mode = bve::parsers::b3d_csv_object::Mesh::additive;
				blend_mode.glow_attenuation_mode = bve::parsers::b3d_csv_object::Mesh::divide_exponent4;
				result = create_instructions("BlendMode, Additive, 2, DivideExponent4", b3d::FileType::b3d);
			}
		}
	}
	SUBCASE("csv") {
		SUBCASE("normal blend") {
			SUBCASE("divide exponent 2") {
				blend_mode.blend_mode = bve::parsers::b3d_csv_object::Mesh::normal;
				blend_mode.glow_attenuation_mode = bve::parsers::b3d_csv_object::Mesh::divide_exponent2;
				result = create_instructions("SetBlendMode, Normal, 2, DivideExponent2", b3d::FileType::b3d);
			}
			SUBCASE("divide exponent 4") {
				blend_mode.blend_mode = bve::parsers::b3d_csv_object::Mesh::normal;
				blend_mode.glow_attenuation_mode = bve::parsers::b3d_csv_object::Mesh::divide_exponent4;
				result = create_instructions("SetBlendMode, Normal, 2, DivideExponent4", b3d::FileType::b3d);
			}
		}
		SUBCASE("additive blend") {
			SUBCASE("divide exponent 2") {
				blend_mode.blend_mode = bve::parsers::b3d_csv_object::Mesh::additive;
				blend_mode.glow_attenuation_mode = bve::parsers::b3d_csv_object::Mesh::divide_exponent2;
				result = create_instructions("SetBlendMode, Additive, 2, DivideExponent2", b3d::FileType::b3d);
			}
			SUBCASE("divide exponent 4") {
				blend_mode.blend_mode = bve::parsers::b3d_csv_object::Mesh::additive;
				blend_mode.glow_attenuation_mode = bve::parsers::b3d_csv_object::Mesh::divide_exponent4;
				result = create_instructions("SetBlendMode, Additive, 2, DivideExponent4", b3d::FileType::b3d);
			}
		}
	}

	REQUIRE_EQ(result.size(), 1);
	COMPARE_VARIANT_NODES_MEMBER(result[0], blend_mode, blend_mode);
	COMPARE_VARIANT_NODES_MEMBER(result[0], blend_mode, glow_half_distance);
	COMPARE_VARIANT_NODES_MEMBER(result[0], blend_mode, glow_attenuation_mode);
}

TEST_CASE("libparsers - b3d_csv_object - commands - load") {
	b3d::InstructionList result;
	SUBCASE("b3d") {
		result = create_instructions("Load path/day.png, path/night.png", b3d::FileType::b3d);
	}
	SUBCASE("csv") {
		result = create_instructions("LoadTexture, path/day.png, path/night.png", b3d::FileType::csv);
	}

	REQUIRE_EQ(result.size(), 1);
	b3d::instructions::LoadTexture texture;
	texture.daytime_texture = "path/day.png"s;
	texture.nighttime_texture = "path/night.png"s;
	COMPARE_VARIANT_NODES_MEMBER(result[0], texture, daytime_texture);
	COMPARE_VARIANT_NODES_MEMBER(result[0], texture, nighttime_texture);
}

TEST_CASE("libparsers - b3d_csv_object - commands - decal transparent color") {
	b3d::InstructionList result;
	SUBCASE("b3d") {
		result = create_instructions("Transparent 1, 2, 3", b3d::FileType::b3d);
	}
	SUBCASE("csv") {
		result = create_instructions("SetDecalTransparentColor, 1, 2, 3", b3d::FileType::csv);
	}

	REQUIRE_EQ(result.size(), 1);
	b3d::instructions::SetDecalTransparentColor decal_transparent_color;
	decal_transparent_color.red = 1;
	decal_transparent_color.green = 2;
	decal_transparent_color.blue = 3;
	COMPARE_VARIANT_NODES_MEMBER(result[0], decal_transparent_color, red);
	COMPARE_VARIANT_NODES_MEMBER(result[0], decal_transparent_color, green);
	COMPARE_VARIANT_NODES_MEMBER(result[0], decal_transparent_color, blue);
}

TEST_CASE("libparsers - b3d_csv_object - commands - texture coordinates") {
	b3d::InstructionList result;
	SUBCASE("b3d") {
		result = create_instructions("Coordinates 1, 2, 3", b3d::FileType::b3d);
	}
	SUBCASE("csv") {
		result = create_instructions("SetTextureCoordinates, 1, 2, 3", b3d::FileType::csv);
	}

	REQUIRE_EQ(result.size(), 1);
	b3d::instructions::SetTextureCoordinates texture_coordinates;
	texture_coordinates.vertex_index = 1;
	texture_coordinates.x = 2;
	texture_coordinates.y = 3;
	COMPARE_VARIANT_NODES_MEMBER(result[0], texture_coordinates, vertex_index);
	COMPARE_VARIANT_NODES_MEMBER(result[0], texture_coordinates, x);
	COMPARE_VARIANT_NODES_MEMBER(result[0], texture_coordinates, y);
}

TEST_SUITE_END();
