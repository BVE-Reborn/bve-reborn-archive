#include "function_scripts.hpp"
#include <ostream>

namespace parsers {
namespace function_scripts {
	struct function_script_lexer_token_io_class {
	  private:
		std::ostream& os_;

	  public:
		explicit function_script_lexer_token_io_class(std::ostream& os) : os_(os) {}

		void operator()(const lexer_types::plus& arg) const {
			(void) arg;
			os_ << "\"+\"";
		}

		void operator()(const lexer_types::minus& arg) const {
			(void) arg;
			os_ << "\"-\"";
		}

		void operator()(const lexer_types::star& arg) const {
			(void) arg;
			os_ << "\"*\"";
		}

		void operator()(const lexer_types::slash& arg) const {
			(void) arg;
			os_ << "\"/\"";
		}

		void operator()(const lexer_types::double_eq& arg) const {
			(void) arg;
			os_ << "\"==\"";
		}

		void operator()(const lexer_types::un_eq& arg) const {
			(void) arg;
			os_ << "\"!=\"";
		}

		void operator()(const lexer_types::less& arg) const {
			(void) arg;
			os_ << "\"<\"";
		}

		void operator()(const lexer_types::greater& arg) const {
			(void) arg;
			os_ << "\">\"";
		}

		void operator()(const lexer_types::less_eq& arg) const {
			(void) arg;
			os_ << "\"<=\"";
		}

		void operator()(const lexer_types::greater_eq& arg) const {
			(void) arg;
			os_ << "\">=\"";
		}

		void operator()(const lexer_types::bang& arg) const {
			(void) arg;
			os_ << "\"!\"";
		}

		void operator()(const lexer_types::ampersand& arg) const {
			(void) arg;
			os_ << "\"&\"";
		}

		void operator()(const lexer_types::bar& arg) const {
			(void) arg;
			os_ << "\"|\"";
		}

		void operator()(const lexer_types::carret& arg) const {
			(void) arg;
			os_ << "\"^\"";
		}

		void operator()(const lexer_types::l_paren& arg) const {
			(void) arg;
			os_ << "\"(\"";
		}

		void operator()(const lexer_types::r_paren& arg) const {
			(void) arg;
			os_ << "\")\"";
		}

		void operator()(const lexer_types::l_bracket& arg) const {
			(void) arg;
			os_ << "\"[\"";
		}

		void operator()(const lexer_types::r_bracket& arg) const {
			(void) arg;
			os_ << "\"]\"";
		}

		void operator()(const lexer_types::comma& arg) const {
			(void) arg;
			os_ << "\",\"";
		}

		void operator()(const lexer_types::dot& arg) const {
			(void) arg;
			os_ << "\".\"";
		}

		void operator()(const lexer_types::variable& arg) const {
			os_ << "(variable, \"" << arg.name << "\")";
		}

		void operator()(const lexer_types::integer& arg) const {
			os_ << "(int, \"" << arg.val << "\")";
		}

		void operator()(const lexer_types::floating& arg) const {
			os_ << "(float, \"" << arg.val << "\")";
		}
	};
} // namespace function_scripts
} // namespace parsers

std::ostream& operator<<(std::ostream& os, const parsers::function_scripts::lexer_token& lt) {
	parsers::function_scripts::function_script_lexer_token_io_class fsic{os};
	apply_visitor(fsic, lt);
	return os;
}

std::ostream& operator<<(std::ostream& os,
                         const parsers::function_scripts::lexer_token_list& list) {
	for (auto& token : list) {
		os << token << '\n';
	}

	return os;
}
