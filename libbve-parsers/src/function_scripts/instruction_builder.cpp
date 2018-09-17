#include "function_scripts.hpp"
#include "utils.hpp"
#include <algorithm>
#include <iterator>
#include <map>
#include <set>
#include <sstream>

using namespace std::string_literals;

namespace parsers::function_scripts {
	namespace {
		const std::map<std::string, instructions::variable> naked_variables = {
		    {"value"s, instructions::variable::value},                             //
		    {"delta"s, instructions::variable::delta},                             //
		    {"currentstate"s, instructions::variable::current_state},              //
		    {"time"s, instructions::variable::time},                               //
		    {"cameradistance"s, instructions::variable::camera_distance},          //
		    {"cameramode"s, instructions::variable::camera_mode},                  //
		    {"cars"s, instructions::variable::cars},                               //
		    {"speed"s, instructions::variable::speed},                             //
		    {"speedometer"s, instructions::variable::speedometer},                 //
		    {"acceleration"s, instructions::variable::acceleration},               //
		    {"accelerationmotor"s, instructions::variable::acceleration_motor},    //
		    {"distance"s, instructions::variable::distance},                       //
		    {"trackdistance"s, instructions::variable::track_distance},            //
		    {"mainreservoir"s, instructions::variable::main_reservoir},            //
		    {"emergencyreservoir"s, instructions::variable::emergency_reservoir},  //
		    {"brakepipe"s, instructions::variable::brake_pipe},                    //
		    {"brakecylinder"s, instructions::variable::brake_cylinder},            //
		    {"straightairpipe"s, instructions::variable::straight_air_pipe},       //
		    {"doors"s, instructions::variable::doors},                             //
		    {"leftdoors"s, instructions::variable::left_doors},                    //
		    {"rightdoors"s, instructions::variable::right_doors},                  //
		    {"leftdoorstarget"s, instructions::variable::left_doors_target},       //
		    {"rightdoorstarget"s, instructions::variable::right_doors_target},     //
		    {"leftdoorsbutton"s, instructions::variable::left_doors_button},       //
		    {"rightdoorsbutton"s, instructions::variable::right_doors_button},     //
		    {"reversernotch"s, instructions::variable::reverser_notch},            //
		    {"powernotch"s, instructions::variable::power_notch},                  //
		    {"powernotches"s, instructions::variable::power_notches},              //
		    {"brakenotch"s, instructions::variable::brake_notch},                  //
		    {"brakenotches"s, instructions::variable::brake_notches},              //
		    {"brakenotchlinear"s, instructions::variable::brake_notch_linear},     //
		    {"brakenotcheslinear"s, instructions::variable::brake_notches_linear}, //
		    {"emergencybrake"s, instructions::variable::emergency_brake},          //
		    {"hasairbrake"s, instructions::variable::has_air_brake},               //
		    {"holdbrake"s, instructions::variable::hold_brake},                    //
		    {"hasholdbrake"s, instructions::variable::has_hold_brake},             //
		    {"constspeed"s, instructions::variable::const_speed},                  //
		    {"hasconstspeed"s, instructions::variable::has_const_speed},           //
		    {"hasplugin"s, instructions::variable::has_plugin},                    //
		    {"odometer"s, instructions::variable::odometer},                       //
		    {"klaxon"s, instructions::variable::klaxon},                           //
		    {"primaryklaxon"s, instructions::variable::primary_klaxon},            //
		    {"secondaryklaxon"s, instructions::variable::secondary_klaxon},        //
		    {"musicklaxon"s, instructions::variable::music_klaxon},                //
		    {"section"s, instructions::variable::section},                         //
		};

		const std::map<std::string, instructions::indexed_variable> indexable_variables = {
		    {"speed"s, instructions::indexed_variable::speed},                                  //
		    {"speedometer"s, instructions::indexed_variable::speedometer},                      //
		    {"acceleration"s, instructions::indexed_variable::acceleration},                    //
		    {"accelerationmotor"s, instructions::indexed_variable::acceleration_motor},         //
		    {"distance"s, instructions::indexed_variable::distance},                            //
		    {"trackdistance"s, instructions::indexed_variable::track_distance},                 //
		    {"mainreservoir"s, instructions::indexed_variable::main_reservoir},                 //
		    {"emergencyreservoir"s, instructions::indexed_variable::emergency_reservoir},       //
		    {"brakepipe"s, instructions::indexed_variable::brake_pipe},                         //
		    {"brakecylinder"s, instructions::indexed_variable::brake_cylinder},                 //
		    {"straightairpipe"s, instructions::indexed_variable::straight_air_pipe},            //
		    {"doors"s, instructions::indexed_variable::doors},                                  //
		    {"leftdoors"s, instructions::indexed_variable::left_doors},                         //
		    {"rightdoors"s, instructions::indexed_variable::right_doors},                       //
		    {"leftdoorstarget"s, instructions::indexed_variable::left_doors_target},            //
		    {"rightdoorstarget"s, instructions::indexed_variable::right_doors_target},          //
		    {"pluginstate"s, instructions::indexed_variable::plugin_state},                     //
		    {"frontaxlecurveradius"s, instructions::indexed_variable::front_axle_curve_radius}, //
		    {"rearaxlecurveradius"s, instructions::indexed_variable::rear_axle_curve_radius},   //
		    {"curvecant"s, instructions::indexed_variable::curve_cant},                         //
		    {"odometer"s, instructions::indexed_variable::odometer},                            //
		};

		const std::map<std::string, instruction> unary_functions = {
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

		const std::map<std::string, instruction> binary_functions = {
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
		    {"randomint"s, instructions::func_random_int{}},     //
		};

		const std::set<std::string> variatic_functions{
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
			apply_visitor(*this, next);
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

		void operator()(const tree_types::identifier& node) {
			auto const lower_name = util::lower_copy(node.val);

			auto const iter = naked_variables.find(lower_name);
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
		// ReSharper disable once CyclomaticComplexity
		void operator()(const tree_types::function_call& node) {
			auto const arg_count = node.args.size();
			auto const lower_name = util::lower_copy(node.name.val);

			// unary
			auto const unary_iter = unary_functions.find(lower_name);
			if (unary_iter != unary_functions.end()) {
				if (arg_count < 1) {
					list.instructions.emplace_back<instructions::stack_push>({0});
				}
				else {
					call_next_node(node.args[0]);
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
					call_next_node(node.args[0]);
					list.instructions.emplace_back<instructions::op_variable_indexed>(
					    {index_iter->second});
					list.used_indexed_variables.insert(index_iter->second);
				}
				else {
					list.instructions.emplace_back<instructions::stack_push>({0});
				}
				if (arg_count != 1) {
					std::ostringstream error;
					error << "Variable \"" << node.name.val
					      << "\" must be indexed with one argument.";
					list.errors.emplace_back<errors::error_t>({0, error.str()});
				}
				return;
			}

			// binary
			auto const binary_iter = binary_functions.find(lower_name);
			if (binary_iter != binary_functions.end()) {
				if (arg_count >= 2) {
					call_next_node(node.args[0]);
					call_next_node(node.args[1]);
					list.instructions.emplace_back(binary_iter->second);
				}
				else if (arg_count == 1) {
					call_next_node(node.args[0]);
					list.instructions.emplace_back<instructions::stack_push>({0});
					list.instructions.emplace_back(binary_iter->second);
				}
				else {
					list.instructions.emplace_back<instructions::stack_push>({0});
					list.instructions.emplace_back<instructions::stack_push>({0});
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
					list.instructions.emplace_back<instructions::stack_push>({0});
					list.instructions.emplace_back<instructions::stack_push>({0});
					list.instructions.emplace_back<instructions::func_if>({});
				}
				else if (arg_count == 0) {
					list.instructions.emplace_back<instructions::stack_push>({0});
					list.instructions.emplace_back<instructions::stack_push>({0});
					list.instructions.emplace_back<instructions::stack_push>({0});
					list.instructions.emplace_back<instructions::func_if>({});
				}
				if (arg_count != 3) {
					std::ostringstream error;
					error << "Function \"" << node.name.val << "\" requires 3 arguments.";
					add_error(list.errors, 0, error);
				}
				return;
			}

			// variatic
			auto const variatic_iter = variatic_functions.find(lower_name);
			if (variatic_iter != variatic_functions.end()) {
				for (auto& arg : node.args) {
					call_next_node(arg);
				}

				auto const creator = [this](auto&& s, std::size_t const count) {
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

			std::ostringstream error;
			error << "Function \"" << node.name.val << "\" not recognized.";
			add_error(list.errors, 0, error);

			list.instructions.emplace_back<instructions::stack_push>({0});
		}
	};

	instruction_list build_instructions(const tree_node& head_node,
	                                    const errors::errors_t& errors) {
		intruction_builder_helper ibh;
		std::copy(errors.begin(), errors.end(), std::back_inserter(ibh.list.errors));
		apply_visitor(ibh, head_node);
		return ibh.list;
	}
} // namespace parsers::function_scripts
