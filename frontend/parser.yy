%skeleton "lalr1.cc"
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

%token <std::string> SEMICOLON    ";"

%token <std::string> L_ROUND_BR   "("
%token <std::string> R_ROUND_BR   ")"

%token <std::string> ASSIGN       "="
%token <std::string> ADD_ASSIGN   "+="
%token <std::string> SUB_ASSIGN   "-="
%token <std::string> MUL_ASSIGN   "*="
%token <std::string> DIV_ASSIGN   "/="
%token <std::string> MOD_ASSIGN   "%="

%token <std::string> PLUSPLUS     "++"
%token <std::string> MINUSMINUS   "--"
%token <std::string> PLUS         "+"
%token <std::string> MINUS        "-"
%token <std::string> STAR         "*"
%token <std::string> SLASH        "/"
%token <std::string> PERCENT      "%"

%token <std::string> IDENTIFIER LITERAL

%nterm <std::unique_ptr<TranslationUnitNode>> translation_unit
%nterm <std::unique_ptr<TranslationUnitNode>> statement_list

%nterm <AstNodePtr> statement
%nterm <AstNodePtr> expression_stmt
%nterm <AstNodePtr> expression
%nterm <AstNodePtr> assignment_expression
%nterm <AstNodePtr> primary_expression primary_assignment
%nterm <AstNodePtr> additive_expression multiplicative_expression 
%nterm <AstNodePtr> PLUS_OR_MINUS MUL_OR_DIV_OR_PERCENT
%nterm <AstNodePtr> unary_expression prefix_expression postfix_expression
%nterm <AstNodePtr> UNARY_OP PREFIX_OP POSTFIX_OP

%%
%start translation_unit;

translation_unit
    : statement_list { driver.ast_root = std::move($1); }
    ;

statement_list
    : %empty { $$ = std::make_unique<TranslationUnitNode>(); }
    | statement_list statement
        {
            std::unique_ptr<TranslationUnitNode> node = std::move($1);
            node->children.push_back(std::move($2));
            $$ = std::move(node);
        }
    ;

statement
    : expression_stmt { $$ = std::move($1); }
    ;

expression_stmt
    : expression SEMICOLON {
        $$ = std::move($1);
    }
    ;

expression
    : assignment_expression { $$ = std::move($1); }
    ;

assignment_expression
    : additive_expression { $$ = std::move($1); }
    | IDENTIFIER primary_assignment assignment_expression {
        auto ident = std::make_unique<AstNode>(std::make_unique<IdentifierToken>($1));
        $2->children.push_back(std::move(ident));
        $2->children.push_back(std::move($3));
        $$ = std::move($2);
    }
    ;

primary_assignment
    : ASSIGN     { $$ = std::make_unique<AstNode>(std::make_unique<AssignmentToken>($1));    }
    | ADD_ASSIGN { $$ = std::make_unique<AstNode>(std::make_unique<AddAssignmentToken>($1)); }
    | SUB_ASSIGN { $$ = std::make_unique<AstNode>(std::make_unique<SubAssignmentToken>($1)); }
    | MUL_ASSIGN { $$ = std::make_unique<AstNode>(std::make_unique<MulAssignmentToken>($1)); }
    | DIV_ASSIGN { $$ = std::make_unique<AstNode>(std::make_unique<DivAssignmentToken>($1)); }
    | MOD_ASSIGN { $$ = std::make_unique<AstNode>(std::make_unique<ModAssignmentToken>($1)); }
    ;

additive_expression
    : multiplicative_expression { $$ = std::move($1); }
    | additive_expression PLUS_OR_MINUS multiplicative_expression {
        $2->children.push_back(std::move($1));
        $2->children.push_back(std::move($3));
        $$ = std::move($2);
    }
    ;

PLUS_OR_MINUS
    : PLUS  { $$ = std::make_unique<AstNode>(std::make_unique<AddToken>($1));   }
    | MINUS { $$ = std::make_unique<AstNode>(std::make_unique<SubToken>($1)); }
    ;

multiplicative_expression
    : unary_expression { $$ = std::move($1); }
    | multiplicative_expression MUL_OR_DIV_OR_PERCENT unary_expression {
        $2->children.push_back(std::move($1));
        $2->children.push_back(std::move($3));
        $$ = std::move($2);
    }
    ;

MUL_OR_DIV_OR_PERCENT
    : STAR    { $$ = std::make_unique<AstNode>(std::make_unique<MulToken>($1));     }
    | SLASH   { $$ = std::make_unique<AstNode>(std::make_unique<DivToken>($1));     }
    | PERCENT { $$ = std::make_unique<AstNode>(std::make_unique<ModToken>($1)); }
    ;

unary_expression
    : prefix_expression { $$ = std::move($1); }
    | UNARY_OP unary_expression {
        $1->children.push_back(std::move($2));
        $$ = std::move($1);
    }
    ;

UNARY_OP
    : PLUS  { $$ = std::make_unique<AstNode>(std::make_unique<UnaryPlusToken>($1));  }
    | MINUS { $$ = std::make_unique<AstNode>(std::make_unique<UnaryMinusToken>($1)); }
    ;

prefix_expression
    : postfix_expression { $$ = std::move($1); }
    | PREFIX_OP prefix_expression {
        $1->children.push_back(std::move($2));
        $$ = std::move($1);
    }
    ;

PREFIX_OP
    : PLUSPLUS   { $$ = std::make_unique<AstNode>(std::make_unique<PrefixIncrementToken>($1)); }
    | MINUSMINUS { $$ = std::make_unique<AstNode>(std::make_unique<PrefixDecrementToken>($1)); }
    ;

postfix_expression
    : primary_expression { $$ = std::move($1); }
    | postfix_expression POSTFIX_OP {
        $2->children.push_back(std::move($1));
        $$ = std::move($2);
    }
    ;

POSTFIX_OP
    : PLUSPLUS   { $$ = std::make_unique<AstNode>(std::make_unique<PostfixIncrementToken>($1)); }
    | MINUSMINUS { $$ = std::make_unique<AstNode>(std::make_unique<PostfixDecrementToken>($1)); }
    ;

primary_expression
    : IDENTIFIER { $$ = std::make_unique<AstNode>(std::make_unique<IdentifierToken>($1)); }
    | LITERAL    { $$ = std::make_unique<AstNode>(std::make_unique<LiteralToken>($1));    }
    | L_ROUND_BR expression R_ROUND_BR { $$ = std::move($2); }
    ;
%%

void yy::parser::error (const location_type& l, const std::string& m)
{
    std::cerr << l << ": " << m << '\n';
}
