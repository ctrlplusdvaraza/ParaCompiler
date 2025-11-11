%option noyywrap
%{
#include "parser.tab.hh"
#include "driver.hh"
#include <cstdlib>
#include <string>

#undef YY_DECL
#define YY_DECL yy::parser::symbol_type yylex (driver& drv)
%}

DIGIT   [0-9]+
ID      [A-Za-z_][A-Za-z0-9_]*

%%

{DIGIT}     {
               int v = std::atoi(yytext);
               return yy::parser::make_NUMBER(v, drv.location);
            }
":="        { return yy::parser::make_ASSIGN(drv.location); }
"+"         { return yy::parser::make_PLUS(drv.location); }
"-"         { return yy::parser::make_MINUS(drv.location); }
"*"         { return yy::parser::make_STAR(drv.location); }
"/"         { return yy::parser::make_SLASH(drv.location); }
"("         { return yy::parser::make_LPAREN(drv.location); }
")"         { return yy::parser::make_RPAREN(drv.location); }
{ID}        { return yy::parser::make_IDENTIFIER(std::string(yytext), drv.location); }
[ \t\r]+    { /* skip whitespace */ }
\n          { /* skip newline */ }
.           { /* ignore other chars */ }

<<EOF>>     { yy::parser::make_YYEOF(drv.location); }   /* EOF -> empty/default symbol */
%%
