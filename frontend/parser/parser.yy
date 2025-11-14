%skeleton "lalr1.cc" // -*- C++ -*-
%require "3.8.1"
%header 

%define api.token.constructor
%define api.value.type variant

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
%token <std::string> LITERAL

%nterm <compiler::AstRootPtr> translation_unit
%nterm <compiler::AstRootPtr> statement_list

%nterm <compiler::AstNodePtr> statement
%nterm <compiler::AstNodePtr> expr_stmt
%nterm <compiler::AstNodePtr> expression
%nterm <compiler::AstNodePtr> assignment_expression
%nterm <compiler::AstNodePtr> primary_expression

%%
%start translation_unit;

translation_unit
    : statement_list { driver.ast_root = std::move($1); }
    ;

statement_list
    : %empty { $$ = std::make_unique<compiler::AstRoot>(); }
    | statement_list statement
        {
            compiler::AstRootPtr main_node = std::move($1);
            main_node->children.push_back(std::move($2));
            $$ = std::move(main_node);
        }
    ;

statement
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
        compiler::AstNodePtr main_node = std::make_unique<compiler::AstNode>(
            std::make_unique<compiler::AssignmentToken>("=")
        );
        main_node->children.push_back(std::move($1));
        main_node->children.push_back(std::move($3));
        $$ = std::move(main_node);
    }
    ;

primary_expression
    : IDENTIFIER 
        { $$ = std::make_unique<compiler::AstNode>(std::make_unique<compiler::IdentifierToken>($1)); }

    | LITERAL 
        { $$ = std::make_unique<compiler::AstNode>(std::make_unique<compiler::LiteralToken>($1)); }
    ;

%%

void yy::parser::error (const location_type& l, const std::string& m)
{
    std::cerr << l << ": " << m << '\n';
}
