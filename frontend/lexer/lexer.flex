/* --------------------- FLEX DEFINITIONS SECTION --------------------- */
%option header-file="scanner.hh"
%option noyywrap
%option nounput noinput batch debug

%{
#include <string>

#include "driver.hpp"
#include "parser.hpp"
%}

id    [a-zA-Z][a-zA-Z_0-9]*
int   [0-9]+
blank [ \t\r]

%{
  // Code run each time a pattern is matched.
  # define YY_USER_ACTION  loc.columns (yyleng);
%}

%% /* --------------------- LEXICAL RULES SECTION --------------------- */

%{
  // A handy shortcut to the location held by the driver.
  yy::location& loc = drv.location;
  // Code run each time yylex is called.
  loc.step();
%}

identifier { return yy::parser::make_IDENTIFIER(yytext, location); }
int_number { return yy::parser::make_CONSTANT(std::stoi(yytext), location); }

"="        { return yy::parser::make_ASSIGN(location); }

[ \t\v\f]             { loc.step(); }
\n+                   { loc.lines(yyleng); loc.step(); }

.                     {
                        throw yy::parser::syntax_error(
                            loc, "Unknown character: " + std::string(yytext)
                        );
                      }

<<EOF>>               { return yy::parser::make_YYEOF(loc); }

%%

/* --------------------- USER CODE SECTION --------------------- */

void driver::scan_begin ()
{
    yy_flex_debug = is_trace_lexing();

    if(file.empty() || file == "-")
    {
        yyin = stdin;
    }
    else if(!(yyin = fopen(file.c_str(), "r")))
    {
        std::cerr << "Cannot open " << file << ": " << strerror(errno) << '\n';
        exit(EXIT_FAILURE);
    }
}

void driver::scan_end ()
{
  fclose (yyin);
}
