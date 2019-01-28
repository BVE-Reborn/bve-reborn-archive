#pragma once

#include "parsers/errors.hpp"
#include <iosfwd>
#include <mapbox/variant.hpp>
#include <set>
#include <string>
#include <vector>

namespace bve::parsers::function_scripts {
	namespace instructions {
		enum class Variable : uint8_t {
			none = 0,
			value,
			delta,
			current_state,
			time,
			camera_distance,
			camera_mode,
			cars,
			speed,
			speedometer,
			acceleration,
			acceleration_motor,
			distance,
			track_distance,
			main_reservoir,
			emergency_reservoir,
			brake_pipe,
			brake_cylinder,
			straight_air_pipe,
			doors,
			left_doors,
			right_doors,
			left_doors_target,
			right_doors_target,
			left_doors_button,
			right_doors_button,
			reverser_notch,
			power_notch,
			power_notches,
			brake_notch,
			brake_notches,
			brake_notch_linear,
			brake_notches_linear,
			emergency_brake,
			has_air_brake,
			hold_brake,
			has_hold_brake,
			const_speed,
			has_const_speed,
			has_plugin,
			odometer,
			klaxon,
			primary_klaxon,
			secondary_klaxon,
			music_klaxon,
			section
		};

		enum class IndexedVariable : uint8_t {
			none = 0,
			speed,
			speedometer,
			acceleration,
			acceleration_motor,
			distance,
			track_distance,
			main_reservoir,
			emergency_reservoir,
			brake_pipe,
			brake_cylinder,
			straight_air_pipe,
			doors,
			left_doors,
			right_doors,
			left_doors_target,
			right_doors_target,
			plugin_state,
			front_axle_curve_radius,
			rear_axle_curve_radius,
			curve_cant,
			odometer
		};

		struct StackPush {
			float value;
		};

		struct OPAdd {
			std::size_t count = 2;
		};
		struct OPSubtract {};
		struct OPUnaryMinus {};
		struct OPMultiply {
			std::size_t count = 2;
		};
		struct OPDivide {};

		struct OPEqual {};
		struct OPUnequal {};
		struct OPLess {};
		struct OPGreater {};
		struct OPLessEqual {};
		struct OPGreaterEqual {};

		struct OPUnaryNot {};
		struct OPAnd {};
		struct OPOr {};
		struct OPXor {};

		// struct op_function_call {
		// 	std::string function;
		// 	std::size_t args;
		// };

		struct OPVariableLookup {
			Variable name;
		};

		struct OPVariableIndexed {
			IndexedVariable name;
		};

		struct FuncReciprocal {};
		struct FuncPower {
			std::size_t count = 2;
		};

		struct FuncQuotient {};
		struct FuncMod {};
		struct FuncMin {
			std::size_t count = 2;
		};
		struct FuncMax {
			std::size_t count = 2;
		};
		struct FuncAbs {};
		struct FuncSign {};
		struct FuncFloor {};
		struct FuncCeiling {};
		struct FuncRound {};
		struct FuncRandom {};
		struct FuncRandomInt {};

		struct FuncExp {};
		struct FuncLog {};
		struct FuncSqrt {};
		struct FuncSin {};
		struct FuncCos {};
		struct FuncTan {};
		struct FuncArctan {};

		struct FuncIf {};
	} // namespace instructions

	using Instruction = mapbox::util::variant<instructions::StackPush,
	                                          instructions::OPAdd,
	                                          instructions::OPSubtract,
	                                          instructions::OPUnaryMinus,
	                                          instructions::OPMultiply,
	                                          instructions::OPDivide,
	                                          instructions::OPEqual,
	                                          instructions::OPUnequal,
	                                          instructions::OPLess,
	                                          instructions::OPGreater,
	                                          instructions::OPLessEqual,
	                                          instructions::OPGreaterEqual,
	                                          instructions::OPUnaryNot,
	                                          instructions::OPAnd,
	                                          instructions::OPOr,
	                                          instructions::OPXor,
	                                          instructions::OPVariableLookup,
	                                          instructions::OPVariableIndexed,
	                                          instructions::FuncReciprocal,
	                                          instructions::FuncPower,
	                                          instructions::FuncQuotient,
	                                          instructions::FuncMod,
	                                          instructions::FuncMin,
	                                          instructions::FuncMax,
	                                          instructions::FuncAbs,
	                                          instructions::FuncSign,
	                                          instructions::FuncFloor,
	                                          instructions::FuncCeiling,
	                                          instructions::FuncRound,
	                                          instructions::FuncRandom,
	                                          instructions::FuncRandomInt,
	                                          instructions::FuncExp,
	                                          instructions::FuncLog,
	                                          instructions::FuncSqrt,
	                                          instructions::FuncSin,
	                                          instructions::FuncCos,
	                                          instructions::FuncTan,
	                                          instructions::FuncArctan,
	                                          instructions::FuncIf>;

	struct InstructionList {
		std::set<instructions::Variable> used_variables;
		std::set<instructions::IndexedVariable> used_indexed_variables;
		std::vector<Instruction> instructions;
		errors::Errors errors;
	};

	InstructionList parse(const std::string& text);

	std::ostream& operator<<(std::ostream& os, const InstructionList& list);
} // namespace bve::parsers::function_scripts
