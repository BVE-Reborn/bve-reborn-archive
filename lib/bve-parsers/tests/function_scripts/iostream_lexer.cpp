#include "function_scripts/lexer.hpp"
#include "function_scripts/parse_tree.hpp"
#include <doctest.h>
#include <sstream>

using namespace std::string_literals;
namespace fs_lex_token = bve::parsers::function_scripts::lexer_types;

TEST_SUITE_BEGIN("libparsers - function scripts");

TEST_CASE("libparsers - function scripts - lexer iostream - all symbols") {
	bve::parsers::function_scripts::LexerTokenList const token_list = {{
	    fs_lex_token::Plus{},           //
	    fs_lex_token::Minus{},          //
	    fs_lex_token::Star{},           //
	    fs_lex_token::Slash{},          //
	    fs_lex_token::DoubleEq{},       //
	    fs_lex_token::UnEq{},           //
	    fs_lex_token::Less{},           //
	    fs_lex_token::Greater{},        //
	    fs_lex_token::LessEq{},         //
	    fs_lex_token::GreaterEq{},      //
	    fs_lex_token::Bang{},           //
	    fs_lex_token::Ampersand{},      //
	    fs_lex_token::Bar{},            //
	    fs_lex_token::Caret{},          //
	    fs_lex_token::LParen{},         //
	    fs_lex_token::RParen{},         //
	    fs_lex_token::LBracket{},       //
	    fs_lex_token::RBracket{},       //
	    fs_lex_token::Comma{},          //
	    fs_lex_token::Dot{},            //
	    fs_lex_token::Variable{"sin"s}, //
	    fs_lex_token::Integer{2},       //
	    fs_lex_token::Floating{2.2F}    //
	}};

	std::ostringstream output;

	output << token_list;

	CHECK_EQ(output.str(),
	         "\"+\"\n"
	         "\"-\"\n"
	         "\"*\"\n"
	         "\"/\"\n"
	         "\"==\"\n"
	         "\"!=\"\n"
	         "\"<\"\n"
	         "\">\"\n"
	         "\"<=\"\n"
	         "\">=\"\n"
	         "\"!\"\n"
	         "\"&\"\n"
	         "\"|\"\n"
	         "\"^\"\n"
	         "\"(\"\n"
	         "\")\"\n"
	         "\"[\"\n"
	         "\"]\"\n"
	         "\",\"\n"
	         "\".\"\n"
	         "(variable, \"sin\")\n"
	         "(int, \"2\")\n"
	         "(float, \"2.2\")\n");
}

TEST_SUITE_END();
