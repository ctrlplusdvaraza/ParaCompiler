#pragma once
#include <memory>
#include <string>
#include <vector>

struct ASTNode {
    virtual ~ASTNode() = default;
};

struct Expression : ASTNode {};

struct Statement : ASTNode {};

struct TranslationUnit : ASTNode {
    std::vector<std::shared_ptr<Statement>> stmts;
};

struct CompoundStmt : Statement {
    std::vector<std::shared_ptr<Statement>> stmts;
};

struct ExprStmt : Statement {
    std::shared_ptr<Expression> expr; // nullptr if empty
};

struct PrintStmt : Statement {
    std::shared_ptr<Expression> expr;
};

struct IfStmt : Statement {
    std::shared_ptr<Expression> cond;
    std::shared_ptr<Statement> thenStmt;
    std::shared_ptr<Statement> elseStmt; // nullptr if no else
};

struct WhileStmt : Statement {
    std::shared_ptr<Expression> cond;
    std::shared_ptr<Statement> body;
};

struct BinaryExpr : Expression {
    std::string op;
    std::shared_ptr<Expression> lhs;
    std::shared_ptr<Expression> rhs;
};

struct Identifier : Expression {
    std::string name;
    explicit Identifier(const std::string& n) : name(n) {}
};

struct Constant : Expression {
    int value;

    explicit Constant(int val): value(val) {}
};

struct StdInGetNum : Expression {};

