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
		std::string get_name(const instructions::Variable& var) {
			switch (var) {
				default:
				case instructions::Variable::none:
					return "none"s;
				case instructions::Variable::value:
					return "value"s;
				case instructions::Variable::delta:
					return "delta"s;
				case instructions::Variable::current_state:
					return "current_state"s;
				case instructions::Variable::time:
					return "time"s;
				case instructions::Variable::camera_distance:
					return "camera_distance"s;
				case instructions::Variable::camera_mode:
					return "camera_mode"s;
				case instructions::Variable::cars:
					return "cars"s;
				case instructions::Variable::speed:
					return "speed"s;
				case instructions::Variable::speedometer:
					return "speedometer"s;
				case instructions::Variable::acceleration:
					return "acceleration"s;
				case instructions::Variable::acceleration_motor:
					return "acceleration_motor"s;
				case instructions::Variable::distance:
					return "distance"s;
				case instructions::Variable::track_distance:
					return "track_distance"s;
				case instructions::Variable::main_reservoir:
					return "main_reservoir"s;
				case instructions::Variable::emergency_reservoir:
					return "emergency_reservoir"s;
				case instructions::Variable::brake_pipe:
					return "brake_pipe"s;
				case instructions::Variable::brake_cylinder:
					return "brake_cylinder"s;
				case instructions::Variable::straight_air_pipe:
					return "straight_air_pipe"s;
				case instructions::Variable::doors:
					return "doors"s;
				case instructions::Variable::left_doors:
					return "left_doors"s;
				case instructions::Variable::right_doors:
					return "right_doors"s;
				case instructions::Variable::left_doors_target:
					return "left_doors_target"s;
				case instructions::Variable::right_doors_target:
					return "right_doors_target"s;
				case instructions::Variable::left_doors_button:
					return "left_doors_button"s;
				case instructions::Variable::right_doors_button:
					return "right_doors_button"s;
				case instructions::Variable::reverser_notch:
					return "reverser_notch"s;
				case instructions::Variable::power_notch:
					return "power_notch"s;
				case instructions::Variable::power_notches:
					return "power_notches"s;
				case instructions::Variable::brake_notch:
					return "brake_notch"s;
				case instructions::Variable::brake_notches:
					return "brake_notches"s;
				case instructions::Variable::brake_notch_linear:
					return "brake_notch_linear"s;
				case instructions::Variable::brake_notches_linear:
					return "brake_notches_linear"s;
				case instructions::Variable::emergency_brake:
					return "emergency_brake"s;
				case instructions::Variable::has_air_brake:
					return "has_air_brake"s;
				case instructions::Variable::hold_brake:
					return "hold_brake"s;
				case instructions::Variable::has_hold_brake:
					return "has_hold_brake"s;
				case instructions::Variable::const_speed:
					return "const_speed"s;
				case instructions::Variable::has_const_speed:
					return "has_const_speed"s;
				case instructions::Variable::has_plugin:
					return "has_plugin"s;
				case instructions::Variable::odometer:
					return "odometer"s;
				case instructions::Variable::klaxon:
					return "klaxon"s;
				case instructions::Variable::primary_klaxon:
					return "primary_klaxon"s;
				case instructions::Variable::secondary_klaxon:
					return "secondary_klaxon"s;
				case instructions::Variable::music_klaxon:
					return "music_klaxon"s;
				case instructions::Variable::section:
					return "section"s;
			}
		}

		// ReSharper disable once CyclomaticComplexity
		std::string get_name(const instructions::IndexedVariable& var) {
			switch (var) {
				default:
				case instructions::IndexedVariable::none:
					return "none"s;
				case instructions::IndexedVariable::speed:
					return "speed"s;
				case instructions::IndexedVariable::speedometer:
					return "speedometer"s;
				case instructions::IndexedVariable::acceleration:
					return "acceleration"s;
				case instructions::IndexedVariable::acceleration_motor:
					return "acceleration_motor"s;
				case instructions::IndexedVariable::distance:
					return "distance"s;
				case instructions::IndexedVariable::track_distance:
					return "track_distance"s;
				case instructions::IndexedVariable::main_reservoir:
					return "main_reservoir"s;
				case instructions::IndexedVariable::emergency_reservoir:
					return "emergency_reservoir"s;
				case instructions::IndexedVariable::brake_pipe:
					return "brake_pipe"s;
				case instructions::IndexedVariable::brake_cylinder:
					return "brake_cylinder"s;
				case instructions::IndexedVariable::straight_air_pipe:
					return "straight_air_pipe"s;
				case instructions::IndexedVariable::doors:
					return "doors"s;
				case instructions::IndexedVariable::left_doors:
					return "left_doors"s;
				case instructions::IndexedVariable::right_doors:
					return "right_doors"s;
				case instructions::IndexedVariable::left_doors_target:
					return "left_doors_target"s;
				case instructions::IndexedVariable::right_doors_target:
					return "right_doors_target"s;
				case instructions::IndexedVariable::plugin_state:
					return "plugin_state"s;
				case instructions::IndexedVariable::front_axle_curve_radius:
					return "front_axle_curve_radius"s;
				case instructions::IndexedVariable::rear_axle_curve_radius:
					return "rear_axle_curve_radius"s;
				case instructions::IndexedVariable::curve_cant:
					return "curve_cant"s;
				case instructions::IndexedVariable::odometer:
					return "odometer"s;
			}
		}
	} // namespace

	struct FunctionScriptInstructionIOClass {
	  private:
		std::ostream& os_;
		std::size_t stack_size_ = 0;

	  public:
		std::vector<std::tuple<std::string, std::string, std::string>> outputs;

		explicit FunctionScriptInstructionIOClass(std::ostream& os) : os_(os) {}

		void print() {
			if (outputs.empty()) {
				return;
			}
			auto const size_name_iter = std::max_element(outputs.begin(), outputs.end(), [](const auto& lhs, const auto& rhs) {
				return std::get<0>(lhs).size() < std::get<0>(rhs).size();
			});
			auto const size_input_iter = std::max_element(outputs.begin(), outputs.end(), [](const auto& lhs, const auto& rhs) {
				return std::get<1>(lhs).size() < std::get<1>(rhs).size();
			});
			auto const size_output_iter = std::max_element(outputs.begin(), outputs.end(), [](const auto& lhs, const auto& rhs) {
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
				for (std::ptrdiff_t i = 0; i < size_largest_name - std::ptrdiff_t(name.size()) + 2; ++i) {
					os_ << ' ';
				}
				for (std::ptrdiff_t i = 0; i < size_largest_input - std::ptrdiff_t(input.size()) + 1; ++i) {
					os_ << ' ';
				}
				os_ << input;
				os_ << " -> ";
				for (std::ptrdiff_t i = 0; i < size_largest_output - std::ptrdiff_t(output.size()); ++i) {
					os_ << ' ';
				}
				os_ << output;
				os_ << '\n';
				count++;
			}
		}

		void addOutputs(const std::string& name, const std::string& str, const std::string& output) {
			outputs.emplace_back(std::make_tuple(name, str, output));
		}

		void printUnaryExpr(const std::string& name, const char* separator) {
			std::ostringstream inputs;
			std::ostringstream output;
			inputs << "(" << separator << stack_size_ - 1 << ")";
			output << stack_size_ - 1;
			addOutputs(name, inputs.str(), output.str());
		}

		void printBinaryExpr(const std::string& name, const char* separator) {
			std::ostringstream inputs;
			std::ostringstream output;
			inputs << "(" << stack_size_ - 2 << " " << separator << " " << stack_size_ - 1 << ")";
			stack_size_ -= 1;
			output << stack_size_ - 1;
			addOutputs(name, inputs.str(), output.str());
		}

		void printVariadicExpr(const std::string& name, const char* separator, std::size_t const count) {
			std::ostringstream inputs;
			std::ostringstream output;
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
			addOutputs(name, inputs.str(), output.str());
		}

		void printFunction(const std::string& name, const std::string& input_name, std::size_t const count) {
			std::ostringstream inputs;
			std::ostringstream output;
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
			addOutputs(name, inputs.str(), output.str());
		}

		void operator()(const instructions::StackPush& inst) {
			std::ostringstream name;
			std::ostringstream input;
			std::ostringstream output;
			name << "STACK_PUSH: " << inst.value;
			input << "#" << inst.value;
			output << ++stack_size_ - 1;
			addOutputs(name.str(), input.str(), output.str());
		}
		void operator()(const instructions::OPAdd& inst) {
			std::ostringstream name;
			name << "OP_ADD" << inst.count;
			printVariadicExpr(name.str(), "+", inst.count);
		}
		void operator()(const instructions::OPSubtract& inst) {
			(void) inst;
			printBinaryExpr("OP_SUBTRACT", "-");
		}
		void operator()(const instructions::OPUnaryMinus& inst) {
			(void) inst;
			printUnaryExpr("OP_UNARY_MINUS"s, "-");
		}
		void operator()(const instructions::OPMultiply& inst) {
			std::ostringstream name;
			name << "OP_MULTIPLY" << inst.count;
			printVariadicExpr(name.str(), "*", inst.count);
		}
		void operator()(const instructions::OPDivide& inst) {
			(void) inst;
			printBinaryExpr("OP_DIVIDE"s, "/");
		}
		void operator()(const instructions::OPEqual& inst) {
			(void) inst;
			printBinaryExpr("OP_EQUAL"s, "==");
		}
		void operator()(const instructions::OPUnequal& inst) {
			(void) inst;
			printBinaryExpr("OP_UNEQUAL"s, "!=");
		}
		void operator()(const instructions::OPLess& inst) {
			(void) inst;
			printBinaryExpr("OP_LESS"s, "<");
		}
		void operator()(const instructions::OPGreater& inst) {
			(void) inst;
			printBinaryExpr("OP_GREATER"s, ">");
		}
		void operator()(const instructions::OPLessEqual& inst) {
			(void) inst;
			printBinaryExpr("OP_LESS_EQUAL"s, "<=");
		}
		void operator()(const instructions::OPGreaterEqual& inst) {
			(void) inst;
			printBinaryExpr("OP_GREATER_EQUAL"s, ">=");
		}
		void operator()(const instructions::OPUnaryNot& inst) {
			(void) inst;
			printUnaryExpr("OP_UNARY_NOT"s, "!");
		}
		void operator()(const instructions::OPAnd& inst) {
			(void) inst;
			printBinaryExpr("OP_AND"s, "&");
		}
		void operator()(const instructions::OPOr& inst) {
			(void) inst;
			printBinaryExpr("OP_OR"s, "|");
		}
		void operator()(const instructions::OPXor& inst) {
			(void) inst;
			printBinaryExpr("OP_XOR"s, "^");
		}
		void operator()(const instructions::OPVariableLookup& inst) {
			std::ostringstream name;
			std::ostringstream output;
			auto&& var_name = get_name(inst.name);
			name << "OP_VARIABLE_LOOKUP: " << var_name;
			output << ++stack_size_ - 1;
			addOutputs(name.str(), var_name, output.str());
		}
		void operator()(const instructions::OPVariableIndexed& inst) {
			std::ostringstream name;
			std::ostringstream input;
			std::ostringstream output;
			auto&& var_name = get_name(inst.name);
			name << "OP_VARIABLE_INDEXED: " << var_name;
			input << var_name << "[" << stack_size_ - 1 << "]";
			output << stack_size_ - 1;
			addOutputs(name.str(), input.str(), output.str());
		}
		void operator()(const instructions::FuncReciprocal& inst) {
			(void) inst;
			printFunction("FUNC_RECIPROCAL"s, "Reciprocal", 1);
		}
		void operator()(const instructions::FuncPower& inst) {
			std::ostringstream name;
			name << "FUNC_POWER" << inst.count;
			printVariadicExpr(name.str(), "**", inst.count);
		}
		void operator()(const instructions::FuncQuotient& inst) {
			(void) inst;
			printBinaryExpr("FUNC_QUOTIENT"s, "//");
		}
		void operator()(const instructions::FuncMod& inst) {
			(void) inst;
			printBinaryExpr("FUNC_MOD"s, "%");
		}
		void operator()(const instructions::FuncMin& inst) {
			std::ostringstream name;
			name << "FUNC_MIN" << inst.count;
			printFunction(name.str(), "Min", inst.count);
		}
		void operator()(const instructions::FuncMax& inst) {
			std::ostringstream name;
			name << "FUNC_MAX" << inst.count;
			printFunction(name.str(), "Max", inst.count);
		}
		void operator()(const instructions::FuncAbs& inst) {
			(void) inst;
			printFunction("FUNC_ABS"s, "Abs", 1);
		}
		void operator()(const instructions::FuncSign& inst) {
			(void) inst;
			printFunction("FUNC_SIGN"s, "Sign", 1);
		}
		void operator()(const instructions::FuncFloor& inst) {
			(void) inst;
			printFunction("FUNC_FLOOR"s, "Floor", 1);
		}
		void operator()(const instructions::FuncCeiling& inst) {
			(void) inst;
			printFunction("FUNC_CEILING"s, "Ceiling", 1);
		}
		void operator()(const instructions::FuncRound& inst) {
			(void) inst;
			printFunction("FUNC_ROUND"s, "Round", 1);
		}
		void operator()(const instructions::FuncRandom& inst) {
			(void) inst;
			printFunction("FUNC_RANDOM"s, "Random", 2);
		}
		void operator()(const instructions::FuncRandomInt& inst) {
			(void) inst;
			printFunction("FUNC_RANDOMINT"s, "RandomInt", 2);
		}
		void operator()(const instructions::FuncExp& inst) {
			(void) inst;
			printFunction("FUNC_EXP"s, "Exp", 1);
		}
		void operator()(const instructions::FuncLog& inst) {
			(void) inst;
			printFunction("FUNC_LOG"s, "Log", 1);
		}
		void operator()(const instructions::FuncSqrt& inst) {
			(void) inst;
			printFunction("FUNC_SQRT"s, "Sqrt", 1);
		}
		void operator()(const instructions::FuncSin& inst) {
			(void) inst;
			printFunction("FUNC_SIN"s, "Sin", 1);
		}
		void operator()(const instructions::FuncCos& inst) {
			(void) inst;
			printFunction("FUNC_COS"s, "Cos", 1);
		}
		void operator()(const instructions::FuncTan& inst) {
			(void) inst;
			printFunction("FUNC_TAN"s, "Tan", 1);
		}
		void operator()(const instructions::FuncArctan& inst) {
			(void) inst;
			printFunction("FUNC_ARCTAN"s, "Arctan", 1);
		}
		void operator()(const instructions::FuncIf& inst) {
			(void) inst;
			printFunction("FUNC_IF"s, "If", 3);
		}
	};

	std::ostream& operator<<(std::ostream& os, const InstructionList& list) {
		FunctionScriptInstructionIOClass io_class(os);

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
			apply_visitor(io_class, inst);
		}

		io_class.print();

		return os;
	}
} // namespace bve::parsers::function_scripts
