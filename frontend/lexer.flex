/*------------------------------------- Flex definitions section -------------------------------------*/

%option noyywrap
%option nounput noinput

%{
    #include <string>

    #include "driver.hpp"
    #include "parser.hpp"

    #define YY_DECL yy::parser::symbol_type yylex(Driver& driver)
    YY_DECL;

    #define YY_USER_ACTION location.columns(yyleng);
%}

identifier [a-zA-Z][a-zA-Z_0-9]*
number     [0-9]+

blank      [ \t\r]+
new_line   [\n]+

line_comment    "//".*
block_comment   "/*"([^*]|"*"[^/])*"*/"

%% /*------------------------------------- Lexical rules section -------------------------------------*/

%{
    yy::location& location = driver.location;
%}

{number}     { return yy::parser::make_LITERAL(yytext, location); }

"if"         { return yy::parser::make_IF(yytext, location);    }
"else"       { return yy::parser::make_ELSE(yytext, location);  }
"while"      { return yy::parser::make_WHILE(yytext, location); }
"print"      { return yy::parser::make_PRINT(yytext, location); }
"?"          { return yy::parser::make_INPUT(yytext, location); }

{identifier} { return yy::parser::make_IDENTIFIER(yytext, location); }

";"          { return yy::parser::make_SEMICOLON(yytext, location); }

"("          { return yy::parser::make_L_ROUND_BR(yytext, location); }
")"          { return yy::parser::make_R_ROUND_BR(yytext, location); }
"{"          { return yy::parser::make_L_CURLY_BR(yytext, location); }
"}"          { return yy::parser::make_R_CURLY_BR(yytext, location); }

"+="         { return yy::parser::make_ADD_ASSIGN(yytext, location); }
"-="         { return yy::parser::make_SUB_ASSIGN(yytext, location); }
"*="         { return yy::parser::make_MUL_ASSIGN(yytext, location); }
"/="         { return yy::parser::make_DIV_ASSIGN(yytext, location); }
"%="         { return yy::parser::make_MOD_ASSIGN(yytext, location); }
"="          { return yy::parser::make_ASSIGN(yytext, location); }

"++"         { return yy::parser::make_PLUSPLUS(yytext, location); }
"--"         { return yy::parser::make_MINUSMINUS(yytext, location); }
"+"          { return yy::parser::make_PLUS(yytext, location); }
"-"          { return yy::parser::make_MINUS(yytext, location); }
"*"          { return yy::parser::make_STAR(yytext, location); }
"/"          { return yy::parser::make_SLASH(yytext, location); }
"%"          { return yy::parser::make_PERCENT(yytext, location); }

"=="         { return yy::parser::make_EQ_CMP(yytext, location); }
"!="         { return yy::parser::make_NE_CMP(yytext, location); }
"<="         { return yy::parser::make_LE_CMP(yytext, location); }
"<"          { return yy::parser::make_L_CMP(yytext, location); }
">="         { return yy::parser::make_GE_CMP(yytext, location); }
">"          { return yy::parser::make_G_CMP(yytext, location); }

"!"          { return yy::parser::make_NOT_LOGICAL(yytext, location); }
"&&"         { return yy::parser::make_AND_LOGICAL(yytext, location); }
"||"         { return yy::parser::make_OR_LOGICAL(yytext, location); }

{line_comment} { location.step(); }

{block_comment} { 
    for (int i = 0; i < yyleng; ++i) {
        if (yytext[i] == '\n') {
            location.lines(1);
        }
    }
    location.step(); 
}

{blank}      { location.step(); }

{new_line}   { location.lines(yyleng); location.step(); }

.            { throw yy::parser::syntax_error(location, "unknown character: " + std::string(yytext)); }

<<EOF>>      { return yy::parser::make_YYEOF(location); }

%%
