#include "function_scripts.hpp"
#include <map>
#include <set>

using namespace std::string_literals;

namespace parsers {
namespace function_scripts {
	namespace {
		static std::map<std::string, instructions::variable> naked_variables = {
		    {"value"s, instructions::variable::value},                           //
		    {"delta"s, instructions::variable::delta},                           //
		    {"currentState"s, instructions::variable::currentState},             //
		    {"time"s, instructions::variable::time},                             //
		    {"cameraDistance"s, instructions::variable::cameraDistance},         //
		    {"cameraMode"s, instructions::variable::cameraMode},                 //
		    {"cars"s, instructions::variable::cars},                             //
		    {"speed"s, instructions::variable::speed},                           //
		    {"speedometer"s, instructions::variable::speedometer},               //
		    {"acceleration"s, instructions::variable::acceleration},             //
		    {"accelerationMotor"s, instructions::variable::accelerationMotor},   //
		    {"distance"s, instructions::variable::distance},                     //
		    {"trackDistance"s, instructions::variable::trackDistance},           //
		    {"mainReservoir"s, instructions::variable::mainReservoir},           //
		    {"emergencyReservoir"s, instructions::variable::emergencyReservoir}, //
		    {"brakePipe"s, instructions::variable::brakePipe},                   //
		    {"brakeCylinder"s, instructions::variable::brakeCylinder},           //
		    {"straightAirPipe"s, instructions::variable::straightAirPipe},       //
		    {"doors"s, instructions::variable::doors},                           //
		    {"leftDoors"s, instructions::variable::leftDoors},                   //
		    {"rightDoors"s, instructions::variable::rightDoors},                 //
		    {"leftDoorsTarget"s, instructions::variable::leftDoorsTarget},       //
		    {"rightDoorsTarget"s, instructions::variable::rightDoorsTarget},     //
		    {"leftDoorsButton"s, instructions::variable::leftDoorsButton},       //
		    {"rightDoorsButton"s, instructions::variable::rightDoorsButton},     //
		    {"reverserNotch"s, instructions::variable::reverserNotch},           //
		    {"powerNotch"s, instructions::variable::powerNotch},                 //
		    {"powerNotches"s, instructions::variable::powerNotches},             //
		    {"brakeNotch"s, instructions::variable::brakeNotch},                 //
		    {"brakeNotches"s, instructions::variable::brakeNotches},             //
		    {"brakeNotchLinear"s, instructions::variable::brakeNotchLinear},     //
		    {"brakeNotchesLinear"s, instructions::variable::brakeNotchesLinear}, //
		    {"emergencyBrake"s, instructions::variable::emergencyBrake},         //
		    {"hasAirBrake"s, instructions::variable::hasAirBrake},               //
		    {"holdBrake"s, instructions::variable::holdBrake},                   //
		    {"hasHoldBrake"s, instructions::variable::hasHoldBrake},             //
		    {"constSpeed"s, instructions::variable::constSpeed},                 //
		    {"hasConstSpeed"s, instructions::variable::hasConstSpeed},           //
		    {"hasPlugin"s, instructions::variable::hasPlugin},                   //
		    {"Odometer"s, instructions::variable::Odometer},                     //
		    {"Klaxon"s, instructions::variable::Klaxon},                         //
		    {"PrimaryKlaxon"s, instructions::variable::PrimaryKlaxon},           //
		    {"MusicKlaxon"s, instructions::variable::MusicKlaxon},               //
		    {"section"s, instructions::variable::section},                       //
		};

		static std::map<std::string, instructions::indexed_variable> indexable_variables = {
		    {"speed"s, instructions::indexed_variable::speed},                               //
		    {"speedometer"s, instructions::indexed_variable::speedometer},                   //
		    {"acceleration"s, instructions::indexed_variable::acceleration},                 //
		    {"accelerationMotor"s, instructions::indexed_variable::accelerationMotor},       //
		    {"distance"s, instructions::indexed_variable::distance},                         //
		    {"trackDistance"s, instructions::indexed_variable::trackDistance},               //
		    {"mainReservoir"s, instructions::indexed_variable::mainReservoir},               //
		    {"emergencyReservoir"s, instructions::indexed_variable::emergencyReservoir},     //
		    {"brakePipe"s, instructions::indexed_variable::brakePipe},                       //
		    {"brakeCylinder"s, instructions::indexed_variable::brakeCylinder},               //
		    {"straightAirPipe"s, instructions::indexed_variable::straightAirPipe},           //
		    {"doors"s, instructions::indexed_variable::doors},                               //
		    {"leftDoors"s, instructions::indexed_variable::leftDoors},                       //
		    {"rightDoors"s, instructions::indexed_variable::rightDoors},                     //
		    {"leftDoorsTarget"s, instructions::indexed_variable::leftDoorsTarget},           //
		    {"rightDoorsTarget"s, instructions::indexed_variable::rightDoorsTarget},         //
		    {"pluginState"s, instructions::indexed_variable::pluginState},                   //
		    {"FrontAxleCurveRadius"s, instructions::indexed_variable::FrontAxleCurveRadius}, //
		    {"RearAxleCurveRadius"s, instructions::indexed_variable::RearAxleCurveRadius},   //
		    {"CurveCant"s, instructions::indexed_variable::CurveCant},                       //
		};

		static std::map<std::string, instruction> unary_functions = {
		    {"Minus"s, instructions::op_unary_minus{}},       //
		    {"Not"s, instructions::op_unary_not{}},           //
		    {"Reciprocal"s, instructions::func_reciprocal{}}, //
		    {"Abs"s, instructions::func_abs{}},               //
		    {"Sign"s, instructions::func_sign{}},             //
		    {"Floor"s, instructions::func_floor{}},           //
		    {"Ceiling"s, instructions::func_ceiling{}},       //
		    {"Round"s, instructions::func_round{}},           //
		    {"Exp"s, instructions::func_exp{}},               //
		    {"Log"s, instructions::func_log{}},               //
		    {"Sqrt"s, instructions::func_sqrt{}},             //
		    {"Sin"s, instructions::func_sin{}},               //
		    {"Cos"s, instructions::func_cos{}},               //
		    {"Tan"s, instructions::func_tan{}},               //
		    {"ArcTan"s, instructions::func_arctan{}},         //
		};

		static std::map<std::string, instruction> binary_functions = {
		    {"Subtract"s, instructions::op_subtract{}},          //
		    {"Divide"s, instructions::op_divide{}},              //
		    {"Equal"s, instructions::op_equal{}},                //
		    {"Unequal"s, instructions::op_unequal{}},            //
		    {"Less"s, instructions::op_less{}},                  //
		    {"Greater"s, instructions::op_greater{}},            //
		    {"LessEqual"s, instructions::op_less_equal{}},       //
		    {"GreaterEqual"s, instructions::op_greater_equal{}}, //
		    {"And"s, instructions::op_and{}},                    //
		    {"Or"s, instructions::op_or{}},                      //
		    {"Xor"s, instructions::op_xor{}},                    //
		    {"Quotient"s, instructions::func_quotient{}},        //
		    {"Mod"s, instructions::func_mod{}},                  //
		    {"random"s, instructions::func_random{}},            //
		    {"randomInt"s, instructions::func_randomInt{}},      //
		};

		static std::set<std::string> variatic_functions{
		    {"Plus"s},  //
		    {"Times"s}, //
		    {"Power"s}, //
		    {"Min"s},   //
		    {"Max"s},   //
		};
	} // namespace

	struct intruction_builder_helper : public boost::static_visitor<void> {
		instruction_list list;

		void call_next_node(const tree_node& next) {
			boost::apply_visitor(*this, next);
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
			// noop
		}

		void operator()(const tree_types::name& node) {
			auto iter = naked_variables.find(node.val);
			if (iter != naked_variables.end()) {
				list.instructions.emplace_back<instructions::op_variable_lookup>({iter->second});
				list.used_variables.emplace_back(iter->second);
			}
			else {
				// Todo(sirflankalot): Error
			}
		}
		void operator()(const tree_types::function_call& node) {
			auto arg_count = node.args.size();

			// unary
			auto unary_iter = unary_functions.find(node.name.val);
			if (unary_iter != unary_functions.end()) {
				if (arg_count < 1) {
					// TODO(sirflankalot): Error
				}
				else {
					call_next_node(node.args[0]);
					list.instructions.emplace_back(unary_iter->second);
				}
				return;
			}
			// unary indexing
			auto index_iter = indexable_variables.find(node.name.val);
			if (index_iter != indexable_variables.end()) {
				if (arg_count < 1) {
					// TODO(sirflankalot): Error
				}
				else {
					call_next_node(node.args[0]);
					list.instructions.emplace_back<instructions::op_variable_indexed>({index_iter->second});
					list.used_indexed_variables.emplace_back(index_iter->second);
				}
				return;
			}

			// binary
			auto binary_iter = binary_functions.find(node.name.val);
			if (binary_iter != binary_functions.end()) {
				if (arg_count < 2) {
					// TODO(sirflankalot): Error
				}
				else {
					call_next_node(node.args[0]);
					call_next_node(node.args[1]);
					list.instructions.emplace_back(binary_iter->second);
				}
				return;
			}

			// ternary
			if (node.name.val == "If"s) {
				if (arg_count < 3) {
					// TODO(sirflankalot): Error
				}
				else {
					call_next_node(node.args[0]);
					call_next_node(node.args[1]);
					call_next_node(node.args[2]);
					list.instructions.emplace_back<instructions::func_if>({});
				}
				return;
			}

			// variatic
			auto variatic_iter = variatic_functions.find(node.name.val);
			if (variatic_iter != variatic_functions.end()) {
				for (auto& arg : node.args) {
					call_next_node(arg);
				}

				auto creator = [this](auto&& s, std::size_t count) {
					s.count = count;
					list.instructions.emplace_back(s);
				};

				if (node.name.val == "Plus"s) {
					creator(instructions::op_add{}, arg_count);
				}
				else if (node.name.val == "Times"s) {
					creator(instructions::op_multiply{}, arg_count);
				}
				else if (node.name.val == "Power"s) {
					creator(instructions::func_power{}, arg_count);
				}
				else if (node.name.val == "Min"s) {
					creator(instructions::func_min{}, arg_count);
				}
				else if (node.name.val == "Max"s) {
					creator(instructions::func_max{}, arg_count);
				}
				return;
			}
			else {
				// TODO(sirflankalot): Error
				return;
			}
		}
	};

	instruction_list build_instructions(const tree_node& head_node) {
		intruction_builder_helper ibh;
		boost::apply_visitor(ibh, head_node);
		return ibh.list;
	}
} // namespace function_scripts
} // namespace parsers