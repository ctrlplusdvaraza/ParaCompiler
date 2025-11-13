#include <iostream>
#include "driver.hpp"


#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include "ast.hpp"

void print_indent(int level) {
    for (int i = 0; i < level; ++i) std::cout << "  ";
}

void traverse_ast(const std::shared_ptr<ASTNode>& node, int level = 0) {
    if (!node) return;

    if (auto tu = std::dynamic_pointer_cast<TranslationUnit>(node)) {
        print_indent(level);
        std::cout << "TranslationUnit\n";
        for (auto& stmt : tu->stmts)
            traverse_ast(stmt, level + 1);
    }
    else if (auto cs = std::dynamic_pointer_cast<CompoundStmt>(node)) {
        print_indent(level);
        std::cout << "CompoundStmt\n";
        for (auto& stmt : cs->stmts)
            traverse_ast(stmt, level + 1);
    }
    else if (auto es = std::dynamic_pointer_cast<ExprStmt>(node)) {
        print_indent(level);
        std::cout << "ExprStmt\n";
        traverse_ast(es->expr, level + 1);
    }
    else if (auto ps = std::dynamic_pointer_cast<PrintStmt>(node)) {
        print_indent(level);
        std::cout << "PrintStmt\n";
        traverse_ast(ps->expr, level + 1);
    }
    else if (auto ifs = std::dynamic_pointer_cast<IfStmt>(node)) {
        print_indent(level);
        std::cout << "IfStmt\n";
        print_indent(level+1); std::cout << "Condition:\n";
        traverse_ast(ifs->cond, level + 2);
        print_indent(level+1); std::cout << "Then:\n";
        traverse_ast(ifs->thenStmt, level + 2);
        if (ifs->elseStmt) {
            print_indent(level+1); std::cout << "Else:\n";
            traverse_ast(ifs->elseStmt, level + 2);
        }
    }
    else if (auto ws = std::dynamic_pointer_cast<WhileStmt>(node)) {
        print_indent(level);
        std::cout << "WhileStmt\n";
        print_indent(level+1); std::cout << "Condition:\n";
        traverse_ast(ws->cond, level + 2);
        print_indent(level+1); std::cout << "Body:\n";
        traverse_ast(ws->body, level + 2);
    }
    else if (auto be = std::dynamic_pointer_cast<BinaryExpr>(node)) {
        print_indent(level);
        std::cout << "BinaryExpr: " << be->op << "\n";
        print_indent(level+1); std::cout << "LHS:\n";
        traverse_ast(be->lhs, level + 2);
        print_indent(level+1); std::cout << "RHS:\n";
        traverse_ast(be->rhs, level + 2);
    }
    else if (auto id = std::dynamic_pointer_cast<Identifier>(node)) {
        print_indent(level);
        std::cout << "Identifier: " << id->name << "\n";
    }
    else if (auto c = std::dynamic_pointer_cast<Constant>(node)) {
        print_indent(level);
        std::cout << "Constant: " << c->value << "\n";
    }
    else if (std::dynamic_pointer_cast<StdInGetNum>(node)) {
        print_indent(level);
        std::cout << "StdInGetNum\n";
    }
    else {
        print_indent(level);
        std::cout << "Unknown ASTNode\n";
    }
}

int
main (int argc, char *argv[])
{
    int res = 0;
    driver drv;
    for (int i = 1; i < argc; ++i)
        if (argv[i] == std::string ("-p"))
            drv.trace_parsing = true;
        else if (argv[i] == std::string ("-s"))
            drv.trace_scanning = true;
        else if (!drv.parse (argv[i])) {
            auto root = drv.result;
            traverse_ast(root);
        }
        else
        res = 1;
    return res;
}