/* --------------------- FLEX DEFINITIONS SECTION --------------------- */
%option header-file="scanner.hh"
%option noyywrap
%option nounput noinput batch debug

%{
#include <string>

#include "driver.hpp"
#include "parser.hpp"
%}

identifier [a-zA-Z][a-zA-Z_0-9]*
int_number [1-9][0-9]*
blank      [ \t\r]

%{
  # define YY_USER_ACTION location.columns(yyleng);
%}

%% /* --------------------- LEXICAL RULES SECTION --------------------- */

%{
  yy::location& location = driver.get_location();
  location.step();
%}

{identifier} { return yy::parser::make_IDENTIFIER(yytext, location); }
{int_number} { return yy::parser::make_LITERAL(yytext, location); }

"="          { return yy::parser::make_ASSIGN(location); }
";"          { return yy::parser::make_SEMICOLON(location); }

[ \t\v\f]             { location.step(); }
\n+                   { location.lines(yyleng); location.step(); }

.                     {
                        throw yy::parser::syntax_error(
                            location, "Unknown character: " + std::string(yytext)
                        );
                      }

<<EOF>>               { return yy::parser::make_YYEOF(location); }

%%

/* --------------------- USER CODE SECTION --------------------- */

void Driver::scan_begin()
{
    if(file_.empty() || file_ == "-")
    {
        yyin = stdin;
    }
    else if(!(yyin = fopen(file_.c_str(), "r")))
    {
        std::cerr << "Cannot open " << file_ << ": " << strerror(errno) << '\n';
        exit(EXIT_FAILURE);
    }
}

void Driver::scan_end()
{
    fclose (yyin);
}
