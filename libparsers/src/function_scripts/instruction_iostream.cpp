#include "parsers/function_scripts.hpp"
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <ostream>
#include <sstream>
#include <tuple>

using namespace std::string_literals;

namespace parsers {
namespace function_scripts {
	namespace {
		std::string get_name(const instructions::variable& var) {
			switch (var) {
				default:
				case instructions::variable::none:
					return "none"s;
				case instructions::variable::value:
					return "value"s;
				case instructions::variable::delta:
					return "delta"s;
				case instructions::variable::currentState:
					return "currentState"s;
				case instructions::variable::time:
					return "time"s;
				case instructions::variable::cameraDistance:
					return "cameraDistance"s;
				case instructions::variable::cameraMode:
					return "cameraMode"s;
				case instructions::variable::cars:
					return "cars"s;
				case instructions::variable::speed:
					return "speed"s;
				case instructions::variable::speedometer:
					return "speedometer"s;
				case instructions::variable::acceleration:
					return "acceleration"s;
				case instructions::variable::accelerationMotor:
					return "accelerationMotor"s;
				case instructions::variable::distance:
					return "distance"s;
				case instructions::variable::trackDistance:
					return "trackDistance"s;
				case instructions::variable::mainReservoir:
					return "mainReservoir"s;
				case instructions::variable::emergencyReservoir:
					return "emergencyReservoir"s;
				case instructions::variable::brakePipe:
					return "brakePipe"s;
				case instructions::variable::brakeCylinder:
					return "brakeCylinder"s;
				case instructions::variable::straightAirPipe:
					return "straightAirPipe"s;
				case instructions::variable::doors:
					return "doors"s;
				case instructions::variable::leftDoors:
					return "leftDoors"s;
				case instructions::variable::rightDoors:
					return "rightDoors"s;
				case instructions::variable::leftDoorsTarget:
					return "leftDoorsTarget"s;
				case instructions::variable::rightDoorsTarget:
					return "rightDoorsTarget"s;
				case instructions::variable::leftDoorsButton:
					return "leftDoorsButton"s;
				case instructions::variable::rightDoorsButton:
					return "rightDoorsButton"s;
				case instructions::variable::reverserNotch:
					return "reverserNotch"s;
				case instructions::variable::powerNotch:
					return "powerNotch"s;
				case instructions::variable::powerNotches:
					return "powerNotches"s;
				case instructions::variable::brakeNotch:
					return "brakeNotch"s;
				case instructions::variable::brakeNotches:
					return "brakeNotches"s;
				case instructions::variable::brakeNotchLinear:
					return "brakeNotchLinear"s;
				case instructions::variable::brakeNotchesLinear:
					return "brakeNotchesLinear"s;
				case instructions::variable::emergencyBrake:
					return "emergencyBrake"s;
				case instructions::variable::hasAirBrake:
					return "hasAirBrake"s;
				case instructions::variable::holdBrake:
					return "holdBrake"s;
				case instructions::variable::hasHoldBrake:
					return "hasHoldBrake"s;
				case instructions::variable::constSpeed:
					return "constSpeed"s;
				case instructions::variable::hasConstSpeed:
					return "hasConstSpeed"s;
				case instructions::variable::hasPlugin:
					return "hasPlugin"s;
				case instructions::variable::Odometer:
					return "Odometer"s;
				case instructions::variable::Klaxon:
					return "Klaxon"s;
				case instructions::variable::PrimaryKlaxon:
					return "PrimaryKlaxon"s;
				case instructions::variable::MusicKlaxon:
					return "MusicKlaxon"s;
				case instructions::variable::section:
					return "section"s;
			}
		}

		std::string get_name(const instructions::indexed_variable& var) {
			switch (var) {
				default:
				case instructions::indexed_variable::none:
					return "none"s;
				case instructions::indexed_variable::speed:
					return "speed"s;
				case instructions::indexed_variable::speedometer:
					return "speedometer"s;
				case instructions::indexed_variable::acceleration:
					return "acceleration"s;
				case instructions::indexed_variable::accelerationMotor:
					return "accelerationMotor"s;
				case instructions::indexed_variable::distance:
					return "distance"s;
				case instructions::indexed_variable::trackDistance:
					return "trackDistance"s;
				case instructions::indexed_variable::mainReservoir:
					return "mainReservoir"s;
				case instructions::indexed_variable::emergencyReservoir:
					return "emergencyReservoir"s;
				case instructions::indexed_variable::brakePipe:
					return "brakePipe"s;
				case instructions::indexed_variable::brakeCylinder:
					return "brakeCylinder"s;
				case instructions::indexed_variable::straightAirPipe:
					return "straightAirPipe"s;
				case instructions::indexed_variable::doors:
					return "doors"s;
				case instructions::indexed_variable::leftDoors:
					return "leftDoors"s;
				case instructions::indexed_variable::rightDoors:
					return "rightDoors"s;
				case instructions::indexed_variable::leftDoorsTarget:
					return "leftDoorsTarget"s;
				case instructions::indexed_variable::rightDoorsTarget:
					return "rightDoorsTarget"s;
				case instructions::indexed_variable::pluginState:
					return "pluginState"s;
				case instructions::indexed_variable::FrontAxleCurveRadius:
					return "FrontAxleCurveRadius"s;
				case instructions::indexed_variable::RearAxleCurveRadius:
					return "RearAxleCurveRadius"s;
				case instructions::indexed_variable::CurveCant:
					return "CurveCant"s;
			}
		}
	} // namespace

	struct function_script_instruction_io_class {
		std::ostream& _os;
		std::vector<std::tuple<std::string, std::string, std::string>> outputs;

		std::size_t last_function_size = 0;
		std::size_t stack_size = 0;
		function_script_instruction_io_class(std::ostream& os) : _os(os){};

		void print() {
			if (outputs.size() == 0) {
				return;
			}
			auto size_name_iter =
			    std::max_element(outputs.begin(), outputs.end(), [](const auto& lhs, const auto& rhs) {
				    return std::get<0>(lhs).size() < std::get<0>(rhs).size();
			    });
			auto size_input_iter =
			    std::max_element(outputs.begin(), outputs.end(), [](const auto& lhs, const auto& rhs) {
				    return std::get<1>(lhs).size() < std::get<1>(rhs).size();
			    });
			auto size_output_iter =
			    std::max_element(outputs.begin(), outputs.end(), [](const auto& lhs, const auto& rhs) {
				    return std::get<2>(lhs).size() < std::get<2>(rhs).size();
			    });

			std::ptrdiff_t size_largest_name = std::get<0>(*size_name_iter).size();
			std::ptrdiff_t size_largest_input = std::get<1>(*size_input_iter).size();
			std::ptrdiff_t size_largest_output = std::get<2>(*size_output_iter).size();

			std::size_t count = 0;
			for (auto& o : outputs) {
				auto name = std::get<0>(o);
				auto input = std::get<1>(o);
				auto output = std::get<2>(o);

				_os << std::setw(int(std::ceil(std::log10(outputs.size())))) << count << '\t';
				_os << name;
				for (std::ptrdiff_t i = 0; i < size_largest_name - std::ptrdiff_t(name.size()) + 2; ++i) {
					_os << ' ';
				}
				for (std::ptrdiff_t i = 0; i < size_largest_input - std::ptrdiff_t(input.size()) + 1; ++i) {
					_os << ' ';
				}
				_os << input;
				_os << " -> ";
				_os << output;
				for (std::ptrdiff_t i = 0; i < size_largest_output - std::ptrdiff_t(output.size()) + 1; ++i) {
					_os << ' ';
				}
				_os << '\n';
				count++;
			}
		}

		void add_outputs(const std::string& name, const std::string& repr, const std::string& output) {
			outputs.emplace_back(std::make_tuple(name, repr, output));
		}

		void print_unary_expr(const std::string& name, const char* separator) {
			std::ostringstream inputs, output;
			inputs << "(" << separator << stack_size - 1 << ")";
			output << stack_size - 1;
			add_outputs(name, inputs.str(), output.str());
		}

		void print_binary_expr(const std::string& name, const char* separator) {
			std::ostringstream inputs, output;
			inputs << "(" << stack_size - 2 << " " << separator << " " << stack_size - 1 << ")";
			stack_size -= 1;
			output << stack_size - 1;
			add_outputs(name, inputs.str(), output.str());
		}

		void print_variatic_expr(const std::string& name, const char* separator, std::size_t count) {
			std::ostringstream inputs, output;
			inputs << "(";
			for (std::size_t i = count; i > 0; --i) {
				inputs << stack_size - i;
				if (i != 1) {
					inputs << " " << separator << " ";
				}
			}
			inputs << ")";
			stack_size -= count - 1;
			output << stack_size - 1;
			add_outputs(name, inputs.str(), output.str());
		}

		void print_function(const std::string& name, const std::string& input_name, std::size_t count) {
			std::ostringstream inputs, output;
			inputs << input_name << "[";
			for (std::size_t i = count; i > 0; --i) {
				inputs << stack_size - i;
				if (i != 1) {
					inputs << ", ";
				}
			}
			inputs << "]";
			stack_size -= count - 1;
			output << stack_size - 1;
			add_outputs(name, inputs.str(), output.str());
		}

		void operator()(const instructions::stack_push& inst) {
			std::ostringstream name, input, output;
			name << "STACK_PUSH: " << inst.value;
			input << "#" << inst.value;
			output << ++stack_size - 1;
			add_outputs(name.str(), input.str(), output.str());
		}
		void operator()(const instructions::op_add& inst) {
			std::ostringstream name;
			name << "OP_ADD" << inst.count;
			print_variatic_expr(name.str(), "+", inst.count);
		}
		void operator()(const instructions::op_subtract& inst) {
			(void) inst;
			print_binary_expr("OP_SUBTRACT", "-");
		}
		void operator()(const instructions::op_unary_minus& inst) {
			(void) inst;
			print_unary_expr("OP_UNARY_MINUS"s, "-");
		}
		void operator()(const instructions::op_multiply& inst) {
			std::ostringstream name;
			name << "OP_MULTIPLY" << inst.count;
			print_variatic_expr(name.str(), "*", inst.count);
		}
		void operator()(const instructions::op_divide& inst) {
			(void) inst;
			print_binary_expr("OP_DIVIDE"s, "/");
		}
		void operator()(const instructions::op_equal& inst) {
			(void) inst;
			print_binary_expr("OP_EQUAL"s, "/");
		}
		void operator()(const instructions::op_unequal& inst) {
			(void) inst;
			print_binary_expr("OP_UNEQUAL"s, "!=");
		}
		void operator()(const instructions::op_less& inst) {
			(void) inst;
			print_binary_expr("OP_LESS"s, "<");
		}
		void operator()(const instructions::op_greater& inst) {
			(void) inst;
			print_binary_expr("OP_GREATER"s, ">");
		}
		void operator()(const instructions::op_less_equal& inst) {
			(void) inst;
			print_binary_expr("OP_LESS_EQUAL"s, "<=");
		}
		void operator()(const instructions::op_greater_equal& inst) {
			(void) inst;
			print_binary_expr("OP_GREATER_EQUAL"s, ">=");
		}
		void operator()(const instructions::op_unary_not& inst) {
			(void) inst;
			print_unary_expr("OP_UNARY_NOT"s, "!");
		}
		void operator()(const instructions::op_and& inst) {
			(void) inst;
			print_binary_expr("OP_AND"s, "&");
		}
		void operator()(const instructions::op_or& inst) {
			(void) inst;
			print_binary_expr("OP_OR"s, "|");
		}
		void operator()(const instructions::op_xor& inst) {
			(void) inst;
			print_binary_expr("OP_XOR"s, "^");
		}
		void operator()(const instructions::op_variable_lookup& inst) {
			std::ostringstream name, output;
			auto&& var_name = get_name(inst.name);
			name << "OP_VARIABLE_LOOKUP: " << var_name;
			output << ++stack_size - 1;
			add_outputs(name.str(), var_name, output.str());
		}
		void operator()(const instructions::op_variable_indexed& inst) {
			std::ostringstream name, input, output;
			auto&& var_name = get_name(inst.name);
			name << "OP_VARIABLE_INDEXED: " << var_name;
			input << var_name << "[" << stack_size - 1 << "]";
			output << stack_size - 1;
			add_outputs(name.str(), input.str(), output.str());
		}
		void operator()(const instructions::func_reciprocal& inst) {
			(void) inst;
			print_function("FUNC_RECIPROCAL"s, "Reciprocal", 1);
		}
		void operator()(const instructions::func_power& inst) {
			std::ostringstream name;
			name << "OP_POWER" << inst.count;
			print_variatic_expr(name.str(), "^", inst.count);
		}
		void operator()(const instructions::func_quotient& inst) {
			(void) inst;
			print_binary_expr("FUNC_QUOTIENT"s, "//");
		}
		void operator()(const instructions::func_mod& inst) {
			(void) inst;
			print_binary_expr("FUNC_MOD"s, "%");
		}
		void operator()(const instructions::func_min& inst) {
			std::ostringstream name;
			name << "OP_MIN" << inst.count;
			print_function(name.str(), "Min", inst.count);
		}
		void operator()(const instructions::func_max& inst) {
			std::ostringstream name;
			name << "OP_MAX" << inst.count;
			print_function(name.str(), "Max", inst.count);
		}
		void operator()(const instructions::func_abs& inst) {
			(void) inst;
			print_function("FUNC_ABS"s, "Abs", 1);
		}
		void operator()(const instructions::func_sign& inst) {
			(void) inst;
			print_function("FUNC_SIGN"s, "Sign", 1);
		}
		void operator()(const instructions::func_floor& inst) {
			(void) inst;
			print_function("FUNC_FLOOR"s, "Floor", 1);
		}
		void operator()(const instructions::func_ceiling& inst) {
			(void) inst;
			print_function("FUNC_CEILING"s, "Ceiling", 1);
		}
		void operator()(const instructions::func_round& inst) {
			(void) inst;
			print_function("FUNC_ROUND"s, "Round", 1);
		}
		void operator()(const instructions::func_random& inst) {
			(void) inst;
			print_function("FUNC_RANDOM"s, "Random", 2);
		}
		void operator()(const instructions::func_randomInt& inst) {
			(void) inst;
			print_function("FUNC_RANDOMINT"s, "RoundInt", 2);
		}
		void operator()(const instructions::func_exp& inst) {
			(void) inst;
			print_function("FUNC_EXP"s, "Exp", 1);
		}
		void operator()(const instructions::func_log& inst) {
			(void) inst;
			print_function("FUNC_LOG"s, "Log", 1);
		}
		void operator()(const instructions::func_sqrt& inst) {
			(void) inst;
			print_function("FUNC_SQRT"s, "Sqrt", 1);
		}
		void operator()(const instructions::func_sin& inst) {
			(void) inst;
			print_function("FUNC_SIN"s, "Sin", 1);
		}
		void operator()(const instructions::func_cos& inst) {
			(void) inst;
			print_function("FUNC_COS"s, "Cos", 1);
		}
		void operator()(const instructions::func_tan& inst) {
			(void) inst;
			print_function("FUNC_TAN"s, "Tan", 1);
		}
		void operator()(const instructions::func_arctan& inst) {
			(void) inst;
			print_function("FUNC_ARCTAN"s, "Arctan", 1);
		}
		void operator()(const instructions::func_if& inst) {
			(void) inst;
			print_function("FUNC_IF"s, "If", 3);
		}
	};

	std::ostream& operator<<(std::ostream& os, const instruction_list& list) {
		function_script_instruction_io_class fsiic(os);

		if (!list.used_variables.empty()) {
			os << "Variables Used: ";
		}

		std::size_t i = 0;
		for (auto& var : list.used_variables) {
			os << get_name(var);
			if (i != list.used_variables.size() - 1) {
				os << ", ";
			}
			++i;
		}
		if (!list.used_variables.empty()) {
			os << '\n';
		}

		if (!list.used_indexed_variables.empty()) {
			os << "Index Variables Used: ";
		}
		i = 0;
		for (auto& var : list.used_indexed_variables) {
			os << get_name(var);
			if (i != list.used_indexed_variables.size() - 1) {
				os << ", ";
			}
			++i;
		}
		if (!list.used_indexed_variables.empty()) {
			os << '\n';
		}

		for (auto& instr : list.instructions) {
			mapbox::util::apply_visitor(fsiic, instr);
		}

		fsiic.print();

		return os;
	}
} // namespace function_scripts
} // namespace parsers
