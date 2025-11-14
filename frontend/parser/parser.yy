%skeleton "lalr1.cc" // -*- C++ -*-
%require "3.8.1"
%header 

%code requires {
  #include <string>
  #include <memory>
  #include "ast.hpp"

  class Driver;
}

%param { Driver& driver }

%locations

%code {
#include "driver.hpp"
}

%define api.token.prefix {TOK_}

%token
  ASSIGN       "="
  SEMICOLON    ";"
;

%token <std::string> IDENTIFIER
%token <int>         LITERAL

%%
%start translation_unit;

translation_unit
    : stmt_list { driver.ast_root = std::move($1); }
    ;

stmt_list
    : %empty { $$ = std::make_unique<AstRoot>(); }
    | stmt_list stmt {
        auto main_node = std::move($1);
        main_node->children.push_back(std::move($2));
        $$ = std::move(main_node);
    }
    ;

stmt
    : expr_stmt { $$ = std::move($1); }
    ;

expr_stmt
    : expression SEMICOLON {
        $$ = std::move($1);
    }
    ;

expression
    : assignment_expression { $$ = std::move($1); }
    ;

assignment_expression
    : primary_expression ASSIGN primary_expression {
        auto main_node = std::make_unique<compiler::AstNode>(
            std::make_unique<compiler::AssignmentToken>()
        );
        main_node->children.push_back(std::move($1));
        main_node->children.push_back(std::move($3));
        $$ = std::move(main_node);
    }
    ;

primary_expression
    : IDENTIFIER { $$ = std::make_unique<compiler::AstNode>(
                    std::make_unique<compiler::IdentifierToken>($1)); }
    | LITERAL { $$ = std::make_unique<compiler::AstNode>(
                    std::make_unique<compiler::LiteralToken>($1)); }
    ;

%%

