#include "parsers/function_scripts.hpp"
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <ostream>
#include <sstream>
#include <tuple>

using namespace std::string_literals;

namespace bve::parsers::function_scripts {
	namespace {
		// ReSharper disable once CyclomaticComplexity
		std::string get_name(const instructions::variable& var) {
			switch (var) {
				default:
				case instructions::variable::none:
					return "none"s;
				case instructions::variable::value:
					return "value"s;
				case instructions::variable::delta:
					return "delta"s;
				case instructions::variable::current_state:
					return "current_state"s;
				case instructions::variable::time:
					return "time"s;
				case instructions::variable::camera_distance:
					return "camera_distance"s;
				case instructions::variable::camera_mode:
					return "camera_mode"s;
				case instructions::variable::cars:
					return "cars"s;
				case instructions::variable::speed:
					return "speed"s;
				case instructions::variable::speedometer:
					return "speedometer"s;
				case instructions::variable::acceleration:
					return "acceleration"s;
				case instructions::variable::acceleration_motor:
					return "acceleration_motor"s;
				case instructions::variable::distance:
					return "distance"s;
				case instructions::variable::track_distance:
					return "track_distance"s;
				case instructions::variable::main_reservoir:
					return "main_reservoir"s;
				case instructions::variable::emergency_reservoir:
					return "emergency_reservoir"s;
				case instructions::variable::brake_pipe:
					return "brake_pipe"s;
				case instructions::variable::brake_cylinder:
					return "brake_cylinder"s;
				case instructions::variable::straight_air_pipe:
					return "straight_air_pipe"s;
				case instructions::variable::doors:
					return "doors"s;
				case instructions::variable::left_doors:
					return "left_doors"s;
				case instructions::variable::right_doors:
					return "right_doors"s;
				case instructions::variable::left_doors_target:
					return "left_doors_target"s;
				case instructions::variable::right_doors_target:
					return "right_doors_target"s;
				case instructions::variable::left_doors_button:
					return "left_doors_button"s;
				case instructions::variable::right_doors_button:
					return "right_doors_button"s;
				case instructions::variable::reverser_notch:
					return "reverser_notch"s;
				case instructions::variable::power_notch:
					return "power_notch"s;
				case instructions::variable::power_notches:
					return "power_notches"s;
				case instructions::variable::brake_notch:
					return "brake_notch"s;
				case instructions::variable::brake_notches:
					return "brake_notches"s;
				case instructions::variable::brake_notch_linear:
					return "brake_notch_linear"s;
				case instructions::variable::brake_notches_linear:
					return "brake_notches_linear"s;
				case instructions::variable::emergency_brake:
					return "emergency_brake"s;
				case instructions::variable::has_air_brake:
					return "has_air_brake"s;
				case instructions::variable::hold_brake:
					return "hold_brake"s;
				case instructions::variable::has_hold_brake:
					return "has_hold_brake"s;
				case instructions::variable::const_speed:
					return "const_speed"s;
				case instructions::variable::has_const_speed:
					return "has_const_speed"s;
				case instructions::variable::has_plugin:
					return "has_plugin"s;
				case instructions::variable::odometer:
					return "odometer"s;
				case instructions::variable::klaxon:
					return "klaxon"s;
				case instructions::variable::primary_klaxon:
					return "primary_klaxon"s;
				case instructions::variable::secondary_klaxon:
					return "secondary_klaxon"s;
				case instructions::variable::music_klaxon:
					return "music_klaxon"s;
				case instructions::variable::section:
					return "section"s;
			}
		}

		// ReSharper disable once CyclomaticComplexity
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
				case instructions::indexed_variable::acceleration_motor:
					return "acceleration_motor"s;
				case instructions::indexed_variable::distance:
					return "distance"s;
				case instructions::indexed_variable::track_distance:
					return "track_distance"s;
				case instructions::indexed_variable::main_reservoir:
					return "main_reservoir"s;
				case instructions::indexed_variable::emergency_reservoir:
					return "emergency_reservoir"s;
				case instructions::indexed_variable::brake_pipe:
					return "brake_pipe"s;
				case instructions::indexed_variable::brake_cylinder:
					return "brake_cylinder"s;
				case instructions::indexed_variable::straight_air_pipe:
					return "straight_air_pipe"s;
				case instructions::indexed_variable::doors:
					return "doors"s;
				case instructions::indexed_variable::left_doors:
					return "left_doors"s;
				case instructions::indexed_variable::right_doors:
					return "right_doors"s;
				case instructions::indexed_variable::left_doors_target:
					return "left_doors_target"s;
				case instructions::indexed_variable::right_doors_target:
					return "right_doors_target"s;
				case instructions::indexed_variable::plugin_state:
					return "plugin_state"s;
				case instructions::indexed_variable::front_axle_curve_radius:
					return "front_axle_curve_radius"s;
				case instructions::indexed_variable::rear_axle_curve_radius:
					return "rear_axle_curve_radius"s;
				case instructions::indexed_variable::curve_cant:
					return "curve_cant"s;
				case instructions::indexed_variable::odometer:
					return "odometer"s;
			}
		}
	} // namespace

	struct function_script_instruction_io_class {
	  private:
		std::ostream& os_;
		std::size_t stack_size_ = 0;

	  public:
		std::vector<std::tuple<std::string, std::string, std::string>> outputs;

		explicit function_script_instruction_io_class(std::ostream& os) : os_(os) {}

		void print() {
			if (outputs.empty()) {
				return;
			}
			auto const size_name_iter =
			    std::max_element(outputs.begin(), outputs.end(),
			                     [](const auto& lhs, const auto& rhs) {
				                     return std::get<0>(lhs).size() < std::get<0>(rhs).size();
			                     });
			auto const size_input_iter =
			    std::max_element(outputs.begin(), outputs.end(),
			                     [](const auto& lhs, const auto& rhs) {
				                     return std::get<1>(lhs).size() < std::get<1>(rhs).size();
			                     });
			auto const size_output_iter =
			    std::max_element(outputs.begin(), outputs.end(),
			                     [](const auto& lhs, const auto& rhs) {
				                     return std::get<2>(lhs).size() < std::get<2>(rhs).size();
			                     });

			std::ptrdiff_t const size_largest_name = std::get<0>(*size_name_iter).size();
			std::ptrdiff_t const size_largest_input = std::get<1>(*size_input_iter).size();
			std::ptrdiff_t const size_largest_output = std::get<2>(*size_output_iter).size();

			std::size_t count = 0;
			for (auto& o : outputs) {
				auto name = std::get<0>(o);
				auto input = std::get<1>(o);
				auto output = std::get<2>(o);

				os_ << std::setw(int(std::ceil(std::log10(outputs.size())))) << count << '\t';
				os_ << name;
				for (std::ptrdiff_t i = 0; i < size_largest_name - std::ptrdiff_t(name.size()) + 2;
				     ++i) {
					os_ << ' ';
				}
				for (std::ptrdiff_t i = 0;
				     i < size_largest_input - std::ptrdiff_t(input.size()) + 1; ++i) {
					os_ << ' ';
				}
				os_ << input;
				os_ << " -> ";
				for (std::ptrdiff_t i = 0; i < size_largest_output - std::ptrdiff_t(output.size());
				     ++i) {
					os_ << ' ';
				}
				os_ << output;
				os_ << '\n';
				count++;
			}
		}

		void add_outputs(const std::string& name,
		                 const std::string& repr,
		                 const std::string& output) {
			outputs.emplace_back(std::make_tuple(name, repr, output));
		}

		void print_unary_expr(const std::string& name, const char* separator) {
			std::ostringstream inputs, output;
			inputs << "(" << separator << stack_size_ - 1 << ")";
			output << stack_size_ - 1;
			add_outputs(name, inputs.str(), output.str());
		}

		void print_binary_expr(const std::string& name, const char* separator) {
			std::ostringstream inputs, output;
			inputs << "(" << stack_size_ - 2 << " " << separator << " " << stack_size_ - 1 << ")";
			stack_size_ -= 1;
			output << stack_size_ - 1;
			add_outputs(name, inputs.str(), output.str());
		}

		void print_variatic_expr(const std::string& name,
		                         const char* separator,
		                         std::size_t const count) {
			std::ostringstream inputs, output;
			inputs << "(";
			// ReSharper disable once CppUseAuto
			for (std::size_t i = count; i > 0; --i) {
				inputs << stack_size_ - i;
				if (i != 1) {
					inputs << " " << separator << " ";
				}
			}
			inputs << ")";
			stack_size_ -= count - 1;
			output << stack_size_ - 1;
			add_outputs(name, inputs.str(), output.str());
		}

		void print_function(const std::string& name,
		                    const std::string& input_name,
		                    std::size_t const count) {
			std::ostringstream inputs, output;
			inputs << input_name << "[";
			// ReSharper disable once CppUseAuto
			for (std::size_t i = count; i > 0; --i) {
				inputs << stack_size_ - i;
				if (i != 1) {
					inputs << ", ";
				}
			}
			inputs << "]";
			stack_size_ -= count - 1;
			output << stack_size_ - 1;
			add_outputs(name, inputs.str(), output.str());
		}

		void operator()(const instructions::stack_push& inst) {
			std::ostringstream name, input, output;
			name << "STACK_PUSH: " << inst.value;
			input << "#" << inst.value;
			output << ++stack_size_ - 1;
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
			print_binary_expr("OP_EQUAL"s, "==");
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
			output << ++stack_size_ - 1;
			add_outputs(name.str(), var_name, output.str());
		}
		void operator()(const instructions::op_variable_indexed& inst) {
			std::ostringstream name, input, output;
			auto&& var_name = get_name(inst.name);
			name << "OP_VARIABLE_INDEXED: " << var_name;
			input << var_name << "[" << stack_size_ - 1 << "]";
			output << stack_size_ - 1;
			add_outputs(name.str(), input.str(), output.str());
		}
		void operator()(const instructions::func_reciprocal& inst) {
			(void) inst;
			print_function("FUNC_RECIPROCAL"s, "Reciprocal", 1);
		}
		void operator()(const instructions::func_power& inst) {
			std::ostringstream name;
			name << "FUNC_POWER" << inst.count;
			print_variatic_expr(name.str(), "**", inst.count);
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
			name << "FUNC_MIN" << inst.count;
			print_function(name.str(), "Min", inst.count);
		}
		void operator()(const instructions::func_max& inst) {
			std::ostringstream name;
			name << "FUNC_MAX" << inst.count;
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
		void operator()(const instructions::func_random_int& inst) {
			(void) inst;
			print_function("FUNC_RANDOMINT"s, "RandomInt", 2);
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

		for (auto& inst : list.instructions) {
			apply_visitor(fsiic, inst);
		}

		fsiic.print();

		return os;
	}
} // namespace bve::parsers::function_scripts
