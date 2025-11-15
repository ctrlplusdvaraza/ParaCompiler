/*------------------------------------- Flex definitions section -------------------------------------*/

%option header-file="lexer.hpp"
%option noyywrap
%option nounput noinput batch debug

%{
    #include <string>

    #include "driver.hpp"
    #include "parser.hpp"

    #define YY_USER_ACTION location.columns(yyleng);
%}

identifier [a-zA-Z][a-zA-Z_0-9]*
number     [1-9][0-9]*

blank      [ \t\r]+
new_line   [\n]+

%% /*------------------------------------- Lexical rules section -------------------------------------*/

%{
    yy::location location;
%}

{identifier} { return yy::parser::make_IDENTIFIER(yytext, location); }
{number}     { return yy::parser::make_LITERAL(yytext, location); }

";"          { return yy::parser::make_SEMICOLON(yytext, location); }
"("          { return yy::parser::make_L_ROUND_BR(yytext, location); }
")"          { return yy::parser::make_R_ROUND_BR(yytext, location); }

"="          { return yy::parser::make_ASSIGN(yytext, location); }
"+="         { return yy::parser::make_ADD_ASSIGN(yytext, location); }
"-="         { return yy::parser::make_SUB_ASSIGN(yytext, location); }
"*="         { return yy::parser::make_MUL_ASSIGN(yytext, location); }
"/="         { return yy::parser::make_DIV_ASSIGN(yytext, location); }
"%="         { return yy::parser::make_MOD_ASSIGN(yytext, location); }

"+"          { return yy::parser::make_PLUS(yytext, location); }
"-"          { return yy::parser::make_MINUS(yytext, location); }
"*"          { return yy::parser::make_STAR(yytext, location); }
"/"          { return yy::parser::make_SLASH(yytext, location); }
"%"          { return yy::parser::make_PERCENT(yytext, location); }
"++"         { return yy::parser::make_PLUSPLUS(yytext, location); }
"--"         { return yy::parser::make_MINUSMINUS(yytext, location); }

{blank}      { location.step(); }

{new_line}   { location.lines(yyleng); location.step(); }

.            { throw yy::parser::syntax_error(location, "Unknown character: " + std::string(yytext)); }

<<EOF>>      { return yy::parser::make_YYEOF(location); }

%% /*----------------------------------------- Code section ------------------------------------------*/

/* to be continued (maybe:)) */
