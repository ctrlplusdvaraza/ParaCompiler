%skeleton "lalr1.cc" // -*- C++ -*-
%require "3.8.1"
%header "parser.hh"


%define api.token.constructor
%define api.value.type variant
%define parse.assert

%code requires {
  #include <string>
  #include <memory>
  #include "inc/ast.hh"

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

%nterm <std::shared_ptr<TranslationUnit>> translation_unit stmt_list
%nterm <std::shared_ptr<Statement>> stmt expr_stmt compound_stmt if_stmt while_stmt print_stmt
%nterm <std::shared_ptr<Expression>> expression additive_expression multiplicative_expression primary_expression assignment_expression
%nterm <std::shared_ptr<Expression>> conditional_expression
%nterm <std::shared_ptr<Expression>> logical_or_expression
%nterm <std::shared_ptr<Expression>> logical_and_expression
%nterm <std::shared_ptr<Expression>> inclusive_or_expression
%nterm <std::shared_ptr<Expression>> exclusive_or_expression
%nterm <std::shared_ptr<Expression>> and_expression
%nterm <std::shared_ptr<Expression>> equality_expression
%nterm <std::shared_ptr<Expression>> relational_expression
%nterm <std::string> assignment_operator

%printer { yyo << $$; } <*>;


%%
%start translation_unit;

/* ------------------ grammar rules ------------------ */

translation_unit
    : stmt_list { drv.result = $1; }
    ;

stmt_list
    : %empty { $$ = std::make_shared<TranslationUnit>(); }
    | stmt_list stmt {
        auto tu = std::dynamic_pointer_cast<TranslationUnit>($1);
        tu->stmts.push_back(std::dynamic_pointer_cast<Statement>($2));
        $$ = tu;
    }
    ;

stmt
    : expr_stmt     { $$ = $1; }
    | compound_stmt { $$ = $1; }
    | if_stmt       { $$ = $1; }
    | while_stmt    { $$ = $1; }
    | print_stmt    { $$ = $1; }
    ;

expr_stmt
    : SEMICOLON { $$ = std::make_shared<ExprStmt>(); }
    | expression SEMICOLON { 
        auto s = std::make_shared<ExprStmt>();
        s->expr = std::dynamic_pointer_cast<Expression>($1);
        $$ = s;
    }
    ;

compound_stmt
    : LBRACE stmt_list RBRACE { 
        auto s = std::make_shared<CompoundStmt>();
        auto tu = std::dynamic_pointer_cast<TranslationUnit>($2);
        s->stmts = tu->stmts;
        $$ = s;
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
        auto s = std::make_shared<WhileStmt>();
        s->cond = std::dynamic_pointer_cast<Expression>($3);
        s->body = std::dynamic_pointer_cast<Statement>($5);
        $$ = s;
    }
    ;

print_stmt
    : PRINT expression SEMICOLON {
        auto s = std::make_shared<PrintStmt>();
        s->expr = std::dynamic_pointer_cast<Expression>($2);
        $$ = s;
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
    | STDIN_GET_NUM { $$ = std::make_shared<StdInGetNum>(); }
    | LPAREN expression RPAREN { $$ = $2; }
    ;

%%

void
yy::parser::error (const location_type& l, const std::string& m)
{
  std::cerr << l << ": " << m << '\n';
}

