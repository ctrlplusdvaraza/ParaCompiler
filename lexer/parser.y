%require "3.2"
%language "c++"

%{

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast_node.hpp"


int yylex(void);
int yyerror(const char *s);
void count(void);
void comment(void);

AST *root = NULL;
%}

%union {
    int ival;
    char *sval;
    AST *node;
}

%code requires {
    #include "ast_node.hpp"
}

%token ELSE IF WHILE STDIN_GET_NUM PRINT


%token ADD_ASSIGN SUB_ASSIGN MUL_ASSIGN DIV_ASSIGN MOD_ASSIGN
%token LE_OP GE_OP EQ_OP NE_OP



/* nonterminals typed */
%type <node> translation_unit stmt_list stmt expression assignment_expression additive_expression multiplicative_expression primary_expression print_stmt compound_stmt if_stmt while_stmt expr_stmt
/* terminals typed */
%token <sval> IDENTIFIER STRING_LITERAL
%token <ival> CONSTANT 



/** grammar rule entry point */
%start translation_unit


/* WHAT ?????*/
/* operator precedence (so expressions parse correctly) */
%left ','            /* not used heavily but keep */
%left '='           /* assignment right-assoc; we'll handle explicitly */
%left '?' ':'       /* conditional (not used here) */
%left LE_OP GE_OP EQ_OP NE_OP '<' '>'
%left '+' '-'
%left '*' '/' '%'

%%

/* ------------------ grammar rules ------------------ */

translation_unit
    : stmt_list
    ;

stmt_list
    : /* empty */
    | stmt_list stmt
    ;

stmt
    : expr_stmt
    | compound_stmt
    | if_stmt
    | while_stmt
    | print_stmt
    ;

expr_stmt
    : ';' 
    | expression ';'
    ;

compound_stmt
    : '{' stmt_list '}'
    ;

if_stmt
    : IF '(' expression ')' stmt %prec '='
    | IF '(' expression ')' stmt ELSE stmt
    ;

while_stmt
    : WHILE '(' expression ')' stmt
    ;

print_stmt
    : PRINT expression ';'
    ;

/* Expression levels */

expression
    : assignment_expression
    ;



relational_expression
	: additive_expression
	| relational_expression '<' additive_expression
	| relational_expression '>' additive_expression
    ;

equality_expression
	: relational_expression
    ;

and_expression
	: equality_expression
    ;

exclusive_or_expression
	: and_expression
    ;

inclusive_or_expression
	: exclusive_or_expression
    ;

logical_and_expression
	: inclusive_or_expression
    ;

logical_or_expression
	: logical_and_expression
    ;

conditional_expression
	: logical_or_expression
    ;

assignment_expression
    : conditional_expression
    | IDENTIFIER assignment_operator assignment_expression {printf("var = x\n");}
    ;

assignment_operator
	: '='
	| MUL_ASSIGN
	| DIV_ASSIGN
	| MOD_ASSIGN
	| ADD_ASSIGN
	| SUB_ASSIGN
	;


additive_expression
    : multiplicative_expression
    | additive_expression '+' multiplicative_expression
    | additive_expression '-' multiplicative_expression
    ;

multiplicative_expression
    : primary_expression
    | multiplicative_expression '*' primary_expression
    | multiplicative_expression '/' primary_expression
    | multiplicative_expression '%' primary_expression
    ;

/* primary expression includes numbers, identifiers, input '?', parenthesized */
primary_expression
    : IDENTIFIER
    | CONSTANT
    | STDIN_GET_NUM
    | '(' expression ')'
    ;

%%

/* Bison will produce code that references yylval.* for tokens */
/* NOTE: your lexer must set yylval.ival for CONSTANT and yylval.sval for IDENTIFIER */

/* yyerror */
int yyerror(const char *s) {
    fprintf(stderr, "parse error: %s\n", s);
    return 0;
}

/* main: call yyparse (scanner should read stdin or file redirection) */
int main(void) {
    int res = yyparse();

    if (res == 0) {
        return 0;
    }
    return 1;
}
