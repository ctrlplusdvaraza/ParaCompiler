/* parser.y - minimal grammar + AST + interpreter for your language */

%{
/* C prologue */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Forward of yylex / yyerror */
int yylex(void);
int yyerror(const char *s);

/* AST node definitions */
typedef enum {
    N_NUM,
    N_ID,
    N_BINOP,
    N_ASSIGN,
    N_PRINT,
    N_INPUT,   /* '?' */
    N_SEQ,     /* sequence/list of statements */
    N_IF,
    N_WHILE,
    N_EXPR_STMT
} NodeType;

typedef struct AST {
    NodeType type;
    char op;            /* for binary ops: '+','-','*','/' '%' or comparator as token char */
    int ival;           /* for N_NUM */
    char *name;         /* for N_ID and assignment target */
    struct AST *left;   /* generic children */
    struct AST *right;
    struct AST *third;  /* for if: else branch or unused */
    struct AST *next;   /* for sequence linking */
} AST;

/* constructors */
AST *new_num(int v);
AST *new_id(char *name);
AST *new_binop(char op, AST *l, AST *r);
AST *new_assign(char *name, AST *expr);
AST *new_print(AST *expr);
AST *new_input(void);
AST *new_seq(AST *a, AST *b);
AST *new_if(AST *cond, AST *thenb, AST *elseb);
AST *new_while(AST *cond, AST *body);
AST *new_expr_stmt(AST *e);

/* interpreter */
int eval_expr(AST *n);
void exec_stmt(AST *n);

/* simple symbol table (string -> int) */
typedef struct Sym {
    char *name;
    int val;
    struct Sym *next;
} Sym;
static Sym *symtab = NULL;
void set_var(const char *name, int val);
int get_var(const char *name);

/* root */
AST *root = NULL;

%}

/* semantic value union */
%union {
    int ival;
    char *sval;
    AST *node;
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
    : stmt_list            { root = $1; /* after parse, execute */ 
                             /* execute top-level sequence */ 
                             if (root) exec_stmt(root); }
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

/* ------------------ C user code: constructors, interpreter ------------------ */

AST *new_num(int v) {
    AST *n = (AST*)malloc(sizeof(AST));
    n->type = N_NUM; n->ival = v; n->name = NULL; n->op = 0; n->left = n->right = n->third = n->next = NULL;
    return n;
}
AST *new_id(char *name) {
    AST *n = (AST*)malloc(sizeof(AST));
    n->type = N_ID; n->name = strdup(name); n->op = 0; n->left = n->right = n->third = n->next = NULL;
    return n;
}
AST *new_binop(char op, AST *l, AST *r) {
    AST *n = (AST*)malloc(sizeof(AST));
    n->type = N_BINOP; n->op = op; n->left = l; n->right = r; n->third = n->next = NULL; n->name = NULL;
    return n;
}
AST *new_assign(char *name, AST *expr) {
    AST *n = (AST*)malloc(sizeof(AST));
    n->type = N_ASSIGN; n->name = strdup(name); n->left = expr; n->right = NULL; n->third = n->next = NULL; n->op = 0;
    return n;
}
AST *new_print(AST *expr) {
    AST *n = (AST*)malloc(sizeof(AST));
    n->type = N_PRINT; n->left = expr; n->right = n->third = n->next = NULL; n->name = NULL; n->op = 0;
    return n;
}
AST *new_input(void) {
    AST *n = (AST*)malloc(sizeof(AST));
    n->type = N_INPUT; n->left = n->right = n->third = n->next = NULL; n->name = NULL; n->op = 0;
    return n;
}
AST *new_seq(AST *a, AST *b) {
    if (!a) return b;
    if (!b) return a;
    /* append b at end of a */
    AST *t = a;
    while (t->next) t = t->next;
    t->next = b;
    return a;
}
AST *new_if(AST *cond, AST *thenb, AST *elseb) {
    AST *n = (AST*)malloc(sizeof(AST));
    n->type = N_IF; n->left = cond; n->right = thenb; n->third = elseb; n->next = NULL; n->name = NULL; n->op = 0;
    return n;
}
AST *new_while(AST *cond, AST *body) {
    AST *n = (AST*)malloc(sizeof(AST));
    n->type = N_WHILE; n->left = cond; n->right = body; n->third = NULL; n->next = NULL; n->name = NULL; n->op = 0;
    return n;
}
AST *new_expr_stmt(AST *e) {
    AST *n = (AST*)malloc(sizeof(AST));
    n->type = N_EXPR_STMT; n->left = e; n->right = n->third = n->next = NULL; n->name = NULL; n->op = 0;
    return n;
}

/* symbol table helpers */
void set_var(const char *name, int val) {
    Sym *s = symtab;
    while (s) {
        if (strcmp(s->name, name) == 0) { s->val = val; return; }
        s = s->next;
    }
    s = (Sym*)malloc(sizeof(Sym));
    s->name = strdup(name);
    s->val = val;
    s->next = symtab;
    symtab = s;
}
int get_var(const char *name) {
    Sym *s = symtab;
    while (s) { if (strcmp(s->name, name) == 0) return s->val; s = s->next; }
    return 0; /* default 0 if not set */
}

/* evaluate expression, returns int */
int eval_expr(AST *n) {
    if (!n) return 0;
    switch (n->type) {
        case N_NUM: return n->ival;
        case N_ID: return get_var(n->name);
        case N_INPUT: {
            int v = 0;
            if (scanf("%d", &v) != 1) v = 0;
            return v;
        }
        case N_BINOP: {
            int a = eval_expr(n->left);
            int b = eval_expr(n->right);
            switch (n->op) {
                case '+': return a + b;
                case '-': return a - b;
                case '*': return a * b;
                case '/': return b ? (a / b) : 0;
                case '%': return b ? (a % b) : 0;
                case '<': return a < b;
                case '>': return a > b;
                case '=': return a == b;
                case '!' : return a != b;
                case 'L': return a <= b; /* we map LE_OP -> 'L' below when creating node */
                case 'G': return a >= b; /* GE_OP -> 'G' */
                default: return 0;
            }
        }
        case N_ASSIGN: {
            int val = eval_expr(n->left);
            set_var(n->name, val);
            return val;
        }
        default:
            return 0;
    }
}

/* execute statement node(s) */
void exec_stmt(AST *n) {
    for (AST *cur = n; cur; cur = cur->next) {
        if (!cur) continue;
        switch (cur->type) {
            case N_EXPR_STMT: if (cur->left) eval_expr(cur->left); break;
            case N_PRINT: {
                int v = eval_expr(cur->left);
                printf("%d\n", v);
                break;
            }
            case N_IF: {
                int c = eval_expr(cur->left);
                if (c) exec_stmt(cur->right);
                else if (cur->third) exec_stmt(cur->third);
                break;
            }
            case N_WHILE: {
                while (eval_expr(cur->left)) {
                    exec_stmt(cur->right);
                }
                break;
            }
            case N_ASSIGN: { /* top-level assignment as statement */
                eval_expr(cur);
                break;
            }
            default:
                /* also handle sequences (already flattened via next) */
                break;
        }
    }
}

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
        /* parsing done; root was executed inside rule, but if you'd prefer, execute here:
           if (root) exec_stmt(root);
        */
        return 0;
    }
    return 1;
}
