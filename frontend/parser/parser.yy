%skeleton "lalr1.cc" // -*- C++ -*-
%require "3.8.1"
%header 

%define api.token.constructor
%define api.value.type variant
%locations

%code requires {
    #include <string>
    #include <memory>

    #include "ast.hpp"
    using namespace compiler; 
    class Driver;
}

%code {
    #include "driver.hpp"
}

%param { Driver& driver }

%token
    ASSIGN       "="
    ADD_ASSIGN   "+="
    SUB_ASSIGN   "-="
    MUL_ASSIGN   "*="
    DIV_ASSIGN   "/="
    MOD_ASSIGN   "%="

    PLUS         "+"
    MINUS        "-"
    STAR         "*"
    SLASH        "/"
    PERCENT      "%"

    SEMICOLON    ";"
;

%token <std::string> IDENTIFIER LITERAL

%nterm <AstRootPtr> translation_unit
%nterm <AstRootPtr> statement_list

%nterm <AstNodePtr> statement
%nterm <AstNodePtr> expr_stmt
%nterm <AstNodePtr> expression
%nterm <AstNodePtr> assignment_expression
%nterm <AstNodePtr> primary_expression primary_assignment
%nterm <AstNodePtr> additive_expr multiplicative_expr 
$nterm <AstNodePtr> PLUS_OR_MINUS MUL_OR_DIV_OR_PERCENT

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
    : primary_expressioni { $$ = std::move($1); }
    | IDENTIFIER primary_assignment assignment_expression {
        auto ident = std::make_unique<AstNode>(std::make_unique<IdentifierToken>($1));

        $2->children.push_back(std::move(ident));
        $2->children.push_back(std::move($3));
        $$ = std::move($2);
    }
    ;

primary_assignment
    : ASSIGN     { $$ = std::make_unique<AstNode>(std::make_unique<AssignmentToken>());    }
    | ADD_ASSIGN { $$ = std::make_unique<AstNode>(std::make_unique<AddAssignmentToken>()); }
    | SUB_ASSIGN { $$ = std::make_unique<AstNode>(std::make_unique<SubAssignmentToken>()); }
    | MUL_ASSIGN { $$ = std::make_unique<AstNode>(std::make_unique<MulAssignmentToken>()); }
    | DIV_ASSIGN { $$ = std::make_unique<AstNode>(std::make_unique<DivAssignmentToken>()); }
    | MOD_ASSIGN { $$ = std::make_unique<AstNode>(std::make_unique<ModAssignmentToken>()); }
    ;

additive_expr
    : multiplicative_expr { $$ = std::move($1); }
    | additive_expr PLUS_OR_MINUS multiplicative_expr {
        $2->children.push_back(std::move($1));
        $2->children.push_back(std::move($3));
        $$ = std::move($2);
    }
    ;

multiplicative_expr
    : primary_expression { $$ = std::move($1); }
    | multiplicative_expr MUL_OR_DIV_OR_PERCENT primary_expression {
        $2->children.push_back(std::move($1));
        $2->children.push_back(std::move($3));
        $$ = std::move($2);
    }
    ;
        
PLUS_OR_MINUS
    : PLUS  { $$ = std::make_unique<AstNode>(std::make_unique<AddOperator>());   }
    | MINUS { $$ = std::make_unique<AstNode>(std::make_unique<MinusOperator>()); }
    ;

MUL_OR_DIV_OR_PERCENT
    : MUL     { $$ = std::make_unique<AstNode>(std::make_unique<MulOperator>());     }
    | DIV     { $$ = std::make_unique<AstNode>(std::make_unique<DivOperator>());     }
    | PERCENT { $$ = std::make_unique<AstNode>(std::make_unique<PercentOperator>()); }
    ;

primary_expression
    : IDENTIFIER { $$ = std::make_unique<AstNode>(std::make_unique<IdentifierToken>($1)); }
    | LITERAL    { $$ = std::make_unique<AstNode>(std::make_unique<LiteralToken>($1));    }
    ;
%%

void yy::parser::error (const location_type& l, const std::string& m)
{
    std::cerr << l << ": " << m << '\n';
 }
