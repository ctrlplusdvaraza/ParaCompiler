#pragma once
#include <memory>
#include <string>
#include <vector>

#include "abstract_token.hpp"
#include "tokens.hpp"

namespace compiler
{

struct AstNode;
struct AstRoot;

using AstNodePtr = std::unique_ptr<AstNode>;
using AstRootPtr = std::unique_ptr<AstRoot>;

using AbstractTokenPtr = std::unique_ptr<AbstractToken>;

struct AstNode
{
    explicit AstNode(AbstractTokenPtr token) : token(std::move(token)) {}

    AbstractTokenPtr token;
    std::vector<AstNodePtr> children;
};

struct AstRoot
{
    std::vector<AstNodePtr> children;
};

} // namespace compiler
