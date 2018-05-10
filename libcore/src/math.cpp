#include "core/math.hpp"
#include <cassert>
#include <cmath>
#include <glm/geometric.hpp>

#define _USE_MATH_DEFINES
#include <math.h> // NOLINT Also include overloads of std::*

float openbve2::math::radius_from_distances(float deltax, float deltay) {
	// https://math.stackexchange.com/questions/1088902/what-is-the-radius-of-a-circle-given-two-points-and-the-center-of-the-circle-is#comment2218658_1088926
	// gives the formula for this
	// X = y1 in the formula
	// Z = x1 in the formula
	// y0 and x0 are both zero, so they can be ommitted.

	auto& x1 = deltax;
	auto& y1 = deltay;

	auto angle = std::atan2(y1, x1);

	auto num = std::sqrt(y1 * y1 + x1 * x1) * std::sin(float(M_PI_2) - angle);
	auto denom = std::sin(float(M_PI) - 2 * (float(M_PI_2) - angle));

	auto radius = num / denom;

	return radius;
}

openbve2::math::evaulate_curve_t openbve2::math::evaluate_curve(glm::vec3 input_position,
                                                                glm::vec3 input_direction,
                                                                float distance,
                                                                float radius) {
	if (distance == 0) {
		return {input_position, input_direction};
	}

	auto original_input_direction = input_direction;
	assert(input_direction != glm::vec3(0));
	input_direction = normalize(input_direction);

	if (radius == 0) {
		input_position += input_direction * distance;
		return {input_position, original_input_direction};
	}

	float vertical_movement = input_direction.y * distance;

	// non-vertical movement we are allowed
	float horizontal_movement =
	    std::sqrt(distance * distance - vertical_movement * vertical_movement);

	bool flipped_radius = radius < 0;
	radius = std::abs(radius);

	// convert from game direction coordinates to 2d cartisian plane
	auto xy = glm::vec2(input_direction.z, -input_direction.x);

	// this algorithm works by pretending all curves are to the right. THe
	// problem lies if we keep the input vector the same, we will get the wrong
	// part of the curve. Inverting it over the Y axis brings it to the right
	// place
	if (flipped_radius) {
		xy.y *= -1;
	}

	// get angle of input
	float atan = std::atan2(xy.y, xy.x);
	if (xy.y < 0) {
		atan += static_cast<float>(M_PI * 2);
	}

	// flip the result of arctan so the angles are going clockwise, not counter
	float input_angle = static_cast<float>(M_PI * 2) - atan;

	// compute fraction of circle traveled
	float circumference = 2 * static_cast<float>(M_PI) * radius;
	float frac_traveled = horizontal_movement / circumference;

	// get angle traveled in radians
	float travel_angle = frac_traveled * static_cast<float>(M_PI * 2);

	// make sure the ending angle includes the starting angle
	travel_angle += input_angle;

	// get coordinates on a unit circle with its center at (0,0)
	// right part of the circle is 90deg
	// bottom is 180, etc
	// hence why we flipped x and y, as we mirrored it over y=x

	// the track is comming in at (0,0)
	// right on the coordinate plane is moving forward (+z in game world)
	// down on the coordinate plane is turning right (+x in the game world)
	float travel_x = std::sin(travel_angle);
	float travel_y = std::cos(travel_angle);

	// Get tangent at ending point
	auto radius_line = glm::vec2(travel_x, travel_y) - glm::vec2(0, 0);
	glm::vec2 tangent_line;
	if (flipped_radius) {
		radius_line.y *= -1;
		// Counterclockwise tangent vector
		tangent_line = normalize(glm::vec2(-radius_line.y, radius_line.x));
	}
	else {
		// Clockwise tangent vector
		tangent_line = normalize(glm::vec2(radius_line.y, -radius_line.x));
	}

	// add the offset to make the vector start at the input angle
	travel_x -= std::sin(input_angle);
	travel_y -= std::cos(input_angle);

	// scale this unit circle to the proper radius
	travel_x *= radius;
	travel_y *= radius;

	// flip x to make a left turn
	if (flipped_radius) {
		travel_y *= -1;
	}

	// convert to game space
	glm::vec3 gamespace_offset(-travel_y, vertical_movement, travel_x);
	tangent_line *= horizontal_movement;
	glm::vec3 tangent_3d(-tangent_line.y, vertical_movement, tangent_line.x);
	tangent_3d = normalize(tangent_3d);
	tangent_3d *= length(original_input_direction);

	// add to the position
	input_position += gamespace_offset;

	return evaulate_curve_t{input_position, tangent_3d};
}

glm::vec3 openbve2::math::position_from_offsets(glm::vec3 input_position,
                                                glm::vec3 input_tangent,
                                                float x_offset,
                                                float y_offset) {
	assert(input_tangent != glm::vec3(0));

	auto x_z = normalize(glm::vec2(input_tangent.x, input_tangent.z));

	// rotate 270 degrees
	glm::vec2 normal(x_z.y, -x_z.x);

	// apply offset
	normal *= x_offset;

	return input_position + glm::vec3(normal.x, y_offset, normal.y);
}
