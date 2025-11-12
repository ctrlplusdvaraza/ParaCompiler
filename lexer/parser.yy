%skeleton "lalr1.cc" // -*- C++ -*-
%require "3.8.1"
%header "parser.hh"


%define api.token.constructor
%define api.value.type variant
%define parse.assert

%code requires {
  # include <string>
  class driver;
}

// The parsing context.
%param { driver& drv }

%locations

%define parse.trace
%define parse.error detailed
%define parse.lac full

%code {
# include "driver.hh"
}

%define api.token.prefix {TOK_}

%token
  ELSE         "else"
  IF           "if"
  WHILE        "while"
  STDIN_GET_NUM "?"
  PRINT        "print"


  ADD_ASSIGN   "+="
  SUB_ASSIGN   "-="
  MUL_ASSIGN   "*="
  DIV_ASSIGN   "/="
  MOD_ASSIGN   "%="

  LE_OP        "<="
  GE_OP        ">="
  EQ_OP        "=="
  NE_OP        "!="

  LESS         "<"
  GREATER      ">"
  COMMA        ","
  SEMICOLON    ";"
  LBRACE       "{"
  RBRACE       "}"
  ASSIGN       "="
  LPAREN       "("
  RPAREN       ")"

  MINUS        "-"
  PLUS         "+"
  STAR         "*"
  SLASH        "/"
  PERCENT      "%"
;

%token <std::string> IDENTIFIER
%token <int>         CONSTANT
%token <std::string> STRING_LITERAL


%printer { yyo << $$; } <*>;


%%
%start translation_unit;

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
    : SEMICOLON 
    | expression SEMICOLON
    ;

compound_stmt
    : LBRACE stmt_list RBRACE
    ;

if_stmt
    : IF LPAREN expression RPAREN stmt %prec ASSIGN
    | IF LPAREN expression RPAREN stmt ELSE stmt
    ;

while_stmt
    : WHILE LPAREN expression RPAREN stmt
    ;

print_stmt
    : PRINT expression SEMICOLON
    ;

/* Expression levels */

expression
    : assignment_expression
    ;


relational_expression
	: additive_expression
	| relational_expression LESS additive_expression
	| relational_expression GREATER additive_expression
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
    | IDENTIFIER assignment_operator assignment_expression
    ;

assignment_operator
	: ASSIGN
	| MUL_ASSIGN
	| DIV_ASSIGN
	| MOD_ASSIGN
	| ADD_ASSIGN
	| SUB_ASSIGN
	;


additive_expression
    : multiplicative_expression
    | additive_expression PLUS multiplicative_expression
    | additive_expression MINUS multiplicative_expression
    ;

multiplicative_expression
    : primary_expression
    | multiplicative_expression STAR primary_expression
    | multiplicative_expression SLASH primary_expression
    | multiplicative_expression PERCENT primary_expression
    ;

/* primary expression includes numbers, identifiers, STDIN_GET_NUM, parenthesized */
primary_expression
    : IDENTIFIER
    | CONSTANT
    | STDIN_GET_NUM
    | LPAREN expression RPAREN
    ;

%%

void
yy::parser::error (const location_type& l, const std::string& m)
{
  std::cerr << l << ": " << m << '\n';
}

