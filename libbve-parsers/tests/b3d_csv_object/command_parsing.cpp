#include "b3d_csv_object.hpp"
#include "test_macro_util.hpp"
#include <doctest.h>

using namespace std::string_literals;

// ReSharper disable once CppInconsistentNaming
namespace b3d = bve::parsers::b3d_csv_object;

TEST_SUITE_BEGIN("libparsers - b3d_csv_object");

TEST_CASE("libparsers - b3d_csv_object - commands - mesh builder") {
	b3d::instruction_list result;
	SUBCASE("b3d") {
		result = b3d::create_instructions("[meshbuilder]", b3d::file_type::b3d);
	}
	SUBCASE("csv") {
		result = b3d::create_instructions("createmeshbuilder", b3d::file_type::csv);
	}

	REQUIRE_EQ(result.size(), 1);
	COMPARE_VARIANT_NODES(result[0], b3d::instructions::create_mesh_builder{});
}

TEST_CASE("libparsers - b3d_csv_object - commands - vertex") {
	b3d::instruction_list result;
	SUBCASE("b3d") {
		result = b3d::create_instructions("Vertex 1, 2, 3, 4, 5, 6", b3d::file_type::b3d);
	}
	SUBCASE("csv") {
		result = b3d::create_instructions("AddVertex, 1, 2, 3, 4, 5, 6", b3d::file_type::csv);
	}

	REQUIRE_EQ(result.size(), 1);
	b3d::instructions::add_vertex vert;
	vert.v_x = 1;
	vert.v_y = 2;
	vert.v_z = 3;
	vert.n_x = 4;
	vert.n_y = 5;
	vert.n_z = 6;
	COMPARE_VARIANT_NODES_MEMBER(result[0], vert, v_x);
	COMPARE_VARIANT_NODES_MEMBER(result[0], vert, v_y);
	COMPARE_VARIANT_NODES_MEMBER(result[0], vert, v_z);
	COMPARE_VARIANT_NODES_MEMBER(result[0], vert, n_x);
	COMPARE_VARIANT_NODES_MEMBER(result[0], vert, n_y);
	COMPARE_VARIANT_NODES_MEMBER(result[0], vert, n_z);
}

TEST_CASE("libparsers - b3d_csv_object - commands - face") {
	b3d::instruction_list result;
	SUBCASE("b3d") {
		result = b3d::create_instructions("Face 1, 2, 3", b3d::file_type::b3d);
	}
	SUBCASE("csv") {
		result = b3d::create_instructions("AddFace, 1, 2, 3", b3d::file_type::csv);
	}

	REQUIRE_EQ(result.size(), 1);
	b3d::instructions::add_face face;
	face.vertices = {1, 2, 3};
	face.side_count = b3d::instructions::sides::one;
	COMPARE_VARIANT_NODES_MEMBER(result[0], face, vertices);
	COMPARE_VARIANT_NODES_MEMBER(result[0], face, side_count);
}

TEST_CASE("libparsers - b3d_csv_object - commands - face2") {
	b3d::instruction_list result;
	SUBCASE("b3d") {
		result = b3d::create_instructions("Face2 1, 2, 3", b3d::file_type::b3d);
	}
	SUBCASE("csv") {
		result = b3d::create_instructions("AddFace2, 1, 2, 3", b3d::file_type::csv);
	}

	REQUIRE_EQ(result.size(), 1);
	b3d::instructions::add_face face;
	face.vertices = {1, 2, 3};
	face.side_count = b3d::instructions::sides::two;
	COMPARE_VARIANT_NODES_MEMBER(result[0], face, vertices);
	COMPARE_VARIANT_NODES_MEMBER(result[0], face, side_count);
}

TEST_CASE("libparsers - b3d_csv_object - commands - cube") {
	b3d::instruction_list result;
	SUBCASE("b3d") {
		result = b3d::create_instructions("Cube 1, 2, 3", b3d::file_type::b3d);
	}
	SUBCASE("csv") {
		result = b3d::create_instructions("Cube, 1, 2, 3", b3d::file_type::csv);
	}

	REQUIRE_EQ(result.size(), 1);
	b3d::instructions::cube cube;
	cube.half_width = 1;
	cube.half_height = 2;
	cube.half_depth = 3;
	COMPARE_VARIANT_NODES_MEMBER(result[0], cube, half_width);
	COMPARE_VARIANT_NODES_MEMBER(result[0], cube, half_height);
	COMPARE_VARIANT_NODES_MEMBER(result[0], cube, half_depth);
}

TEST_CASE("libparsers - b3d_csv_object - commands - cylinder") {
	b3d::instruction_list result;
	SUBCASE("b3d") {
		result = b3d::create_instructions("Cylinder 1, 2, 3, 4", b3d::file_type::b3d);
	}
	SUBCASE("csv") {
		result = b3d::create_instructions("Cylinder, 1, 2, 3, 4", b3d::file_type::csv);
	}

	REQUIRE_EQ(result.size(), 1);
	b3d::instructions::cylinder cylinder;
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
	b3d::instruction_list result = b3d::create_instructions("generatenormals", b3d::file_type::csv);

	REQUIRE_EQ(result.size(), 0);
}

TEST_CASE("libparsers - b3d_csv_object - commands - texture") {
	b3d::instruction_list result = b3d::create_instructions("[texture]", b3d::file_type::b3d);

	REQUIRE_EQ(result.size(), 0);
}

TEST_CASE("libparsers - b3d_csv_object - commands - translate") {
	b3d::instruction_list result;
	auto applies_to = b3d::instructions::apply_to::single_mesh;
	SUBCASE("regular") {
		SUBCASE("b3d") {
			result = b3d::create_instructions("Translate 1, 2, 3", b3d::file_type::b3d);
		}
		SUBCASE("csv") {
			result = b3d::create_instructions("Translate, 1, 2, 3", b3d::file_type::csv);
		}
	}
	SUBCASE("all") {
		applies_to = b3d::instructions::apply_to::all_meshes;
		SUBCASE("b3d") {
			result = b3d::create_instructions("TranslateAll 1, 2, 3", b3d::file_type::b3d);
		}
		SUBCASE("csv") {
			result = b3d::create_instructions("TranslateAll, 1, 2, 3", b3d::file_type::csv);
		}
	}

	REQUIRE_EQ(result.size(), 1);
	b3d::instructions::translate translate;
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
	b3d::instruction_list result;
	auto applies_to = b3d::instructions::apply_to::single_mesh;
	SUBCASE("regular") {
		SUBCASE("b3d") {
			result = b3d::create_instructions("Scale 1, 2, 3", b3d::file_type::b3d);
		}
		SUBCASE("csv") {
			result = b3d::create_instructions("Scale, 1, 2, 3", b3d::file_type::csv);
		}
	}
	SUBCASE("all") {
		applies_to = b3d::instructions::apply_to::all_meshes;
		SUBCASE("b3d") {
			result = b3d::create_instructions("ScaleAll 1, 2, 3", b3d::file_type::b3d);
		}
		SUBCASE("csv") {
			result = b3d::create_instructions("ScaleAll, 1, 2, 3", b3d::file_type::csv);
		}
	}

	REQUIRE_EQ(result.size(), 1);
	b3d::instructions::scale scale;
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
	b3d::instruction_list result;
	auto applies_to = b3d::instructions::apply_to::single_mesh;
	SUBCASE("regular") {
		SUBCASE("b3d") {
			result = b3d::create_instructions("Rotate 1, 2, 3, 4", b3d::file_type::b3d);
		}
		SUBCASE("csv") {
			result = b3d::create_instructions("Rotate, 1, 2, 3, 4", b3d::file_type::csv);
		}
	}
	SUBCASE("all") {
		applies_to = b3d::instructions::apply_to::all_meshes;
		SUBCASE("b3d") {
			result = b3d::create_instructions("RotateAll 1, 2, 3, 4", b3d::file_type::b3d);
		}
		SUBCASE("csv") {
			result = b3d::create_instructions("RotateAll, 1, 2, 3, 4", b3d::file_type::csv);
		}
	}

	REQUIRE_EQ(result.size(), 1);
	b3d::instructions::rotate rotate;
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
	b3d::instruction_list result;
	auto applies_to = b3d::instructions::apply_to::single_mesh;
	SUBCASE("regular") {
		SUBCASE("b3d") {
			result = b3d::create_instructions("Shear 1, 2, 3, 4, 5, 6, 7", b3d::file_type::b3d);
		}
		SUBCASE("csv") {
			result = b3d::create_instructions("Shear, 1, 2, 3, 4, 5, 6, 7", b3d::file_type::csv);
		}
	}
	SUBCASE("all") {
		applies_to = b3d::instructions::apply_to::all_meshes;
		SUBCASE("b3d") {
			result = b3d::create_instructions("ShearAll 1, 2, 3, 4, 5, 6, 7", b3d::file_type::b3d);
		}
		SUBCASE("csv") {
			result = b3d::create_instructions("ShearAll, 1, 2, 3, 4, 5, 6, 7", b3d::file_type::csv);
		}
	}

	REQUIRE_EQ(result.size(), 1);
	b3d::instructions::shear shear;
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
	b3d::instruction_list result;
	auto applies_to = b3d::instructions::apply_to::single_mesh;
	SUBCASE("regular") {
		SUBCASE("b3d") {
			result = b3d::create_instructions("Mirror 1, 2, 3", b3d::file_type::b3d);
		}
		SUBCASE("csv") {
			result = b3d::create_instructions("Mirror, 1, 2, 3", b3d::file_type::csv);
		}
	}
	SUBCASE("all") {
		applies_to = b3d::instructions::apply_to::all_meshes;
		SUBCASE("b3d") {
			result = b3d::create_instructions("MirrorAll 1, 2, 3", b3d::file_type::b3d);
		}
		SUBCASE("csv") {
			result = b3d::create_instructions("MirrorAll, 1, 2, 3", b3d::file_type::csv);
		}
	}

	REQUIRE_EQ(result.size(), 1);
	b3d::instructions::mirror mirror;
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
	b3d::instruction_list result;
	SUBCASE("b3d") {
		result = b3d::create_instructions("Color 1, 2, 3, 4", b3d::file_type::b3d);
	}
	SUBCASE("csv") {
		result = b3d::create_instructions("SetColor, 1, 2, 3, 4", b3d::file_type::csv);
	}

	REQUIRE_EQ(result.size(), 1);
	b3d::instructions::set_color color;
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
	b3d::instruction_list result;
	SUBCASE("b3d") {
		result = b3d::create_instructions("EmissiveColor 1, 2, 3", b3d::file_type::b3d);
	}
	SUBCASE("csv") {
		result = b3d::create_instructions("SetEmissiveColor, 1, 2, 3", b3d::file_type::csv);
	}

	REQUIRE_EQ(result.size(), 1);
	b3d::instructions::set_emissive_color emissive_color;
	emissive_color.red = 1;
	emissive_color.green = 2;
	emissive_color.blue = 3;
	COMPARE_VARIANT_NODES_MEMBER(result[0], emissive_color, red);
	COMPARE_VARIANT_NODES_MEMBER(result[0], emissive_color, green);
	COMPARE_VARIANT_NODES_MEMBER(result[0], emissive_color, blue);
}

TEST_CASE("libparsers - b3d_csv_object - commands - blend mode") {
	b3d::instruction_list result;
	b3d::instructions::set_blend_mode blend_mode;

	blend_mode.glow_half_distance = 2;

	// (b3d/csv) * (normal/additive) * (divide_exponent2/divide_exponent4)
	SUBCASE("b3d") {
		SUBCASE("normal blend") {
			SUBCASE("divide exponent 2") {
				blend_mode.blend_mode = bve::parsers::b3d_csv_object::mesh_t::normal;
				blend_mode.glow_attenuation_mode =
				    bve::parsers::b3d_csv_object::mesh_t::divide_exponent2;
				result = b3d::create_instructions("BlendMode, Normal, 2, DivideExponent2",
				                                  b3d::file_type::b3d);
			}
			SUBCASE("divide exponent 4") {
				blend_mode.blend_mode = bve::parsers::b3d_csv_object::mesh_t::normal;
				blend_mode.glow_attenuation_mode =
				    bve::parsers::b3d_csv_object::mesh_t::divide_exponent4;
				result = b3d::create_instructions("BlendMode, Normal, 2, DivideExponent4",
				                                  b3d::file_type::b3d);
			}
		}
		SUBCASE("additive blend") {
			SUBCASE("divide exponent 2") {
				blend_mode.blend_mode = bve::parsers::b3d_csv_object::mesh_t::additive;
				blend_mode.glow_attenuation_mode =
				    bve::parsers::b3d_csv_object::mesh_t::divide_exponent2;
				result = b3d::create_instructions("BlendMode, Additive, 2, DivideExponent2",
				                                  b3d::file_type::b3d);
			}
			SUBCASE("divide exponent 4") {
				blend_mode.blend_mode = bve::parsers::b3d_csv_object::mesh_t::additive;
				blend_mode.glow_attenuation_mode =
				    bve::parsers::b3d_csv_object::mesh_t::divide_exponent4;
				result = b3d::create_instructions("BlendMode, Additive, 2, DivideExponent4",
				                                  b3d::file_type::b3d);
			}
		}
	}
	SUBCASE("csv") {
		SUBCASE("normal blend") {
			SUBCASE("divide exponent 2") {
				blend_mode.blend_mode = bve::parsers::b3d_csv_object::mesh_t::normal;
				blend_mode.glow_attenuation_mode =
				    bve::parsers::b3d_csv_object::mesh_t::divide_exponent2;
				result = b3d::create_instructions("SetBlendMode, Normal, 2, DivideExponent2",
				                                  b3d::file_type::b3d);
			}
			SUBCASE("divide exponent 4") {
				blend_mode.blend_mode = bve::parsers::b3d_csv_object::mesh_t::normal;
				blend_mode.glow_attenuation_mode =
				    bve::parsers::b3d_csv_object::mesh_t::divide_exponent4;
				result = b3d::create_instructions("SetBlendMode, Normal, 2, DivideExponent4",
				                                  b3d::file_type::b3d);
			}
		}
		SUBCASE("additive blend") {
			SUBCASE("divide exponent 2") {
				blend_mode.blend_mode = bve::parsers::b3d_csv_object::mesh_t::additive;
				blend_mode.glow_attenuation_mode =
				    bve::parsers::b3d_csv_object::mesh_t::divide_exponent2;
				result = b3d::create_instructions("SetBlendMode, Additive, 2, DivideExponent2",
				                                  b3d::file_type::b3d);
			}
			SUBCASE("divide exponent 4") {
				blend_mode.blend_mode = bve::parsers::b3d_csv_object::mesh_t::additive;
				blend_mode.glow_attenuation_mode =
				    bve::parsers::b3d_csv_object::mesh_t::divide_exponent4;
				result = b3d::create_instructions("SetBlendMode, Additive, 2, DivideExponent4",
				                                  b3d::file_type::b3d);
			}
		}
	}

	REQUIRE_EQ(result.size(), 1);
	COMPARE_VARIANT_NODES_MEMBER(result[0], blend_mode, blend_mode);
	COMPARE_VARIANT_NODES_MEMBER(result[0], blend_mode, glow_half_distance);
	COMPARE_VARIANT_NODES_MEMBER(result[0], blend_mode, glow_attenuation_mode);
}

TEST_CASE("libparsers - b3d_csv_object - commands - load") {
	b3d::instruction_list result;
	SUBCASE("b3d") {
		result = b3d::create_instructions("Load pathy/mcpathface.png, nighty/mcnightyface.png",
		                                  b3d::file_type::b3d);
	}
	SUBCASE("csv") {
		result =
		    b3d::create_instructions("LoadTexture, pathy/mcpathface.png, nighty/mcnightyface.png",
		                             b3d::file_type::csv);
	}

	REQUIRE_EQ(result.size(), 1);
	b3d::instructions::load_texture texture;
	texture.daytime_texture = "pathy/mcpathface.png"s;
	texture.nighttime_texture = "nighty/mcnightyface.png"s;
	COMPARE_VARIANT_NODES_MEMBER(result[0], texture, daytime_texture);
	COMPARE_VARIANT_NODES_MEMBER(result[0], texture, nighttime_texture);
}

TEST_CASE("libparsers - b3d_csv_object - commands - decal transparent color") {
	b3d::instruction_list result;
	SUBCASE("b3d") {
		result = b3d::create_instructions("Transparent 1, 2, 3", b3d::file_type::b3d);
	}
	SUBCASE("csv") {
		result = b3d::create_instructions("SetDecalTransparentColor, 1, 2, 3", b3d::file_type::csv);
	}

	REQUIRE_EQ(result.size(), 1);
	b3d::instructions::set_decal_transparent_color decal_transparent_color;
	decal_transparent_color.red = 1;
	decal_transparent_color.green = 2;
	decal_transparent_color.blue = 3;
	COMPARE_VARIANT_NODES_MEMBER(result[0], decal_transparent_color, red);
	COMPARE_VARIANT_NODES_MEMBER(result[0], decal_transparent_color, green);
	COMPARE_VARIANT_NODES_MEMBER(result[0], decal_transparent_color, blue);
}

TEST_CASE("libparsers - b3d_csv_object - commands - texture coordinates") {
	b3d::instruction_list result;
	SUBCASE("b3d") {
		result = b3d::create_instructions("Coordinates 1, 2, 3", b3d::file_type::b3d);
	}
	SUBCASE("csv") {
		result = b3d::create_instructions("SetTextureCoordinates, 1, 2, 3", b3d::file_type::csv);
	}

	REQUIRE_EQ(result.size(), 1);
	b3d::instructions::set_texture_coordinates texture_coordinates;
	texture_coordinates.vertex_index = 1;
	texture_coordinates.x = 2;
	texture_coordinates.y = 3;
	COMPARE_VARIANT_NODES_MEMBER(result[0], texture_coordinates, vertex_index);
	COMPARE_VARIANT_NODES_MEMBER(result[0], texture_coordinates, x);
	COMPARE_VARIANT_NODES_MEMBER(result[0], texture_coordinates, y);
}

TEST_SUITE_END();
