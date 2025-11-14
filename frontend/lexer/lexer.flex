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
int_number [1-9][0-9]*

blank      [ \t\r]+
new_line   [\n]+

%% /*------------------------------------- Lexical rules section -------------------------------------*/

%{
    yy::location location;
%}

{identifier} { return yy::parser::make_IDENTIFIER(yytext, location); }
{int_number} { return yy::parser::make_LITERAL(yytext, location); }

"="          { return yy::parser::make_ASSIGN(location); }
";"          { return yy::parser::make_SEMICOLON(location); }


{blank}    { location.step(); }
{new_line} { location.lines(yyleng); location.step(); }
.          { throw yy::parser::syntax_error(location, "Unknown character: " + std::string(yytext)); }
<<EOF>>    { return yy::parser::make_YYEOF(location); }

%% /*----------------------------------------- Code section ------------------------------------------*/

int Driver::input_file_initialize(const std::string& file_path)
{
    if(file_path.empty())
    {
        std::cerr << "No file provided"<< std::endl;
        return 1;
    }

    yyin = fopen(file_path.c_str(), "r");

    if(yyin == NULL)
    {
        std::cerr << "Cannot open " << file_path << ": " << strerror(errno) << std::endl;
        return 1;
    }

    return 0;
}

void Driver::input_file_close()
{
    fclose(yyin);
}
