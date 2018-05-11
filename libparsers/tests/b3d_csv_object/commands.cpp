#include "b3d_csv_object.hpp"
#include "test_macro_util.hpp"
#include <doctest.h>

namespace b3d = parsers::b3d_csv_object;

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
	face.two = false;
	COMPARE_VARIANT_NODES_MEMBER(result[0], face, vertices);
	COMPARE_VARIANT_NODES_MEMBER(result[0], face, two);
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
	face.two = true;
	COMPARE_VARIANT_NODES_MEMBER(result[0], face, vertices);
	COMPARE_VARIANT_NODES_MEMBER(result[0], face, two);
}

TEST_CASE("libparsers - b3d_csv_object - commands - translate") {
	b3d::instruction_list result;
	bool all = false;
	SUBCASE("regular") {
		SUBCASE("b3d") {
			result = b3d::create_instructions("Translate 1, 2, 3", b3d::file_type::b3d);
		}
		SUBCASE("csv") {
			result = b3d::create_instructions("Translate, 1, 2, 3", b3d::file_type::csv);
		}
	}
	SUBCASE("all") {
		all = true;
		SUBCASE("b3d") {
			result = b3d::create_instructions("TranslateAll 1, 2, 3", b3d::file_type::b3d);
		}
		SUBCASE("csv") {
			result = b3d::create_instructions("TranslateAll, 1, 2, 3", b3d::file_type::csv);
		}
	}

	REQUIRE_EQ(result.size(), 1);
	b3d::instructions::translate translate;
	translate.all = all;
	translate.x = 1;
	translate.y = 2;
	translate.z = 3;
	COMPARE_VARIANT_NODES_MEMBER(result[0], translate, all);
	COMPARE_VARIANT_NODES_MEMBER(result[0], translate, x);
	COMPARE_VARIANT_NODES_MEMBER(result[0], translate, y);
	COMPARE_VARIANT_NODES_MEMBER(result[0], translate, z);
}

TEST_CASE("libparsers - b3d_csv_object - commands - scale") {
	b3d::instruction_list result;
	bool all = false;
	SUBCASE("regular") {
		SUBCASE("b3d") {
			result = b3d::create_instructions("Scale 1, 2, 3", b3d::file_type::b3d);
		}
		SUBCASE("csv") {
			result = b3d::create_instructions("Scale, 1, 2, 3", b3d::file_type::csv);
		}
	}
	SUBCASE("all") {
		all = true;
		SUBCASE("b3d") {
			result = b3d::create_instructions("ScaleAll 1, 2, 3", b3d::file_type::b3d);
		}
		SUBCASE("csv") {
			result = b3d::create_instructions("ScaleAll, 1, 2, 3", b3d::file_type::csv);
		}
	}

	REQUIRE_EQ(result.size(), 1);
	b3d::instructions::scale scale;
	scale.all = all;
	scale.x = 1;
	scale.y = 2;
	scale.z = 3;
	COMPARE_VARIANT_NODES_MEMBER(result[0], scale, all);
	COMPARE_VARIANT_NODES_MEMBER(result[0], scale, x);
	COMPARE_VARIANT_NODES_MEMBER(result[0], scale, y);
	COMPARE_VARIANT_NODES_MEMBER(result[0], scale, z);
}

TEST_CASE("libparsers - b3d_csv_object - commands - rotate") {
	b3d::instruction_list result;
	bool all = false;
	SUBCASE("regular") {
		SUBCASE("b3d") {
			result = b3d::create_instructions("Rotate 1, 2, 3, 4", b3d::file_type::b3d);
		}
		SUBCASE("csv") {
			result = b3d::create_instructions("Rotate, 1, 2, 3, 4", b3d::file_type::csv);
		}
	}
	SUBCASE("all") {
		all = true;
		SUBCASE("b3d") {
			result = b3d::create_instructions("RotateAll 1, 2, 3, 4", b3d::file_type::b3d);
		}
		SUBCASE("csv") {
			result = b3d::create_instructions("RotateAll, 1, 2, 3, 4", b3d::file_type::csv);
		}
	}

	REQUIRE_EQ(result.size(), 1);
	b3d::instructions::rotate rotate;
	rotate.all = all;
	rotate.x = 1;
	rotate.y = 2;
	rotate.z = 3;
	rotate.angle = 4;
	COMPARE_VARIANT_NODES_MEMBER(result[0], rotate, all);
	COMPARE_VARIANT_NODES_MEMBER(result[0], rotate, x);
	COMPARE_VARIANT_NODES_MEMBER(result[0], rotate, y);
	COMPARE_VARIANT_NODES_MEMBER(result[0], rotate, z);
	COMPARE_VARIANT_NODES_MEMBER(result[0], rotate, angle);
}

TEST_SUITE_END();