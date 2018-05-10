#include "function_scripts.hpp"
#include <doctest.h>
#include <sstream>

using namespace std::string_literals;
namespace fs_lex_token = parsers::function_scripts::lexer_types;

TEST_SUITE_BEGIN("libparsers - function scripts");

TEST_CASE("libparsers - function scripts - lexer iostream - all symbols") {
	parsers::function_scripts::lexer_token_list const token_list = {{
	    fs_lex_token::plus{},           //
	    fs_lex_token::minus{},          //
	    fs_lex_token::star{},           //
	    fs_lex_token::slash{},          //
	    fs_lex_token::double_eq{},      //
	    fs_lex_token::un_eq{},          //
	    fs_lex_token::less{},           //
	    fs_lex_token::greater{},        //
	    fs_lex_token::less_eq{},        //
	    fs_lex_token::greater_eq{},     //
	    fs_lex_token::bang{},           //
	    fs_lex_token::ampersand{},      //
	    fs_lex_token::bar{},            //
	    fs_lex_token::carret{},         //
	    fs_lex_token::l_paren{},        //
	    fs_lex_token::r_paren{},        //
	    fs_lex_token::l_bracket{},      //
	    fs_lex_token::r_bracket{},      //
	    fs_lex_token::comma{},          //
	    fs_lex_token::dot{},            //
	    fs_lex_token::variable{"sin"s}, //
	    fs_lex_token::integer{2},       //
	    fs_lex_token::floating{2.2f}    //
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
