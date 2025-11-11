#pragma once



struct AST_node {
    char op;
    AST_node *left, *right;
    int value;
};

AST_node* make_num(int val);
AST_node* make_op(char op, AST_node* l, AST_node* r);
void print_ast(AST_node* n, int depth);
