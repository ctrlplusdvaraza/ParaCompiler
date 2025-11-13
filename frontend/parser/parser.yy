%skeleton "lalr1.cc" // -*- C++ -*-
%require "3.8.1"
%header 


%define api.token.constructor
%define api.value.type variant
%define parse.assert

%code requires {
  #include <string>
  #include <memory>
  #include "ast.hpp"

  class Driver;
}

// The parsing context.
%param { Driver& driver }

%locations

%define parse.trace
%define parse.error detailed
%define parse.lac full

%code {
#include "driver.hpp"
}

%define api.token.prefix {TOK_}

%token
  ELSE         "else"
  IF           "if"
  WHILE        "while"
  INPUT        "?"
  PRINT        "print"

  ADD_ASSIGN   "+="
  SUB_ASSIGN   "-="
  MUL_ASSIGN   "*="
  DIV_ASSIGN   "/="
  MOD_ASSIGN   "%="
  ASSIGN       "="
  
  LE_CMP       "<="
  GE_CMP       ">="
  EQ_CMP       "=="
  NE_CMP       "!="
  L_CMP        "<"
  G_CMP        ">"
  
  SEMICOLON    ";"
  
  L_CURLY_BR   "{"
  R_CURLY_BR   "}"
  L_ROUND_BR   "("
  R_ROUND_BR   ")"

  MINUS        "-"
  PLUS         "+"
  STAR         "*"
  SLASH        "/"
  PERCENT      "%"
;

%token <std::string> IDENTIFIER
%token <int>         CONSTANT
%token <std::string> STRING_LITERAL

%nterm <std::unique_ptr<TranslationUnit>> translation_unit stmt_list
%nterm <std::unique_ptr<Statement>> stmt expr_stmt compound_stmt if_stmt while_stmt print_stmt
%nterm <std::unique_ptr<Expression>> expression additive_expression multiplicative_expression primary_expression assignment_expression
%nterm <std::unique_ptr<Expression>> conditional_expression
%nterm <std::unique_ptr<Expression>> logical_or_expression
%nterm <std::unique_ptr<Expression>> logical_and_expression
%nterm <std::unique_ptr<Expression>> inclusive_or_expression
%nterm <std::unique_ptr<Expression>> exclusive_or_expression
%nterm <std::unique_ptr<Expression>> and_expression
%nterm <std::unique_ptr<Expression>> equality_expression
%nterm <std::unqiue_ptr<Expression>> relational_expression
%nterm <std::string> assignment_operator

%printer { yyo << $$; } <*>;


%%
%start translation_unit;

translation_unit
    : stmt_list { drv.ast_root = std::move($1); }
    ;

stmt_list
    : %empty { $$ = std::make_unique<TranslationUnit>(); }
    | stmt_list stmt {
        auto tu = std::move($1);
        tu->stmts.push_back(std::move($2));
        $$ = std::move(tu);
    }
    ;

stmt
    : expr_stmt     { $$ = std::move($1); }
    | compound_stmt { $$ = std::move($1); }
    | if_stmt       { $$ = std::move($1); }
    | while_stmt    { $$ = std::move($1); }
    | print_stmt    { $$ = std::move($1); }
    ;

expr_stmt
    : SEMICOLON { $$ = std::make_unique<ExprStmt>(); }
    | expression SEMICOLON {
        auto s = std::make_unique<ExprStmt>();
        s->expr = std::move($1);
        $$ = std::move(s);
    }
    ;

compound_stmt
    : LBRACE stmt_list RBRACE {
        auto s = std::make_unique<CompoundStmt>();
        auto tu = std::move($2);
        s->stmts = std::move(tu->stmts);
        $$ = std::move(s);
    }
    ;

if_stmt
    : IF LPAREN expression RPAREN stmt {
        auto s = std::make_shared<IfStmt>();
        s->cond = std::dynamic_pointer_cast<Expression>($3);
        s->thenStmt = std::dynamic_pointer_cast<Statement>($5);
        s->elseStmt = nullptr;
        $$ = s;
    }
    | IF LPAREN expression RPAREN stmt ELSE stmt {
        auto s = std::make_shared<IfStmt>();
        s->cond = std::dynamic_pointer_cast<Expression>($3);
        s->thenStmt = std::dynamic_pointer_cast<Statement>($5);
        s->elseStmt = std::dynamic_pointer_cast<Statement>($7);
        $$ = s;
    }
    ;

while_stmt
    : WHILE LPAREN expression RPAREN stmt {
        auto main_node = std::make_unique<compiler::AstNode>(
            std::make_unique<compiler::WhileToken>()
        );
        main_node->children.push_back(std::move($3)); 
        main_node->chidren.push_back(std::move($5));
        $$ = std::move(main_node);
    }
    ;

print_stmt
    : PRINT expression SEMICOLON {
        auto main_node = std::make_shared<compiler::AstNode>(
            std::make_unique<compiler::PrintToken>()
        );
        main_node.push_back(std::move($2));
        $$ = std::move(main_node);
    }
    ;

expression
    : assignment_expression { $$ = $1; }
    ;

logical_or_expression
    : logical_and_expression { $$ = $1; }
    | logical_or_expression "||" logical_and_expression {
        auto e = std::make_shared<BinaryExpr>();
        e->lhs = std::dynamic_pointer_cast<Expression>($1);
        e->rhs = std::dynamic_pointer_cast<Expression>($3);
        e->op = "||";
        $$ = e;
    }
    ;

logical_and_expression
    : inclusive_or_expression { $$ = $1; }
    | logical_and_expression "&&" inclusive_or_expression {
        auto e = std::make_shared<BinaryExpr>();
        e->lhs = std::dynamic_pointer_cast<Expression>($1);
        e->rhs = std::dynamic_pointer_cast<Expression>($3);
        e->op = "&&";
        $$ = e;
    }
    ;

equality_expression
    : relational_expression { $$ = $1; }
    | equality_expression EQ_OP relational_expression {
        auto e = std::make_shared<BinaryExpr>();
        e->lhs = std::dynamic_pointer_cast<Expression>($1);
        e->rhs = std::dynamic_pointer_cast<Expression>($3);
        e->op = "==";
        $$ = e;
    }
    | equality_expression NE_OP relational_expression {
        auto e = std::make_shared<BinaryExpr>();
        e->lhs = std::dynamic_pointer_cast<Expression>($1);
        e->rhs = std::dynamic_pointer_cast<Expression>($3);
        e->op = "!=";
        $$ = e;
    }
    ;

relational_expression
    : additive_expression { $$ = $1; }
    | relational_expression LESS additive_expression {
        auto e = std::make_shared<BinaryExpr>();
        e->lhs = std::dynamic_pointer_cast<Expression>($1);
        e->rhs = std::dynamic_pointer_cast<Expression>($3);
        e->op = "<";
        $$ = e;
    }
    | relational_expression GREATER additive_expression {
        auto e = std::make_shared<BinaryExpr>();
        e->lhs = std::dynamic_pointer_cast<Expression>($1);
        e->rhs = std::dynamic_pointer_cast<Expression>($3);
        e->op = ">";
        $$ = e;
    }
    | relational_expression LE_OP additive_expression {
        auto e = std::make_shared<BinaryExpr>();
        e->lhs = std::dynamic_pointer_cast<Expression>($1);
        e->rhs = std::dynamic_pointer_cast<Expression>($3);
        e->op = "<=";
        $$ = e;
    }
    | relational_expression GE_OP additive_expression {
        auto e = std::make_shared<BinaryExpr>();
        e->lhs = std::dynamic_pointer_cast<Expression>($1);
        e->rhs = std::dynamic_pointer_cast<Expression>($3);
        e->op = ">=";
        $$ = e;
    }
    ;

and_expression
    : equality_expression { $$ = $1; }
    | and_expression '&' equality_expression {
        auto e = std::make_shared<BinaryExpr>();
        e->lhs = std::dynamic_pointer_cast<Expression>($1);
        e->rhs = std::dynamic_pointer_cast<Expression>($3);
        e->op = "&";
        $$ = e;
    }
    ;

exclusive_or_expression
    : and_expression { $$ = $1; }
    | exclusive_or_expression '^' and_expression {
        auto e = std::make_shared<BinaryExpr>();
        e->lhs = std::dynamic_pointer_cast<Expression>($1);
        e->rhs = std::dynamic_pointer_cast<Expression>($3);
        e->op = "^";
        $$ = e;
    }
    ;

inclusive_or_expression
    : exclusive_or_expression { $$ = $1; }
    | inclusive_or_expression '|' exclusive_or_expression {
        auto e = std::make_shared<BinaryExpr>();
        e->lhs = std::dynamic_pointer_cast<Expression>($1);
        e->rhs = std::dynamic_pointer_cast<Expression>($3);
        e->op = "|";
        $$ = e;
    }
    ;

conditional_expression
    : logical_or_expression { $$ = $1; }
    | logical_or_expression '?' expression ':' conditional_expression {
        // Represent ternary as a special BinaryExpr with op="?:"
        auto e = std::make_shared<BinaryExpr>();
        e->op = "?:";
        e->lhs = std::dynamic_pointer_cast<Expression>($1); // condition
        // encode middle and last expressions as a pair using BinaryExpr for simplicity
        auto middle = std::make_shared<BinaryExpr>();
        middle->lhs = std::dynamic_pointer_cast<Expression>($3); // true branch
        middle->rhs = std::dynamic_pointer_cast<Expression>($5); // false branch
        middle->op = ":"; // optional, just to differentiate
        e->rhs = middle;
        $$ = e;
    }
    ;

assignment_expression
    : conditional_expression { $$ = $1; }
    | IDENTIFIER assignment_operator assignment_expression {
        auto e = std::make_shared<BinaryExpr>();
        e->lhs = std::make_shared<Identifier>($1);
        e->op = $2;
        e->rhs = std::dynamic_pointer_cast<Expression>($3);
        $$ = e;
    }
    ;

assignment_operator
    : ASSIGN       { $$ = "="; }
    | MUL_ASSIGN   { $$ = "*="; }
    | DIV_ASSIGN   { $$ = "/="; }
    | MOD_ASSIGN   { $$ = "%="; }
    | ADD_ASSIGN   { $$ = "+="; }
    | SUB_ASSIGN   { $$ = "-="; }
    ;

additive_expression
    : multiplicative_expression { $$ = $1; }
    | additive_expression PLUS multiplicative_expression {
        auto e = std::make_shared<BinaryExpr>();
        e->lhs = std::dynamic_pointer_cast<Expression>($1);
        e->rhs = std::dynamic_pointer_cast<Expression>($3);
        e->op = "+";
        $$ = e;
    }
    | additive_expression MINUS multiplicative_expression {
        auto e = std::make_shared<BinaryExpr>();
        e->lhs = std::dynamic_pointer_cast<Expression>($1);
        e->rhs = std::dynamic_pointer_cast<Expression>($3);
        e->op = "-";
        $$ = e;
    }
    ;

multiplicative_expression
    : primary_expression { $$ = $1; }
    | multiplicative_expression STAR primary_expression {
        auto e = std::make_shared<BinaryExpr>();
        e->lhs = std::dynamic_pointer_cast<Expression>($1);
        e->rhs = std::dynamic_pointer_cast<Expression>($3);
        e->op = "*";
        $$ = e;
    }
    | multiplicative_expression SLASH primary_expression {
        auto e = std::make_shared<BinaryExpr>();
        e->lhs = std::dynamic_pointer_cast<Expression>($1);
        e->rhs = std::dynamic_pointer_cast<Expression>($3);
        e->op = "/";
        $$ = e;
    }
    | multiplicative_expression PERCENT primary_expression {
        auto e = std::make_shared<BinaryExpr>();
        e->lhs = std::dynamic_pointer_cast<Expression>($1);
        e->rhs = std::dynamic_pointer_cast<Expression>($3);
        e->op = "%";
        $$ = e;
    }
    ;

primary_expression
    : IDENTIFIER { $$ = std::make_shared<Identifier>($1); }
    | CONSTANT   { $$ = std::make_shared<Constant>($1); }
    | INPUT { $$ = std::make_shared<StdInGetNum>(); }
    | LPAREN expression RPAREN { $$ = $2; }
    ;

%%

void
yy::parser::error (const location_type& l, const std::string& m)
{
  std::cerr << l << ": " << m << '\n';
}

