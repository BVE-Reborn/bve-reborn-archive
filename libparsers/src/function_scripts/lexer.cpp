#include "function_scripts.hpp"
#include <cctype>
#include <cstdlib>

namespace parsers {
namespace function_scripts {
	namespace {
		static bool is_special_symbol(char c) {
			switch (c) {
				case '+':
				case '-':
				case '*':
				case '/':
				case '=':
				case '!':
				case '<':
				case '>':
				case '&':
				case '|':
				case '^':
				case '(':
				case ')':
				case '[':
				case ']':
				case ',':
					return true;
				default:
					return false;
			}
		}

		static bool is_number(char c) {
			return ('0' <= c && c <= '9');
		}

		static bool is_start_of_number(char c) {
			return (is_number(c) || c == '-' || c == '.');
		}

		static bool is_part_of_variable(char c) {
			return !is_special_symbol(c) && !std::isspace(c);
		}
	} // namespace

	lexer_token_list lex(const std::string& text, errors::errors_t& errors) {
		lexer_token_list ltl;

		for (std::size_t i = 0; i < text.size(); ++i) {
			lexer_token lt;

			bool has_another_character = i + 1 < text.size();

			// parsing number
			if (is_start_of_number(text[i])
			    && (has_another_character && text[i] == '-' ? is_number(text[i + 1]) : true)) {
				bool has_dash = text[i] == '-';
				bool has_dot = false;

				// find if the number has a . in it
				std::find_if(text.begin() + i + (has_dash ? 1 : 0), text.end(), [&has_dot](char c) {
					if (c == '.') {
						has_dot = true;
						return true;
					}
					return !is_number(c);
				});

				// parsing float
				if (has_dot) {
					const char* start_ptr = text.c_str() + i;
					char* str_end = nullptr;
					float f = std::strtof(start_ptr, &str_end);

					lt = lexer_types::floating{f};
					auto chars_used = std::distance(start_ptr, const_cast<const char*>(str_end - 1));
					i += chars_used;
				}
				// parsing int
				else {
					const char* start_ptr = text.c_str() + i;
					char* str_end = nullptr;
					auto integer = std::strtoll(start_ptr, &str_end, 10);

					lt = lexer_types::integer{integer};
					auto chars_used = std::distance(start_ptr, const_cast<const char*>(str_end - 1));
					i += chars_used;
				}
			}
			// parsing symbol
			else if (is_special_symbol(text[i])) {
				switch (text[i]) {
					case '+':
						lt = lexer_types::plus{};
						break;
					case '-':
						lt = lexer_types::minus{};
						break;
					case '*':
						lt = lexer_types::star{};
						break;
					case '/':
						lt = lexer_types::slash{};
						break;
					case '=':
						if (has_another_character && text[i + 1] == '=') {
							i += 1;
						}
						else {
							errors.emplace_back<errors::error_t>({0, "\"=\" must be \"==\""});
						}
						lt = lexer_types::double_eq{};
						break;
					case '!':
						if (has_another_character && text[i + 1] == '=') {
							lt = lexer_types::un_eq{};
							i += 1;
						}
						else {
							lt = lexer_types::bang{};
						}
						break;
					case '<':
						if (has_another_character && text[i + 1] == '=') {
							lt = lexer_types::less_eq{};
							i += 1;
						}
						else {
							lt = lexer_types::less{};
						}
						break;
					case '>':
						if (has_another_character && text[i + 1] == '=') {
							lt = lexer_types::greater_eq{};
							i += 1;
						}
						else {
							lt = lexer_types::greater{};
						}
						break;
					case '&':
						lt = lexer_types::ampersand{};
						break;
					case '|':
						lt = lexer_types::bar{};
						break;
					case '^':
						lt = lexer_types::carret{};
						break;
					case '(':
						lt = lexer_types::l_paren{};
						break;
					case ')':
						lt = lexer_types::r_paren{};
						break;
					case '[':
						lt = lexer_types::l_bracket{};
						break;
					case ']':
						lt = lexer_types::r_bracket{};
						break;
					case ',':
						lt = lexer_types::comma{};
						break;
					default:
						break;
				}
			}
			// parsing variable
			else if (!std::isspace(text[i])) {
				// Find end of variable
				std::size_t i2 = i;
				for (; i2 < text.size() && is_part_of_variable(text[i2]); ++i2)
					;

				std::string var{text.begin() + i, text.begin() + i2};

				lt = lexer_types::variable{std::move(var)};

				if (has_another_character) {
					i2 -= 1;
				}
				i = i2;
			}
			else {
				continue;
			}

			ltl.emplace_back(std::move(lt));
		}

		return ltl;
	}
} // namespace function_scripts
} // namespace parsers
