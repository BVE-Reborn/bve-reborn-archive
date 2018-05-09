#include <doctest.h>
#include <parsers/animated.hpp>

using namespace std::string_literals;

#define TEST_X_Y_SETTING(section, name, attribute)                                                 \
	TEST_CASE("libparsers - animated object - " section " - " name) {                              \
		auto string_a = "[" section "]\n" name " = 1, 2"s;                                         \
		auto result_a = parsers::animated_object::parse(string_a);                                 \
		auto string_b = "[objectA]\n[" section "]\n" name " = 1, 2"s;                              \
		auto result_b = parsers::animated_object::parse(string_b);                                 \
                                                                                                   \
		REQUIRE_EQ(result_a.includes.size(), 0);                                                   \
		REQUIRE_EQ(result_a.subobjects.size(), 1);                                                 \
		REQUIRE_EQ(result_a.dependencies.textures.size(), 0);                                      \
		CHECK_EQ(result_a.subobjects[0].attribute, glm::vec2{1, 2});                               \
                                                                                                   \
		REQUIRE_EQ(result_b.includes.size(), 0);                                                   \
		REQUIRE_EQ(result_b.subobjects.size(), 2);                                                 \
		REQUIRE_EQ(result_b.dependencies.textures.size(), 0);                                      \
		CHECK_EQ(result_b.subobjects[1].attribute, glm::vec2{1, 2});                               \
	}

#define TEST_X_Y_DAMPING(section, name, attribute)                                                 \
	TEST_CASE("libparsers - animated object - " section " - " name) {                              \
		auto string_a = "[" section "]\n" name " = 1, 2"s;                                         \
		auto result_a = parsers::animated_object::parse(string_a);                                 \
		auto string_b = "[objectA]\n[" section "]\n" name " = 1, 2"s;                              \
		auto result_b = parsers::animated_object::parse(string_b);                                 \
                                                                                                   \
		REQUIRE_EQ(result_a.includes.size(), 0);                                                   \
		REQUIRE_EQ(result_a.subobjects.size(), 1);                                                 \
		REQUIRE_EQ(result_a.dependencies.textures.size(), 0);                                      \
		CHECK_EQ(result_a.subobjects[0].attribute.frequency, 1);                                   \
		CHECK_EQ(result_a.subobjects[0].attribute.ratio, 2);                                       \
                                                                                                   \
		REQUIRE_EQ(result_b.includes.size(), 0);                                                   \
		REQUIRE_EQ(result_b.subobjects.size(), 2);                                                 \
		REQUIRE_EQ(result_b.dependencies.textures.size(), 0);                                      \
		CHECK_EQ(result_b.subobjects[1].attribute.frequency, 1);                                   \
		CHECK_EQ(result_b.subobjects[1].attribute.ratio, 2);                                       \
	}

#define TEST_X_Y_Z_SETTING(section, name, attribute)                                               \
	TEST_CASE("libparsers - animated object - " section " - " name) {                              \
		auto string_a = "[" section "]\n" name " = 1, 2, 3"s;                                      \
		auto result_a = parsers::animated_object::parse(string_a);                                 \
		auto string_b = "[objectA]\n[" section "]\n" name " = 1, 2, 3"s;                           \
		auto result_b = parsers::animated_object::parse(string_b);                                 \
                                                                                                   \
		REQUIRE_EQ(result_a.includes.size(), 0);                                                   \
		REQUIRE_EQ(result_a.subobjects.size(), 1);                                                 \
		REQUIRE_EQ(result_a.dependencies.textures.size(), 0);                                      \
		CHECK_EQ(result_a.subobjects[0].attribute, glm::vec3{1, 2, 3});                            \
                                                                                                   \
		REQUIRE_EQ(result_b.includes.size(), 0);                                                   \
		REQUIRE_EQ(result_b.subobjects.size(), 2);                                                 \
		REQUIRE_EQ(result_b.dependencies.textures.size(), 0);                                      \
		CHECK_EQ(result_b.subobjects[1].attribute, glm::vec3{1, 2, 3});                            \
	}

TEST_X_Y_Z_SETTING("object", "position", position);
TEST_X_Y_Z_SETTING("object", "translatexdirection", translate_x_direction);
TEST_X_Y_Z_SETTING("object", "translateydirection", translate_y_direction);
TEST_X_Y_Z_SETTING("object", "translatezdirection", translate_z_direction);
TEST_X_Y_Z_SETTING("object", "rotatexdirection", rotate_x_direction);
TEST_X_Y_Z_SETTING("object", "rotateydirection", rotate_y_direction);
TEST_X_Y_Z_SETTING("object", "rotatezdirection", rotate_z_direction);
TEST_X_Y_DAMPING("object", "rotatexdamping", rotate_x_damping);
TEST_X_Y_DAMPING("object", "rotateydamping", rotate_y_damping);
TEST_X_Y_DAMPING("object", "rotatezdamping", rotate_z_damping);
TEST_X_Y_SETTING("object", "textureshiftxdirection", texture_shift_x_direction);
TEST_X_Y_SETTING("object", "textureshiftydirection", texture_shift_y_direction);
