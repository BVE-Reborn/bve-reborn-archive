#pragma once

#define BOOST_MPL_CFG_NO_PREPROCESSED_HEADERS
#define BOOST_MPL_LIMIT_LIST_SIZE 40

#include <boost/variant/variant.hpp>
#include <cinttypes>
#include <cstdlib>
#include <string>
#include <vector>

namespace parsers {
namespace function_scripts {
	namespace instructions {
		enum class variable {
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
			CurveCant
		};

		struct has_count {
			std::size_t count;
		};

		struct stack_push {
			float value;
		};

		struct op_plus : public has_count {};
		struct op_minus {};
		struct op_unary_minus {};
		struct op_times : public has_count {};
		struct op_divide {};

		struct op_equal {};
		struct op_unequal {};
		struct op_less {};
		struct op_greater {};
		struct op_less_equal {};
		struct op_greater_equal {};

		struct op_not {};
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
			std::size_t index;
		};

		struct func_reciprocal {};
		struct func_power : public has_count {};

		struct func_quotient {};
		struct func_mod {};
		struct func_min : public has_count {};
		struct func_max : public has_count {};
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

	using instruction = boost::variant<
	    instructions::stack_push, instructions::op_plus, instructions::op_minus, instructions::op_unary_minus,
	    instructions::op_times, instructions::op_divide, instructions::op_equal, instructions::op_unequal,
	    instructions::op_less, instructions::op_greater, instructions::op_less_equal, instructions::op_greater_equal,
	    instructions::op_not, instructions::op_and, instructions::op_or, instructions::op_xor,
	    instructions::op_variable_lookup, instructions::op_variable_indexed, instructions::func_reciprocal,
	    instructions::func_power, instructions::func_quotient, instructions::func_mod, instructions::func_min,
	    instructions::func_max, instructions::func_abs, instructions::func_sign, instructions::func_floor,
	    instructions::func_ceiling, instructions::func_round, instructions::func_random, instructions::func_randomInt,
	    instructions::func_exp, instructions::func_log, instructions::func_sqrt, instructions::func_sin,
	    instructions::func_cos, instructions::func_tan, instructions::func_arctan, instructions::func_if>;

	struct instruction_list {
		std::vector<instructions::variable> used_variables;
		std::vector<instructions::op_variable_indexed> used_indexed_variables;
		std::vector<instruction> instruction;
	};
} // namespace function_scripts
} // namespace parsers