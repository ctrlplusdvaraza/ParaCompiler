#ifndef AST_NODE_H
#define AST_NODE_H


#ifndef AST_H
#define AST_H

typedef enum {
    N_NUM,
    N_ID,
    N_BINOP,
    N_ASSIGN,
    N_PRINT,
    N_INPUT,
    N_SEQ,
    N_IF,
    N_WHILE,
    N_EXPR_STMT
} NodeType;

typedef struct AST {
    NodeType type;
    char op;            /* for binary ops */
    int ival;           /* for numbers */
    char *name;         /* for identifiers or assignment target */
    struct AST *left;
    struct AST *right;
    struct AST *third;  /* for else branch */
    struct AST *next;   /* for sequences */
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

/* symbol table helpers */
void set_var(const char *name, int val);
int get_var(const char *name);

#endif


#endif // AST_NODE_H