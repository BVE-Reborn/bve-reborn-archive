#include "function_scripts.hpp"
#include "utils.hpp"
#include <algorithm>
#include <iterator>
#include <map>
#include <set>
#include <sstream>

using namespace std::string_literals;

namespace parsers {
namespace function_scripts {
	namespace {
		static const std::map<std::string, instructions::variable> naked_variables = {
		    {"value"s, instructions::variable::value},                           //
		    {"delta"s, instructions::variable::delta},                           //
		    {"currentstate"s, instructions::variable::currentState},             //
		    {"time"s, instructions::variable::time},                             //
		    {"cameradistance"s, instructions::variable::cameraDistance},         //
		    {"cameramode"s, instructions::variable::cameraMode},                 //
		    {"cars"s, instructions::variable::cars},                             //
		    {"speed"s, instructions::variable::speed},                           //
		    {"speedometer"s, instructions::variable::speedometer},               //
		    {"acceleration"s, instructions::variable::acceleration},             //
		    {"accelerationmotor"s, instructions::variable::accelerationMotor},   //
		    {"distance"s, instructions::variable::distance},                     //
		    {"trackdistance"s, instructions::variable::trackDistance},           //
		    {"mainreservoir"s, instructions::variable::mainReservoir},           //
		    {"emergencyreservoir"s, instructions::variable::emergencyReservoir}, //
		    {"brakepipe"s, instructions::variable::brakePipe},                   //
		    {"brakecylinder"s, instructions::variable::brakeCylinder},           //
		    {"straightairpipe"s, instructions::variable::straightAirPipe},       //
		    {"doors"s, instructions::variable::doors},                           //
		    {"leftdoors"s, instructions::variable::leftDoors},                   //
		    {"rightdoors"s, instructions::variable::rightDoors},                 //
		    {"leftdoorstarget"s, instructions::variable::leftDoorsTarget},       //
		    {"rightdoorstarget"s, instructions::variable::rightDoorsTarget},     //
		    {"leftdoorsbutton"s, instructions::variable::leftDoorsButton},       //
		    {"rightdoorsbutton"s, instructions::variable::rightDoorsButton},     //
		    {"reversernotch"s, instructions::variable::reverserNotch},           //
		    {"powernotch"s, instructions::variable::powerNotch},                 //
		    {"powernotches"s, instructions::variable::powerNotches},             //
		    {"brakenotch"s, instructions::variable::brakeNotch},                 //
		    {"brakenotches"s, instructions::variable::brakeNotches},             //
		    {"brakenotchlinear"s, instructions::variable::brakeNotchLinear},     //
		    {"brakenotcheslinear"s, instructions::variable::brakeNotchesLinear}, //
		    {"emergencybrake"s, instructions::variable::emergencyBrake},         //
		    {"hasairbrake"s, instructions::variable::hasAirBrake},               //
		    {"holdbrake"s, instructions::variable::holdBrake},                   //
		    {"hasholdbrake"s, instructions::variable::hasHoldBrake},             //
		    {"constspeed"s, instructions::variable::constSpeed},                 //
		    {"hasconstspeed"s, instructions::variable::hasConstSpeed},           //
		    {"hasplugin"s, instructions::variable::hasPlugin},                   //
		    {"odometer"s, instructions::variable::Odometer},                     //
		    {"klaxon"s, instructions::variable::Klaxon},                         //
		    {"primaryklaxon"s, instructions::variable::PrimaryKlaxon},           //
		    {"musicklaxon"s, instructions::variable::MusicKlaxon},               //
		    {"section"s, instructions::variable::section},                       //
		};

		static const std::map<std::string, instructions::indexed_variable> indexable_variables = {
		    {"speed"s, instructions::indexed_variable::speed},                               //
		    {"speedometer"s, instructions::indexed_variable::speedometer},                   //
		    {"acceleration"s, instructions::indexed_variable::acceleration},                 //
		    {"accelerationmotor"s, instructions::indexed_variable::accelerationMotor},       //
		    {"distance"s, instructions::indexed_variable::distance},                         //
		    {"trackdistance"s, instructions::indexed_variable::trackDistance},               //
		    {"mainreservoir"s, instructions::indexed_variable::mainReservoir},               //
		    {"emergencyreservoir"s, instructions::indexed_variable::emergencyReservoir},     //
		    {"brakepipe"s, instructions::indexed_variable::brakePipe},                       //
		    {"brakecylinder"s, instructions::indexed_variable::brakeCylinder},               //
		    {"straightairpipe"s, instructions::indexed_variable::straightAirPipe},           //
		    {"doors"s, instructions::indexed_variable::doors},                               //
		    {"leftdoors"s, instructions::indexed_variable::leftDoors},                       //
		    {"rightdoors"s, instructions::indexed_variable::rightDoors},                     //
		    {"leftdoorstarget"s, instructions::indexed_variable::leftDoorsTarget},           //
		    {"rightdoorstarget"s, instructions::indexed_variable::rightDoorsTarget},         //
		    {"pluginstate"s, instructions::indexed_variable::pluginState},                   //
		    {"frontaxlecurveradius"s, instructions::indexed_variable::FrontAxleCurveRadius}, //
		    {"rearaxlecurveradius"s, instructions::indexed_variable::RearAxleCurveRadius},   //
		    {"curvecant"s, instructions::indexed_variable::CurveCant},                       //
		};

		static const std::map<std::string, instruction> unary_functions = {
		    {"minus"s, instructions::op_unary_minus{}},       //
		    {"not"s, instructions::op_unary_not{}},           //
		    {"reciprocal"s, instructions::func_reciprocal{}}, //
		    {"abs"s, instructions::func_abs{}},               //
		    {"sign"s, instructions::func_sign{}},             //
		    {"floor"s, instructions::func_floor{}},           //
		    {"ceiling"s, instructions::func_ceiling{}},       //
		    {"round"s, instructions::func_round{}},           //
		    {"exp"s, instructions::func_exp{}},               //
		    {"log"s, instructions::func_log{}},               //
		    {"sqrt"s, instructions::func_sqrt{}},             //
		    {"sin"s, instructions::func_sin{}},               //
		    {"cos"s, instructions::func_cos{}},               //
		    {"tan"s, instructions::func_tan{}},               //
		    {"arctan"s, instructions::func_arctan{}},         //
		};

		static const std::map<std::string, instruction> binary_functions = {
		    {"subtract"s, instructions::op_subtract{}},          //
		    {"divide"s, instructions::op_divide{}},              //
		    {"equal"s, instructions::op_equal{}},                //
		    {"unequal"s, instructions::op_unequal{}},            //
		    {"less"s, instructions::op_less{}},                  //
		    {"greater"s, instructions::op_greater{}},            //
		    {"lessequal"s, instructions::op_less_equal{}},       //
		    {"greaterequal"s, instructions::op_greater_equal{}}, //
		    {"and"s, instructions::op_and{}},                    //
		    {"or"s, instructions::op_or{}},                      //
		    {"xor"s, instructions::op_xor{}},                    //
		    {"quotient"s, instructions::func_quotient{}},        //
		    {"mod"s, instructions::func_mod{}},                  //
		    {"random"s, instructions::func_random{}},            //
		    {"randomint"s, instructions::func_randomInt{}},      //
		};

		static const std::set<std::string> variatic_functions{
		    {"plus"s},  //
		    {"times"s}, //
		    {"power"s}, //
		    {"min"s},   //
		    {"max"s},   //
		};
	} // namespace

	struct intruction_builder_helper {
		instruction_list list;

		void call_next_node(const tree_node& next) {
			mapbox::util::apply_visitor(*this, next);
		}

		template <class R, class T>
		void generate_binary_func(const T& node) {
			call_next_node(node.left);
			call_next_node(node.right);
			list.instructions.emplace_back<R>(R{});
		}

		template <class R, class T>
		void generate_unary_func(const T& node) {
			call_next_node(node.child);
			list.instructions.emplace_back<R>(R{});
		}

		void operator()(const tree_types::binary_and& node) {
			generate_binary_func<instructions::op_and>(node);
		}
		void operator()(const tree_types::binary_xor& node) {
			generate_binary_func<instructions::op_xor>(node);
		}
		void operator()(const tree_types::binary_or& node) {
			generate_binary_func<instructions::op_or>(node);
		}
		void operator()(const tree_types::binary_eq& node) {
			generate_binary_func<instructions::op_equal>(node);
		}
		void operator()(const tree_types::binary_not_eq& node) {
			generate_binary_func<instructions::op_unequal>(node);
		}
		void operator()(const tree_types::binary_less& node) {
			generate_binary_func<instructions::op_less>(node);
		}
		void operator()(const tree_types::binary_greater& node) {
			generate_binary_func<instructions::op_greater>(node);
		}
		void operator()(const tree_types::binary_less_eq& node) {
			generate_binary_func<instructions::op_less_equal>(node);
		}
		void operator()(const tree_types::binary_greater_eq& node) {
			generate_binary_func<instructions::op_greater_equal>(node);
		}
		void operator()(const tree_types::binary_add& node) {
			generate_binary_func<instructions::op_add>(node);
		}
		void operator()(const tree_types::binary_subtract& node) {
			generate_binary_func<instructions::op_subtract>(node);
		}
		void operator()(const tree_types::binary_multiply& node) {
			generate_binary_func<instructions::op_multiply>(node);
		}
		void operator()(const tree_types::binary_divide& node) {
			generate_binary_func<instructions::op_divide>(node);
		}
		void operator()(const tree_types::unary_not& node) {
			generate_unary_func<instructions::op_unary_not>(node);
		}
		void operator()(const tree_types::unary_minus& node) {
			generate_unary_func<instructions::op_unary_minus>(node);
		}
		void operator()(const tree_types::integer& node) {
			list.instructions.emplace_back<instructions::stack_push>({float(node.num)});
		}
		void operator()(const tree_types::floating& node) {
			list.instructions.emplace_back<instructions::stack_push>({float(node.num)});
		}
		void operator()(const tree_types::none& node) {
			(void) node;
			list.instructions.emplace_back<instructions::stack_push>({0});
		}

		void operator()(const tree_types::name& node) {
			auto lower_name = util::lower_copy(node.val);

			auto iter = naked_variables.find(lower_name);
			if (iter != naked_variables.end()) {
				list.instructions.emplace_back<instructions::op_variable_lookup>({iter->second});
				list.used_variables.insert(iter->second);
			}
			else {
				std::ostringstream error;
				error << "Variable \"" << node.val << "\" not found";
				list.errors.emplace_back<errors::error_t>({0, error.str()});
				list.instructions.emplace_back<instructions::stack_push>({0});
			}
		}
		void operator()(const tree_types::function_call& node) {
			auto arg_count = node.args.size();
			auto lower_name = util::lower_copy(node.name.val);

			// unary
			auto unary_iter = unary_functions.find(lower_name);
			if (unary_iter != unary_functions.end()) {
				if (arg_count < 1) {
					std::ostringstream error;
					error << "Function \"" << node.name.val << "\" requires 1 argument.";
					list.errors.emplace_back<errors::error_t>({0, error.str()});
					list.instructions.emplace_back<instructions::stack_push>({0});
				}
				else {
					call_next_node(node.args[0]);
					list.instructions.emplace_back(unary_iter->second);
				}
				return;
			}
			// unary indexing
			auto index_iter = indexable_variables.find(lower_name);
			if (index_iter != indexable_variables.end()) {
				if (arg_count >= 1) {
					call_next_node(node.args[0]);
					list.instructions.emplace_back<instructions::op_variable_indexed>({index_iter->second});
					list.used_indexed_variables.insert(index_iter->second);
				}
				else {
					list.instructions.emplace_back<instructions::stack_push>({0});
				}
				if (arg_count != 1) {
					std::ostringstream error;
					error << "Variable \"" << node.name.val << "\" must be indexed with one argument.";
					list.errors.emplace_back<errors::error_t>({0, error.str()});
				}
				return;
			}

			// binary
			auto binary_iter = binary_functions.find(lower_name);
			if (binary_iter != binary_functions.end()) {
				if (arg_count >= 2) {
					call_next_node(node.args[0]);
					call_next_node(node.args[1]);
					list.instructions.emplace_back(binary_iter->second);
				}
				else {
					list.instructions.emplace_back<instructions::stack_push>({0});
				}
				if (arg_count != 2) {
					std::ostringstream error;
					error << "Function \"" << node.name.val << "\" requires 2 arguments.";
					list.errors.emplace_back<errors::error_t>({0, error.str()});
				}
				return;
			}

			// ternary
			if (lower_name == "if"s) {
				if (arg_count >= 3) {
					call_next_node(node.args[0]);
					call_next_node(node.args[1]);
					call_next_node(node.args[2]);
					list.instructions.emplace_back<instructions::func_if>({});
				}
				else if (arg_count == 2) {
					call_next_node(node.args[0]);
					call_next_node(node.args[1]);
					list.instructions.emplace_back<instructions::stack_push>({0});
					list.instructions.emplace_back<instructions::func_if>({});
				}
				else if (arg_count == 1) {
					call_next_node(node.args[0]);
				}
				else if (arg_count == 0) {
					list.instructions.emplace_back<instructions::stack_push>({0});
				}
				if (arg_count != 3) {
					std::ostringstream error;
					error << "Function \"" << node.name.val << "\" requires 3 arguments.";
					list.errors.emplace_back<errors::error_t>({0, error.str()});
				}
				return;
			}

			// variatic
			auto variatic_iter = variatic_functions.find(lower_name);
			if (variatic_iter != variatic_functions.end()) {
				for (auto& arg : node.args) {
					call_next_node(arg);
				}

				auto creator = [this](auto&& s, std::size_t count) {
					s.count = count;
					list.instructions.emplace_back(s);
				};

				if (lower_name == "plus"s) {
					creator(instructions::op_add{}, arg_count);
				}
				else if (lower_name == "times"s) {
					creator(instructions::op_multiply{}, arg_count);
				}
				else if (lower_name == "power"s) {
					creator(instructions::func_power{}, arg_count);
				}
				else if (lower_name == "min"s) {
					creator(instructions::func_min{}, arg_count);
				}
				else if (lower_name == "max"s) {
					creator(instructions::func_max{}, arg_count);
				}
				return;
			}
			else {
				std::ostringstream error;
				error << "Function \"" << node.name.val << "\" not recognized.";
				list.errors.emplace_back<errors::error_t>({0, error.str()});

				list.instructions.emplace_back<instructions::stack_push>({0});
				return;
			}
		}
	};

	instruction_list build_instructions(const tree_node& head_node, const errors::errors_t& errors) {
		intruction_builder_helper ibh;
		std::copy(errors.begin(), errors.end(), std::back_inserter(ibh.list.errors));
		mapbox::util::apply_visitor(ibh, head_node);
		return ibh.list;
	}
} // namespace function_scripts
} // namespace parsers
