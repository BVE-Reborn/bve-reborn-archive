#include "parse_tree.hpp"
#include <ostream>

namespace bve::parsers::function_scripts {
	struct FunctionScriptLexerTokenIoClass {
	  private:
		std::ostream& os_;

	  public:
		explicit FunctionScriptLexerTokenIoClass(std::ostream& os) : os_(os) {}

		void operator()(const lexer_types::Plus& arg) const {
			(void) arg;
			os_ << "\"+\"";
		}

		void operator()(const lexer_types::Minus& arg) const {
			(void) arg;
			os_ << "\"-\"";
		}

		void operator()(const lexer_types::Star& arg) const {
			(void) arg;
			os_ << "\"*\"";
		}

		void operator()(const lexer_types::Slash& arg) const {
			(void) arg;
			os_ << "\"/\"";
		}

		void operator()(const lexer_types::DoubleEq& arg) const {
			(void) arg;
			os_ << "\"==\"";
		}

		void operator()(const lexer_types::UnEq& arg) const {
			(void) arg;
			os_ << "\"!=\"";
		}

		void operator()(const lexer_types::Less& arg) const {
			(void) arg;
			os_ << "\"<\"";
		}

		void operator()(const lexer_types::Greater& arg) const {
			(void) arg;
			os_ << "\">\"";
		}

		void operator()(const lexer_types::LessEq& arg) const {
			(void) arg;
			os_ << "\"<=\"";
		}

		void operator()(const lexer_types::GreaterEq& arg) const {
			(void) arg;
			os_ << "\">=\"";
		}

		void operator()(const lexer_types::Bang& arg) const {
			(void) arg;
			os_ << "\"!\"";
		}

		void operator()(const lexer_types::Ampersand& arg) const {
			(void) arg;
			os_ << "\"&\"";
		}

		void operator()(const lexer_types::Bar& arg) const {
			(void) arg;
			os_ << "\"|\"";
		}

		void operator()(const lexer_types::Caret& arg) const {
			(void) arg;
			os_ << "\"^\"";
		}

		void operator()(const lexer_types::LParen& arg) const {
			(void) arg;
			os_ << "\"(\"";
		}

		void operator()(const lexer_types::RParen& arg) const {
			(void) arg;
			os_ << "\")\"";
		}

		void operator()(const lexer_types::LBracket& arg) const {
			(void) arg;
			os_ << "\"[\"";
		}

		void operator()(const lexer_types::RBracket& arg) const {
			(void) arg;
			os_ << "\"]\"";
		}

		void operator()(const lexer_types::Comma& arg) const {
			(void) arg;
			os_ << "\",\"";
		}

		void operator()(const lexer_types::Dot& arg) const {
			(void) arg;
			os_ << "\".\"";
		}

		void operator()(const lexer_types::Variable& arg) const {
			os_ << "(variable, \"" << arg.name << "\")";
		}

		void operator()(const lexer_types::Integer& arg) const {
			os_ << "(int, \"" << arg.val << "\")";
		}

		void operator()(const lexer_types::Floating& arg) const {
			os_ << "(float, \"" << arg.val << "\")";
		}
	};
} // namespace bve::parsers::function_scripts

std::ostream& operator<<(std::ostream& os, const bve::parsers::function_scripts::LexerToken& lt) {
	bve::parsers::function_scripts::FunctionScriptLexerTokenIoClass io_class{os};
	apply_visitor(io_class, lt);
	return os;
}

std::ostream& operator<<(std::ostream& os,
                         const bve::parsers::function_scripts::LexerTokenList& list) {
	for (auto& token : list) {
		os << token << '\n';
	}

	return os;
}
