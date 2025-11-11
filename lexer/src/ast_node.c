#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "ast_node.h"

AST *new_num(int v) {
    AST *n = (AST*)malloc(sizeof(AST));
    n->type = N_NUM; n->ival = v; n->name = NULL; n->op = 0; n->left = n->right = n->third = n->next = NULL;
    return n;
}
AST *new_id(char *name) {
    assert(name);

    AST *n = (AST*)malloc(sizeof(AST));
    n->type = N_ID; n->name = strdup(name); n->op = 0; n->left = n->right = n->third = n->next = NULL;
    return n;
}
AST *new_binop(char op, AST *l, AST *r) {
    assert(l);
    assert(r);

    AST *n = (AST*)malloc(sizeof(AST));
    n->type = N_BINOP; n->op = op; n->left = l; n->right = r; n->third = n->next = NULL; n->name = NULL;
    return n;
}
AST *new_assign(char *name, AST *expr) {
    assert(name);
    assert(expr);

    AST *n = (AST*)malloc(sizeof(AST));
    n->type = N_ASSIGN; n->name = strdup(name); n->left = expr; n->right = NULL; n->third = n->next = NULL; n->op = 0;
    return n;
}
AST *new_print(AST *expr) {
    assert(expr);

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
    assert(cond);
    assert(thenb);
    assert(elseb);

    AST *n = (AST*)malloc(sizeof(AST));
    n->type = N_IF; n->left = cond; n->right = thenb; n->third = elseb; n->next = NULL; n->name = NULL; n->op = 0;
    return n;
}
AST *new_while(AST *cond, AST *body) {
    assert(cond);
    assert(body);

    AST *n = (AST*)malloc(sizeof(AST));
    n->type = N_WHILE; n->left = cond; n->right = body; n->third = NULL; n->next = NULL; n->name = NULL; n->op = 0;
    return n;
}
AST *new_expr_stmt(AST *e) {
    assert(e);

    AST *n = (AST*)malloc(sizeof(AST));
    n->type = N_EXPR_STMT; n->left = e; n->right = n->third = n->next = NULL; n->name = NULL; n->op = 0;
    return n;
}