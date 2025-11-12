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
%start unit;
unit: 
    %empty

%%

void
yy::parser::error (const location_type& l, const std::string& m)
{
  std::cerr << l << ": " << m << '\n';
}

