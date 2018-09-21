#include "function_scripts.hpp"
#include "utils.hpp"
#include <algorithm>
#include <iterator>
#include <map>
#include <set>
#include <sstream>

using namespace std::string_literals;

namespace bve::parsers::function_scripts {
	namespace {
		const std::map<std::string, instructions::Variable> naked_variables = {
		    {"value"s, instructions::Variable::value},                             //
		    {"delta"s, instructions::Variable::delta},                             //
		    {"currentstate"s, instructions::Variable::current_state},              //
		    {"time"s, instructions::Variable::time},                               //
		    {"cameradistance"s, instructions::Variable::camera_distance},          //
		    {"cameramode"s, instructions::Variable::camera_mode},                  //
		    {"cars"s, instructions::Variable::cars},                               //
		    {"speed"s, instructions::Variable::speed},                             //
		    {"speedometer"s, instructions::Variable::speedometer},                 //
		    {"acceleration"s, instructions::Variable::acceleration},               //
		    {"accelerationmotor"s, instructions::Variable::acceleration_motor},    //
		    {"distance"s, instructions::Variable::distance},                       //
		    {"trackdistance"s, instructions::Variable::track_distance},            //
		    {"mainreservoir"s, instructions::Variable::main_reservoir},            //
		    {"emergencyreservoir"s, instructions::Variable::emergency_reservoir},  //
		    {"brakepipe"s, instructions::Variable::brake_pipe},                    //
		    {"brakecylinder"s, instructions::Variable::brake_cylinder},            //
		    {"straightairpipe"s, instructions::Variable::straight_air_pipe},       //
		    {"doors"s, instructions::Variable::doors},                             //
		    {"leftdoors"s, instructions::Variable::left_doors},                    //
		    {"rightdoors"s, instructions::Variable::right_doors},                  //
		    {"leftdoorstarget"s, instructions::Variable::left_doors_target},       //
		    {"rightdoorstarget"s, instructions::Variable::right_doors_target},     //
		    {"leftdoorsbutton"s, instructions::Variable::left_doors_button},       //
		    {"rightdoorsbutton"s, instructions::Variable::right_doors_button},     //
		    {"reversernotch"s, instructions::Variable::reverser_notch},            //
		    {"powernotch"s, instructions::Variable::power_notch},                  //
		    {"powernotches"s, instructions::Variable::power_notches},              //
		    {"brakenotch"s, instructions::Variable::brake_notch},                  //
		    {"brakenotches"s, instructions::Variable::brake_notches},              //
		    {"brakenotchlinear"s, instructions::Variable::brake_notch_linear},     //
		    {"brakenotcheslinear"s, instructions::Variable::brake_notches_linear}, //
		    {"emergencybrake"s, instructions::Variable::emergency_brake},          //
		    {"hasairbrake"s, instructions::Variable::has_air_brake},               //
		    {"holdbrake"s, instructions::Variable::hold_brake},                    //
		    {"hasholdbrake"s, instructions::Variable::has_hold_brake},             //
		    {"constspeed"s, instructions::Variable::const_speed},                  //
		    {"hasconstspeed"s, instructions::Variable::has_const_speed},           //
		    {"hasplugin"s, instructions::Variable::has_plugin},                    //
		    {"odometer"s, instructions::Variable::odometer},                       //
		    {"klaxon"s, instructions::Variable::klaxon},                           //
		    {"primaryklaxon"s, instructions::Variable::primary_klaxon},            //
		    {"secondaryklaxon"s, instructions::Variable::secondary_klaxon},        //
		    {"musicklaxon"s, instructions::Variable::music_klaxon},                //
		    {"section"s, instructions::Variable::section},                         //
		};

		const std::map<std::string, instructions::IndexedVariable> indexable_variables = {
		    {"speed"s, instructions::IndexedVariable::speed},                                  //
		    {"speedometer"s, instructions::IndexedVariable::speedometer},                      //
		    {"acceleration"s, instructions::IndexedVariable::acceleration},                    //
		    {"accelerationmotor"s, instructions::IndexedVariable::acceleration_motor},         //
		    {"distance"s, instructions::IndexedVariable::distance},                            //
		    {"trackdistance"s, instructions::IndexedVariable::track_distance},                 //
		    {"mainreservoir"s, instructions::IndexedVariable::main_reservoir},                 //
		    {"emergencyreservoir"s, instructions::IndexedVariable::emergency_reservoir},       //
		    {"brakepipe"s, instructions::IndexedVariable::brake_pipe},                         //
		    {"brakecylinder"s, instructions::IndexedVariable::brake_cylinder},                 //
		    {"straightairpipe"s, instructions::IndexedVariable::straight_air_pipe},            //
		    {"doors"s, instructions::IndexedVariable::doors},                                  //
		    {"leftdoors"s, instructions::IndexedVariable::left_doors},                         //
		    {"rightdoors"s, instructions::IndexedVariable::right_doors},                       //
		    {"leftdoorstarget"s, instructions::IndexedVariable::left_doors_target},            //
		    {"rightdoorstarget"s, instructions::IndexedVariable::right_doors_target},          //
		    {"pluginstate"s, instructions::IndexedVariable::plugin_state},                     //
		    {"frontaxlecurveradius"s, instructions::IndexedVariable::front_axle_curve_radius}, //
		    {"rearaxlecurveradius"s, instructions::IndexedVariable::rear_axle_curve_radius},   //
		    {"curvecant"s, instructions::IndexedVariable::curve_cant},                         //
		    {"odometer"s, instructions::IndexedVariable::odometer},                            //
		};

		const std::map<std::string, Instruction> unary_functions = {
		    {"minus"s, instructions::OPUnaryMinus{}},        //
		    {"not"s, instructions::OPUnaryNot{}},            //
		    {"reciprocal"s, instructions::FuncReciprocal{}}, //
		    {"abs"s, instructions::FuncAbs{}},               //
		    {"sign"s, instructions::FuncSign{}},             //
		    {"floor"s, instructions::FuncFloor{}},           //
		    {"ceiling"s, instructions::FuncCeiling{}},       //
		    {"round"s, instructions::FuncRound{}},           //
		    {"exp"s, instructions::FuncExp{}},               //
		    {"log"s, instructions::FuncLog{}},               //
		    {"sqrt"s, instructions::FuncSqrt{}},             //
		    {"sin"s, instructions::FuncSin{}},               //
		    {"cos"s, instructions::FuncCos{}},               //
		    {"tan"s, instructions::FuncTan{}},               //
		    {"arctan"s, instructions::FuncArctan{}},         //
		};

		const std::map<std::string, Instruction> binary_functions = {
		    {"subtract"s, instructions::OPSubtract{}},         //
		    {"divide"s, instructions::OPDivide{}},             //
		    {"equal"s, instructions::OPEqual{}},               //
		    {"unequal"s, instructions::OPUnequal{}},           //
		    {"less"s, instructions::OPLess{}},                 //
		    {"greater"s, instructions::OPGreater{}},           //
		    {"lessequal"s, instructions::OPLessEqual{}},       //
		    {"greaterequal"s, instructions::OPGreaterEqual{}}, //
		    {"and"s, instructions::OPAnd{}},                   //
		    {"or"s, instructions::OPOr{}},                     //
		    {"xor"s, instructions::OPXor{}},                   //
		    {"quotient"s, instructions::FuncQuotient{}},       //
		    {"mod"s, instructions::FuncMod{}},                 //
		    {"random"s, instructions::FuncRandom{}},           //
		    {"randomint"s, instructions::FuncRandomInt{}},     //
		};

		const std::set<std::string> variadic_functions{
		    {"plus"s},  //
		    {"times"s}, //
		    {"power"s}, //
		    {"min"s},   //
		    {"max"s},   //
		};
	} // namespace

	struct InstructionBuilderHelper {
		InstructionList list;

		void callNextNode(const TreeNode& next) {
			apply_visitor(*this, next);
		}

		template <class R, class T>
		void generateBinaryFunc(const T& node) {
			callNextNode(node.left);
			callNextNode(node.right);
			list.instructions.emplace_back<R>(R{});
		}

		template <class R, class T>
		void generateUnaryFunc(const T& node) {
			callNextNode(node.child);
			list.instructions.emplace_back<R>(R{});
		}

		void operator()(const tree_types::BinaryAnd& node) {
			generateBinaryFunc<instructions::OPAnd>(node);
		}
		void operator()(const tree_types::BinaryXor& node) {
			generateBinaryFunc<instructions::OPXor>(node);
		}
		void operator()(const tree_types::BinaryOr& node) {
			generateBinaryFunc<instructions::OPOr>(node);
		}
		void operator()(const tree_types::BinaryEq& node) {
			generateBinaryFunc<instructions::OPEqual>(node);
		}
		void operator()(const tree_types::BinaryNotEq& node) {
			generateBinaryFunc<instructions::OPUnequal>(node);
		}
		void operator()(const tree_types::BinaryLess& node) {
			generateBinaryFunc<instructions::OPLess>(node);
		}
		void operator()(const tree_types::BinaryGreater& node) {
			generateBinaryFunc<instructions::OPGreater>(node);
		}
		void operator()(const tree_types::BinaryLessEq& node) {
			generateBinaryFunc<instructions::OPLessEqual>(node);
		}
		void operator()(const tree_types::BinaryGreaterEq& node) {
			generateBinaryFunc<instructions::OPGreaterEqual>(node);
		}
		void operator()(const tree_types::BinaryAdd& node) {
			generateBinaryFunc<instructions::OPAdd>(node);
		}
		void operator()(const tree_types::BinarySubtract& node) {
			generateBinaryFunc<instructions::OPSubtract>(node);
		}
		void operator()(const tree_types::BinaryMultiply& node) {
			generateBinaryFunc<instructions::OPMultiply>(node);
		}
		void operator()(const tree_types::BinaryDivide& node) {
			generateBinaryFunc<instructions::OPDivide>(node);
		}
		void operator()(const tree_types::UnaryNot& node) {
			generateUnaryFunc<instructions::OPUnaryNot>(node);
		}
		void operator()(const tree_types::UnaryMinus& node) {
			generateUnaryFunc<instructions::OPUnaryMinus>(node);
		}
		void operator()(const tree_types::Integer& node) {
			list.instructions.emplace_back<instructions::StackPush>({float(node.num)});
		}
		void operator()(const tree_types::Floating& node) {
			list.instructions.emplace_back<instructions::StackPush>({float(node.num)});
		}
		void operator()(const tree_types::None& node) {
			(void) node;
			list.instructions.emplace_back<instructions::StackPush>({0});
		}

		void operator()(const tree_types::Identifier& node) {
			auto const lower_name = util::lower_copy(node.val);

			auto const iter = naked_variables.find(lower_name);
			if (iter != naked_variables.end()) {
				list.instructions.emplace_back<instructions::OPVariableLookup>({iter->second});
				list.used_variables.insert(iter->second);
			}
			else {
				std::ostringstream error;
				error << "Variable \"" << node.val << "\" not found";
				list.errors.emplace_back<errors::Error>({0, error.str()});
				list.instructions.emplace_back<instructions::StackPush>({0});
			}
		}
		// ReSharper disable once CyclomaticComplexity
		void operator()(const tree_types::FunctionCall& node) {
			auto const arg_count = node.args.size();
			auto const lower_name = util::lower_copy(node.name.val);

			// unary
			auto const unary_iter = unary_functions.find(lower_name);
			if (unary_iter != unary_functions.end()) {
				if (arg_count < 1) {
					list.instructions.emplace_back<instructions::StackPush>({0});
				}
				else {
					callNextNode(node.args[0]);
					list.instructions.emplace_back(unary_iter->second);
				}
				if (arg_count != 1) {
					std::ostringstream err;
					err << "Function \"" << node.name.val << "\" requires only 1 argument.";
					add_error(list.errors, 0, err);
				}
				return;
			}
			// unary indexing
			auto const index_iter = indexable_variables.find(lower_name);
			if (index_iter != indexable_variables.end()) {
				if (arg_count >= 1) {
					callNextNode(node.args[0]);
					list.instructions.emplace_back<instructions::OPVariableIndexed>(
					    {index_iter->second});
					list.used_indexed_variables.insert(index_iter->second);
				}
				else {
					list.instructions.emplace_back<instructions::StackPush>({0});
				}
				if (arg_count != 1) {
					std::ostringstream error;
					error << "Variable \"" << node.name.val
					      << "\" must be indexed with one argument.";
					list.errors.emplace_back<errors::Error>({0, error.str()});
				}
				return;
			}

			// binary
			auto const binary_iter = binary_functions.find(lower_name);
			if (binary_iter != binary_functions.end()) {
				if (arg_count >= 2) {
					callNextNode(node.args[0]);
					callNextNode(node.args[1]);
					list.instructions.emplace_back(binary_iter->second);
				}
				else if (arg_count == 1) {
					callNextNode(node.args[0]);
					list.instructions.emplace_back<instructions::StackPush>({0});
					list.instructions.emplace_back(binary_iter->second);
				}
				else {
					list.instructions.emplace_back<instructions::StackPush>({0});
					list.instructions.emplace_back<instructions::StackPush>({0});
					list.instructions.emplace_back(binary_iter->second);
				}
				if (arg_count != 2) {
					std::ostringstream error;
					error << "Function \"" << node.name.val << "\" requires 2 arguments.";
					add_error(list.errors, 0, error);
				}
				return;
			}

			// ternary
			if (lower_name == "if"s) {
				if (arg_count >= 3) {
					callNextNode(node.args[0]);
					callNextNode(node.args[1]);
					callNextNode(node.args[2]);
					list.instructions.emplace_back<instructions::FuncIf>({});
				}
				else if (arg_count == 2) {
					callNextNode(node.args[0]);
					callNextNode(node.args[1]);
					list.instructions.emplace_back<instructions::StackPush>({0});
					list.instructions.emplace_back<instructions::FuncIf>({});
				}
				else if (arg_count == 1) {
					callNextNode(node.args[0]);
					list.instructions.emplace_back<instructions::StackPush>({0});
					list.instructions.emplace_back<instructions::StackPush>({0});
					list.instructions.emplace_back<instructions::FuncIf>({});
				}
				else if (arg_count == 0) {
					list.instructions.emplace_back<instructions::StackPush>({0});
					list.instructions.emplace_back<instructions::StackPush>({0});
					list.instructions.emplace_back<instructions::StackPush>({0});
					list.instructions.emplace_back<instructions::FuncIf>({});
				}
				if (arg_count != 3) {
					std::ostringstream error;
					error << "Function \"" << node.name.val << "\" requires 3 arguments.";
					add_error(list.errors, 0, error);
				}
				return;
			}

			// variadic
			auto const variadic_iter = variadic_functions.find(lower_name);
			if (variadic_iter != variadic_functions.end()) {
				for (auto& arg : node.args) {
					callNextNode(arg);
				}

				auto const creator = [this](auto&& s, std::size_t const count) {
					s.count = count;
					list.instructions.emplace_back(s);
				};

				if (lower_name == "plus"s) {
					creator(instructions::OPAdd{}, arg_count);
				}
				else if (lower_name == "times"s) {
					creator(instructions::OPMultiply{}, arg_count);
				}
				else if (lower_name == "power"s) {
					creator(instructions::FuncPower{}, arg_count);
				}
				else if (lower_name == "min"s) {
					creator(instructions::FuncMin{}, arg_count);
				}
				else if (lower_name == "max"s) {
					creator(instructions::FuncMax{}, arg_count);
				}
				return;
			}

			std::ostringstream error;
			error << "Function \"" << node.name.val << "\" not recognized.";
			add_error(list.errors, 0, error);

			list.instructions.emplace_back<instructions::StackPush>({0});
		}
	};

	InstructionList build_instructions(const TreeNode& head_node, const errors::Errors& errors) {
		InstructionBuilderHelper ibh;
		std::copy(errors.begin(), errors.end(), std::back_inserter(ibh.list.errors));
		apply_visitor(ibh, head_node);
		return ibh.list;
	}
} // namespace bve::parsers::function_scripts
