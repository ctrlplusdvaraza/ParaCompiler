#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "symtab.h"
#include "ast_node.h"

/* ------------------ C user code: constructors, interpreter ------------------ */

static Sym *symtab = NULL;

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
