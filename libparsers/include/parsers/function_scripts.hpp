#pragma once

#include "parsers/errors.hpp"
#include <cinttypes>
#include <cstdlib>
#include <iosfwd>
#include <mapbox/variant.hpp>
#include <set>
#include <string>
#include <vector>

namespace parsers {
namespace function_scripts {
	namespace instructions {
		enum class variable : uint8_t {
			none = 0,
			value,
			delta,
			currentState,
			time,
			cameraDistance,
			cameraMode,
			cars,
			speed,
			speedometer,
			acceleration,
			accelerationMotor,
			distance,
			trackDistance,
			mainReservoir,
			emergencyReservoir,
			brakePipe,
			brakeCylinder,
			straightAirPipe,
			doors,
			leftDoors,
			rightDoors,
			leftDoorsTarget,
			rightDoorsTarget,
			leftDoorsButton,
			rightDoorsButton,
			reverserNotch,
			powerNotch,
			powerNotches,
			brakeNotch,
			brakeNotches,
			brakeNotchLinear,
			brakeNotchesLinear,
			emergencyBrake,
			hasAirBrake,
			holdBrake,
			hasHoldBrake,
			constSpeed,
			hasConstSpeed,
			hasPlugin,
			Odometer,
			Klaxon,
			PrimaryKlaxon,
			SecondaryKlaxon,
			MusicKlaxon,
			section
		};

		enum class indexed_variable : uint8_t {
			none = 0,
			speed,
			speedometer,
			acceleration,
			accelerationMotor,
			distance,
			trackDistance,
			mainReservoir,
			emergencyReservoir,
			brakePipe,
			brakeCylinder,
			straightAirPipe,
			doors,
			leftDoors,
			rightDoors,
			leftDoorsTarget,
			rightDoorsTarget,
			pluginState,
			FrontAxleCurveRadius,
			RearAxleCurveRadius,
			CurveCant,
			Odometer
		};

		struct stack_push {
			float value;
		};

		struct op_add {
			std::size_t count = 2;
		};
		struct op_subtract {};
		struct op_unary_minus {};
		struct op_multiply {
			std::size_t count = 2;
		};
		struct op_divide {};

		struct op_equal {};
		struct op_unequal {};
		struct op_less {};
		struct op_greater {};
		struct op_less_equal {};
		struct op_greater_equal {};

		struct op_unary_not {};
		struct op_and {};
		struct op_or {};
		struct op_xor {};

		// struct op_function_call {
		// 	std::string function;
		// 	std::size_t args;
		// };

		struct op_variable_lookup {
			variable name;
		};

		struct op_variable_indexed {
			indexed_variable name;
		};

		struct func_reciprocal {};
		struct func_power {
			std::size_t count = 2;
		};

		struct func_quotient {};
		struct func_mod {};
		struct func_min {
			std::size_t count = 2;
		};
		struct func_max {
			std::size_t count = 2;
		};
		struct func_abs {};
		struct func_sign {};
		struct func_floor {};
		struct func_ceiling {};
		struct func_round {};
		struct func_random {};
		struct func_randomInt {};

		struct func_exp {};
		struct func_log {};
		struct func_sqrt {};
		struct func_sin {};
		struct func_cos {};
		struct func_tan {};
		struct func_arctan {};

		struct func_if {};
	} // namespace instructions

	using instruction = mapbox::util::variant<instructions::stack_push,
	                                          instructions::op_add,
	                                          instructions::op_subtract,
	                                          instructions::op_unary_minus,
	                                          instructions::op_multiply,
	                                          instructions::op_divide,
	                                          instructions::op_equal,
	                                          instructions::op_unequal,
	                                          instructions::op_less,
	                                          instructions::op_greater,
	                                          instructions::op_less_equal,
	                                          instructions::op_greater_equal,
	                                          instructions::op_unary_not,
	                                          instructions::op_and,
	                                          instructions::op_or,
	                                          instructions::op_xor,
	                                          instructions::op_variable_lookup,
	                                          instructions::op_variable_indexed,
	                                          instructions::func_reciprocal,
	                                          instructions::func_power,
	                                          instructions::func_quotient,
	                                          instructions::func_mod,
	                                          instructions::func_min,
	                                          instructions::func_max,
	                                          instructions::func_abs,
	                                          instructions::func_sign,
	                                          instructions::func_floor,
	                                          instructions::func_ceiling,
	                                          instructions::func_round,
	                                          instructions::func_random,
	                                          instructions::func_randomInt,
	                                          instructions::func_exp,
	                                          instructions::func_log,
	                                          instructions::func_sqrt,
	                                          instructions::func_sin,
	                                          instructions::func_cos,
	                                          instructions::func_tan,
	                                          instructions::func_arctan,
	                                          instructions::func_if>;

	struct instruction_list {
		std::set<instructions::variable> used_variables;
		std::set<instructions::indexed_variable> used_indexed_variables;
		std::vector<instruction> instructions;
		errors::errors_t errors;
	};

	instruction_list parse(const std::string& text);

	std::ostream& operator<<(std::ostream& os, const instruction_list& list);
} // namespace function_scripts
} // namespace parsers
