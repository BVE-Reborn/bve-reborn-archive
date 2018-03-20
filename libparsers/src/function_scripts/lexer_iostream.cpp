#include "function_scripts.hpp"
#include <ostream>

namespace parsers {
namespace function_scripts {
	struct function_script_lexer_token_io_class {
		std::ostream& _os;
		explicit function_script_lexer_token_io_class(std::ostream& os) : _os(os){};

		void operator()(const lexer_types::plus& arg) {
			(void) arg;
			_os << "\"+\"";
		}

		void operator()(const lexer_types::minus& arg) {
			(void) arg;
			_os << "\"-\"";
		}

		void operator()(const lexer_types::star& arg) {
			(void) arg;
			_os << "\"*\"";
		}

		void operator()(const lexer_types::slash& arg) {
			(void) arg;
			_os << "\"/\"";
		}

		void operator()(const lexer_types::double_eq& arg) {
			(void) arg;
			_os << "\"==\"";
		}

		void operator()(const lexer_types::un_eq& arg) {
			(void) arg;
			_os << "\"!=\"";
		}

		void operator()(const lexer_types::less& arg) {
			(void) arg;
			_os << "\"<\"";
		}

		void operator()(const lexer_types::greater& arg) {
			(void) arg;
			_os << "\">\"";
		}

		void operator()(const lexer_types::less_eq& arg) {
			(void) arg;
			_os << "\"<=\"";
		}

		void operator()(const lexer_types::greater_eq& arg) {
			(void) arg;
			_os << "\">=\"";
		}

		void operator()(const lexer_types::bang& arg) {
			(void) arg;
			_os << "\"!\"";
		}

		void operator()(const lexer_types::ampersand& arg) {
			(void) arg;
			_os << "\"&\"";
		}

		void operator()(const lexer_types::bar& arg) {
			(void) arg;
			_os << "\"|\"";
		}

		void operator()(const lexer_types::carret& arg) {
			(void) arg;
			_os << "\"^\"";
		}

		void operator()(const lexer_types::l_paren& arg) {
			(void) arg;
			_os << "\"(\"";
		}

		void operator()(const lexer_types::r_paren& arg) {
			(void) arg;
			_os << "\")\"";
		}

		void operator()(const lexer_types::l_bracket& arg) {
			(void) arg;
			_os << "\"[\"";
		}

		void operator()(const lexer_types::r_bracket& arg) {
			(void) arg;
			_os << "\"]\"";
		}

		void operator()(const lexer_types::comma& arg) {
			(void) arg;
			_os << "\",\"";
		}

		void operator()(const lexer_types::dot& arg) {
			(void) arg;
			_os << "\".\"";
		}

		void operator()(const lexer_types::variable& arg) {
			_os << "(variable, \"" << arg.name << "\")";
		}

		void operator()(const lexer_types::integer& arg) {
			_os << "(int, \"" << arg.val << "\")";
		}

		void operator()(const lexer_types::floating& arg) {
			_os << "(float, \"" << arg.val << "\")";
		}
	};
} // namespace function_scripts
} // namespace parsers

std::ostream& operator<<(std::ostream& os, const parsers::function_scripts::lexer_token& lt) {
	parsers::function_scripts::function_script_lexer_token_io_class fsic{os};
	mapbox::util::apply_visitor(fsic, lt);
	return os;
}

std::ostream& operator<<(std::ostream& os,
                         const parsers::function_scripts::lexer_token_list& list) {
	for (auto& token : list) {
		os << token << '\n';
	}

	return os;
}
