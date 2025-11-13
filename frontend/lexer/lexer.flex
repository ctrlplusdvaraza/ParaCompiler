/* --------------------- FLEX DEFINITIONS SECTION --------------------- */
%option header-file="scanner.hh"
%option noyywrap
%option nounput noinput batch debug

%{ /* -*- C++ -*- */
# include <cerrno>
# include <climits>
# include <cstdlib>
# include <cstring> // strerror
# include <string>
# include "driver.hpp"
# include "parser.hpp"
%}

id    [a-zA-Z][a-zA-Z_0-9]*
int   [0-9]+
blank [ \t\r]

%{
  // Code run each time a pattern is matched.
  # define YY_USER_ACTION  loc.columns (yyleng);
%}

/* --------------------- LEXICAL RULES SECTION --------------------- */
%%

%{
  // A handy shortcut to the location held by the driver.
  yy::location& loc = drv.location;
  // Code run each time yylex is called.
  loc.step();
%}

"else"                { return yy::parser::make_ELSE(loc); }
"if"                  { return yy::parser::make_IF(loc); }
"while"               { return yy::parser::make_WHILE(loc); }
"?"                   { return yy::parser::make_STDIN_GET_NUM(loc); }
"print"               { return yy::parser::make_PRINT(loc); }

[a-zA-Z][a-zA-Z0-9]*  { return yy::parser::make_IDENTIFIER(yytext, loc); }
[0-9]+                { return yy::parser::make_CONSTANT(std::stoi(yytext), loc); }

"+="                  { return yy::parser::make_ADD_ASSIGN(loc); }
"-="                  { return yy::parser::make_SUB_ASSIGN(loc); }
"*="                  { return yy::parser::make_MUL_ASSIGN(loc); }
"/="                  { return yy::parser::make_DIV_ASSIGN(loc); }
"%="                  { return yy::parser::make_MOD_ASSIGN(loc); }

"<="                  { return yy::parser::make_LE_OP(loc); }
">="                  { return yy::parser::make_GE_OP(loc); }
"=="                  { return yy::parser::make_EQ_OP(loc); }
"!="                  { return yy::parser::make_NE_OP(loc); }

"<"                   { return yy::parser::make_LESS(loc); }
">"                   { return yy::parser::make_GREATER(loc); }
","                   { return yy::parser::make_COMMA(loc); }
";"                   { return yy::parser::make_SEMICOLON(loc); }

("{"|"<%")            { return yy::parser::make_LBRACE(loc); }
("}"|"%>")            { return yy::parser::make_RBRACE(loc); }

"="                   { return yy::parser::make_ASSIGN(loc); }
"("                   { return yy::parser::make_LPAREN(loc); }
")"                   { return yy::parser::make_RPAREN(loc); }

"-"                   { return yy::parser::make_MINUS(loc); }
"+"                   { return yy::parser::make_PLUS(loc); }
"*"                   { return yy::parser::make_STAR(loc); }
"/"                   { return yy::parser::make_SLASH(loc); }
"%"                   { return yy::parser::make_PERCENT(loc); }

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
  yy_flex_debug = trace_scanning;
  if (file.empty () || file == "-")
    yyin = stdin;
  else if (!(yyin = fopen (file.c_str (), "r")))
    {
      std::cerr << "cannot open " << file << ": " << strerror (errno) << '\n';
      exit (EXIT_FAILURE);
    }
}

void driver::scan_end ()
{
  fclose (yyin);
}
