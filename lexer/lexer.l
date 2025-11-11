%option c++
%option noyywrap

%{
#include <iostream>
#include "parser.hpp"

int mylineno = 0;
void count(); // keep count function

// Token type enum
enum class TokenType {
    // Keywords
    ELSE,
    IF,
    WHILE,
    PRINT,
    STDIN_GET_NUM,

    // Identifiers and literals
    IDENTIFIER,
    CONSTANT,
    STRING_LITERAL,

    // Assignment operators
    ADD_ASSIGN,
    SUB_ASSIGN,
    MUL_ASSIGN,
    DIV_ASSIGN,
    MOD_ASSIGN,

    // Comparison operators
    LE_OP,
    GE_OP,
    EQ_OP,
    NE_OP,

    // Single-character symbols
    LESS,
    GREATER,
    COMMA,
    SEMICOLON,
    LBRACE,
    RBRACE,
    ASSIGN,
    LPAREN,
    RPAREN,
    MINUS,
    PLUS,
    MUL,
    DIV,
    MOD
};

%}

D       [0-9]
L       [a-zA-Z_]

%%

"/*"           	{ 
				int c;

				while((c = yyinput()) != 0)
					{
					if(c == '\n')
						++mylineno;

					else if(c == '*')
						{
						if((c = yyinput()) == '/')
							break;
						else
							unput(c);
						}
					}
				}

"else"          { count(); return (int)TokenType::ELSE; }
"if"            { count(); return (int)TokenType::IF; }
"while"         { count(); return (int)TokenType::WHILE; }
"?"             { count(); return (int)TokenType::STDIN_GET_NUM; }
"print"         { count(); return (int)TokenType::PRINT; }

{L}({L}|{D})*   { count(); return (int)TokenType::IDENTIFIER; }
{D}+            { count(); return (int)TokenType::CONSTANT; }
\"(\\.|[^\\"])*\" { count(); return (int)TokenType::STRING_LITERAL; }

"+="            { count(); return (int)TokenType::ADD_ASSIGN; }
"-="            { count(); return (int)TokenType::SUB_ASSIGN; }
"*="            { count(); return (int)TokenType::MUL_ASSIGN; }
"/="            { count(); return (int)TokenType::DIV_ASSIGN; }
"%="            { count(); return (int)TokenType::MOD_ASSIGN; }

"<="            { count(); return (int)TokenType::LE_OP; }
">="            { count(); return (int)TokenType::GE_OP; }
"=="            { count(); return (int)TokenType::EQ_OP; }
"!="            { count(); return (int)TokenType::NE_OP; }

"<"             { count(); return (int)TokenType::LESS; }
">"             { count(); return (int)TokenType::GREATER; }

","             { count(); return (int)TokenType::COMMA; }
";"             { count(); return (int)TokenType::SEMICOLON; }

("{"|"<%")      { count(); return (int)TokenType::LBRACE; }
("}"|"%>")      { count(); return (int)TokenType::RBRACE; }

"="             { count(); return (int)TokenType::ASSIGN; }
"("             { count(); return (int)TokenType::LPAREN; }
")"             { count(); return (int)TokenType::RPAREN; }

"-"             { count(); return (int)TokenType::MINUS; }
"+"             { count(); return (int)TokenType::PLUS; }
"*"             { count(); return (int)TokenType::MUL; }
"/"             { count(); return (int)TokenType::DIV; }
"%"             { count(); return (int)TokenType::MOD; }

[ \t\v\n\f]     { count(); }
.               { std::cerr << "Unknown character: " << yytext << std::endl; std::exit(1); }

%%


int column = 0;

void count(yyFlexLexer* scanner)
{
    for (int i = 0; scanner->YYText()[i] != '\0'; i++)
        if (scanner->YYText()[i] == '\n')
            column = 0;
        else if (scanner->YYText()[i] == '\t')
            column += 8 - (column % 8);
        else
            column++;
}
