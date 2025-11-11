/* parser.y - minimal grammar + AST + interpreter for your language */

%{
/* C prologue */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast_node.h"


int yylex(void);
int yyerror(const char *s);
void count(void);
void comment(void);

/* root */
AST *root = NULL;

%}

/* semantic value union */
%union {
    int ival;
    char *sval;
    AST *node;
}

%code requires {
#include "ast_node.h"
}



/* token declarations (must match lexer token names) */
%token ELSE IF WHILE STDIN_GET_NUM PRINT

%token <sval> IDENTIFIER STRING_LITERAL
%token <ival> CONSTANT 

%token ADD_ASSIGN SUB_ASSIGN MUL_ASSIGN DIV_ASSIGN MOD_ASSIGN
%token LE_OP GE_OP EQ_OP NE_OP

/* single-char tokens are returned as character literal tokens: ';' ',' '=' '+' '-' ... */

/* nonterminals typed */
%type <node> translation_unit stmt_list stmt expression assignment_expression additive_expression multiplicative_expression primary_expression print_stmt compound_stmt if_stmt while_stmt expr_stmt

%start translation_unit

/* operator precedence (so expressions parse correctly) */
%left ','            /* not used heavily but keep */
%left '='           /* assignment right-assoc; we'll handle explicitly */
%left '?' ':'       /* conditional (not used here) */
%left LE_OP GE_OP EQ_OP NE_OP '<' '>'
%left '+' '-'
%left '*' '/' '%'
%%

/* ------------------ grammar rules & actions ------------------ */

translation_unit
    : stmt_list            { root = $1; }
    ;

stmt_list
    : /* empty */          { $$ = NULL; }
    | stmt_list stmt       { $$ = $1 ? new_seq($1, $2) : $2; }
    ;

stmt
    : expr_stmt            { $$ = $1; }
    | compound_stmt        { $$ = $1; }
    | if_stmt              { $$ = $1; }
    | while_stmt           { $$ = $1; }
    | print_stmt           { $$ = $1; }
    ;

expr_stmt
    : ';'                  { $$ = NULL; } 
    | expression ';'       { $$ = new_expr_stmt($1); }
    ;

compound_stmt
    : '{' stmt_list '}'    { $$ = $2; }   /* block yields sequence as a single node (linked seq) */
    ;

if_stmt
    : IF '(' expression ')' stmt %prec '='    { $$ = new_if($3, $5, NULL); }
    | IF '(' expression ')' stmt ELSE stmt    { $$ = new_if($3, $5, $7); }
    ;

while_stmt
    : WHILE '(' expression ')' stmt            { $$ = new_while($3, $5); }
    ;

print_stmt
    : PRINT expression ';'                     { $$ = new_print($2); }
    ;

/* Expression levels */

expression
    : assignment_expression                    { $$ = $1; }
    ;

assignment_expression
    : IDENTIFIER '=' assignment_expression     { $$ = new_assign($1, $3); /* lexer must have strdup'd $1 */ }
    | IDENTIFIER ADD_ASSIGN assignment_expression { /* a += b  -> a = a + b */
            AST *lhs = new_id($1); $$ = new_assign($1, new_binop('+', lhs, $3)); }
    | IDENTIFIER SUB_ASSIGN assignment_expression {
            AST *lhs = new_id($1); $$ = new_assign($1, new_binop('-', lhs, $3)); }
    | IDENTIFIER MUL_ASSIGN assignment_expression {
            AST *lhs = new_id($1); $$ = new_assign($1, new_binop('*', lhs, $3)); }
    | IDENTIFIER DIV_ASSIGN assignment_expression {
            AST *lhs = new_id($1); $$ = new_assign($1, new_binop('/', lhs, $3)); }
    | IDENTIFIER MOD_ASSIGN assignment_expression {
            AST *lhs = new_id($1); $$ = new_assign($1, new_binop('%', lhs, $3)); }
    | additive_expression                        { $$ = $1; }
    ;

additive_expression
    : multiplicative_expression                 { $$ = $1; }
    | additive_expression '+' multiplicative_expression { $$ = new_binop('+',$1,$3); }
    | additive_expression '-' multiplicative_expression { $$ = new_binop('-',$1,$3); }
    ;

multiplicative_expression
    : primary_expression                        { $$ = $1; }
    | multiplicative_expression '*' primary_expression { $$ = new_binop('*',$1,$3); }
    | multiplicative_expression '/' primary_expression { $$ = new_binop('/',$1,$3); }
    | multiplicative_expression '%' primary_expression { $$ = new_binop('%',$1,$3); }
    ;

/* primary expression includes numbers, identifiers, input '?', parenthesized */
primary_expression
    : IDENTIFIER                                { $$ = new_id($1); }
    | CONSTANT                                  { $$ = new_num($1); }
    | STDIN_GET_NUM                             { $$ = new_input(); }
    | '(' expression ')'                        { $$ = $2; }
    ;

/* Note: comparisons are expressed as binary ops using LE_OP etc.
   The lexer returns LE_OP, GE_OP, EQ_OP, NE_OP tokens; treat them here. */




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
    if (yyparse() == 0) {
        return 0;
    }
    return 1;
}
