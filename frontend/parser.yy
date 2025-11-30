%skeleton "lalr1.cc"
%require "3.8.1"
%header 

%define api.token.constructor
%define api.value.type variant
%locations
%define parse.error verbose

// inserting into parser.hpp
%code requires {
    #include "ast.hpp"

    #include "driver.hpp"

    using namespace compiler; 
    using namespace compiler::frontend; 
}

// inserting into parser.cpp
%code {
    #include "parser.hpp"

    #include "driver.hpp"

    #define YY_DECL yy::parser::symbol_type yylex(Driver& driver)
    YY_DECL;

    static AstNodePtr WrapInScope(AstNodePtr stmt)
    {
        if (stmt->is_node_type<ScopeNode>())
        {
            return stmt;
        }

        AstNodePtr scope = std::make_unique<ScopeNode>("{");
        scope->children.push_back(std::move(stmt));
        return scope;
    }

    #define REPORT_UNDECLARED_VARIABLE(loc, name)                           \
    do {                                                                    \
        error(loc, "error: " + name + " was not declared in this scope");   \
        YYERROR;                                                            \
    } while (0)                                                         
}


%param { Driver& driver }

%token <std::string> IF           "if"
%token <std::string> ELSE         "else"
%token <std::string> WHILE        "while"
%token <std::string> PRINT        "print"
%token <std::string> INPUT        "?"

%token <std::string> SEMICOLON    ";"

%token <std::string> L_ROUND_BR   "("
%token <std::string> R_ROUND_BR   ")"
%token <std::string> L_CURLY_BR   "{"
%token <std::string> R_CURLY_BR   "}"

%token <std::string> ADD_ASSIGN   "+="
%token <std::string> SUB_ASSIGN   "-="
%token <std::string> MUL_ASSIGN   "*="
%token <std::string> DIV_ASSIGN   "/="
%token <std::string> MOD_ASSIGN   "%="
%token <std::string> ASSIGN       "="

%token <std::string> PLUSPLUS     "++"
%token <std::string> MINUSMINUS   "--"
%token <std::string> PLUS         "+"
%token <std::string> MINUS        "-"
%token <std::string> STAR         "*"
%token <std::string> SLASH        "/"
%token <std::string> PERCENT      "%"

%token <std::string> EQ_CMP       "=="
%token <std::string> NE_CMP       "!="
%token <std::string> LE_CMP       "<="
%token <std::string> L_CMP        "<"
%token <std::string> GE_CMP       ">="
%token <std::string> G_CMP        ">"

%token <std::string> NOT_LOGICAL  "!"
%token <std::string> AND_LOGICAL  "&&"
%token <std::string> OR_LOGICAL   "||"

%token <std::string> IDENTIFIER LITERAL

%nterm <AstRootPtr> translation_unit
%nterm <AstRootPtr> statement_list

%nterm <AstNodePtr> statement
%nterm <AstNodePtr> expression_statement
%nterm <AstNodePtr> expression
%nterm <AstNodePtr> assignment_expression
%nterm <AstNodePtr> primary_expression primary_assignment
%nterm <AstNodePtr> additive_expression multiplicative_expression 
%nterm <AstNodePtr> PLUS_OR_MINUS MUL_OR_DIV_OR_PERCENT
%nterm <AstNodePtr> unary_expression prefix_expression postfix_expression
%nterm <AstNodePtr> UNARY_OP PREFIX_OP POSTFIX_OP
%nterm <AstNodePtr> logical_or_expression logical_and_expression logical_not_expression
%nterm <AstNodePtr> cmp_expression CMP_OPERATORS
%nterm <AstNodePtr> compound_statement
%nterm <AstNodePtr> if_statement
%nterm <AstNodePtr> while_statement
%nterm <AstNodePtr> print_statement

%%
%start translation_unit;

translation_unit
    : statement_list { driver.ast_root = std::move($1); }
    ;

statement_list
    : %empty { $$ = std::make_unique<AstRoot>(); }
    | statement_list statement {
         AstRootPtr node = std::move($1);
         if ($2) 
            node->children.push_back(std::move($2));
         $$ = std::move(node);
    }
    ;

statement 
    : print_statement      { $$ = std::move($1); }
    | expression_statement { $$ = std::move($1); }
    | compound_statement   { $$ = std::move($1); }
    | if_statement         { $$ = std::move($1); }
    | while_statement      { $$ = std::move($1); }
    ;

print_statement
    : PRINT expression_statement {
        AstNodePtr node = std::make_unique<PrintNode>($1);
        node->children.push_back(std::move($2));
        $$ = std::move(node);
    }
    ;

if_statement
    : IF L_ROUND_BR expression R_ROUND_BR statement {
        AstNodePtr node = std::make_unique<IfNode>($1);
        node->children.push_back(std::move($3));
        node->children.push_back(WrapInScope(std::move($5)));
        $$ = std::move(node);
    }
    | IF L_ROUND_BR expression R_ROUND_BR statement ELSE statement {
        AstNodePtr node = std::make_unique<IfNode>($1);
        node->children.push_back(std::move($3));
        node->children.push_back(WrapInScope(std::move($5)));
        node->children.push_back(WrapInScope(std::move($7)));
        $$ = std::move(node);
    }
    ;

while_statement
    : WHILE L_ROUND_BR expression R_ROUND_BR statement {
        AstNodePtr node = std::make_unique<WhileNode>($1);
        node->children.push_back(std::move($3));
        node->children.push_back(WrapInScope(std::move($5)));
        $$ = std::move(node);
    }
    ;

compound_statement
    : L_CURLY_BR { driver.names_environment.push_scope(); }
      statement_list R_CURLY_BR {
        AstNodePtr scope = std::make_unique<ScopeNode>($1);
        for (AstNodePtr& child : $3->children) {
            scope->children.push_back(std::move(child));
        }

        driver.names_environment.pop_scope();
        $$ = std::move(scope);
    }
    ;

expression_statement
    : expression SEMICOLON { $$ = std::move($1); }
    | SEMICOLON { $$ = nullptr; }
    ;

expression
    : assignment_expression { $$ = std::move($1); }
    ;

logical_or_expression
    : logical_and_expression { $$ = std::move($1); }
    | logical_or_expression OR_LOGICAL logical_and_expression {
        AstNodePtr or_operator = std::make_unique<OrNode>($2);
        or_operator->children.push_back(std::move($1));
        or_operator->children.push_back(std::move($3));
        $$ = std::move(or_operator);
    }
    ;

logical_and_expression
    : logical_not_expression { $$ = std::move($1); }
    | logical_and_expression AND_LOGICAL logical_not_expression {
        AstNodePtr and_operator = std::make_unique<AndNode>($2);
        and_operator->children.push_back(std::move($1));
        and_operator->children.push_back(std::move($3));
        $$ = std::move(and_operator);
    }
    ;

logical_not_expression
    : cmp_expression { $$ = std::move($1); }
    | NOT_LOGICAL logical_not_expression {
        AstNodePtr not_operator = std::make_unique<NotNode>($1);
        not_operator->children.push_back(std::move($2));
        $$ = std::move(not_operator);
    }
    ;

cmp_expression
    : additive_expression { $$ = std::move($1); }
    | cmp_expression CMP_OPERATORS additive_expression {
        $2->children.push_back(std::move($1));
        $2->children.push_back(std::move($3));
        $$ = std::move($2);
    }
    ;

CMP_OPERATORS
    : EQ_CMP { $$ = std::make_unique<EqualNode>($1);        }
    | NE_CMP { $$ = std::make_unique<NotEqualNode>($1);     }
    | L_CMP  { $$ = std::make_unique<LessNode>($1);         }
    | G_CMP  { $$ = std::make_unique<GreaterNode>($1);      }
    | LE_CMP { $$ = std::make_unique<LessEqualNode>($1);    }
    | GE_CMP { $$ = std::make_unique<GreaterEqualNode>($1); }
    ;

assignment_expression
    : logical_or_expression { $$ = std::move($1); }
    | IDENTIFIER primary_assignment assignment_expression {
        if (!driver.names_environment.find_variable($1)) { // merged declare/usage semantic
            driver.names_environment.put_variable($1);
        }
        AstNodePtr identifier = std::make_unique<IdentifierNode>($1);
        $2->children.push_back(std::move(identifier));
        $2->children.push_back(std::move($3));
        $$ = std::move($2);
    }
    ;

primary_assignment
    : ASSIGN     { $$ = std::make_unique<AssignmentNode>($1);    }
    | ADD_ASSIGN { $$ = std::make_unique<AddAssignmentNode>($1); }
    | SUB_ASSIGN { $$ = std::make_unique<SubAssignmentNode>($1); }
    | MUL_ASSIGN { $$ = std::make_unique<MulAssignmentNode>($1); }
    | DIV_ASSIGN { $$ = std::make_unique<DivAssignmentNode>($1); }
    | MOD_ASSIGN { $$ = std::make_unique<ModAssignmentNode>($1); }
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
    : PLUS  { $$ = std::make_unique<AddNode>($1);   }
    | MINUS { $$ = std::make_unique<SubNode>($1); }
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
    : STAR    { $$ = std::make_unique<MulNode>($1);     }
    | SLASH   { $$ = std::make_unique<DivNode>($1);     }
    | PERCENT { $$ = std::make_unique<ModNode>($1); }
    ;

unary_expression
    : postfix_expression { $$ = std::move($1); }
    | prefix_expression  { $$ = std::move($1); }
    | primary_expression { $$ = std::move($1); }
    | UNARY_OP unary_expression {
        $1->children.push_back(std::move($2));
        $$ = std::move($1);
    }
    ;

UNARY_OP
    : PLUS  { $$ = std::make_unique<UnaryPlusNode>($1);  }
    | MINUS { $$ = std::make_unique<UnaryMinusNode>($1); }
    ;

postfix_expression
    : IDENTIFIER POSTFIX_OP {
        if (!driver.names_environment.check_variable_usage_semantic($1)) {
            REPORT_UNDECLARED_VARIABLE(@1, $1);
        }  
        AstNodePtr id = std::make_unique<IdentifierNode>($1);
        $2->children.push_back(std::move(id));
        $$ = std::move($2);
    }
    ;

POSTFIX_OP
    : PLUSPLUS   { $$ = std::make_unique<PostfixIncrementNode>($1); }
    | MINUSMINUS { $$ = std::make_unique<PostfixDecrementNode>($1); }
    ;

prefix_expression
    : PREFIX_OP IDENTIFIER {
        if (!driver.names_environment.check_variable_usage_semantic($2)) {
            REPORT_UNDECLARED_VARIABLE(@2, $2);
        }  
        AstNodePtr id = std::make_unique<IdentifierNode>($2);
        $1->children.push_back(std::move(id));
        $$ = std::move($1);
    }
    ;

PREFIX_OP
    : PLUSPLUS   { $$ = std::make_unique<PrefixIncrementNode>($1); }
    | MINUSMINUS { $$ = std::make_unique<PrefixDecrementNode>($1); }
    ;

primary_expression
    : IDENTIFIER { 
        if (!driver.names_environment.check_variable_usage_semantic($1)) {
            REPORT_UNDECLARED_VARIABLE(@1, $1);
        }
        $$ = std::make_unique<IdentifierNode>($1); 
    }
    | LITERAL    { $$ = std::make_unique<LiteralNode>($1);    }
    | L_ROUND_BR expression R_ROUND_BR { $$ = std::move($2); }
    | INPUT { $$ = std::make_unique<InputNode>($1); }
    ;
%%

void yy::parser::error(const location_type& location, const std::string& message)
{
    std::cerr << location << ": " << message << std::endl;
}
