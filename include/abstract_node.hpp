#pragma once

#include <memory>
#include <string>
#include <vector>

namespace compiler
{

class AbstractAstNode;

using AstNode = AbstractAstNode;
using AstNodePtr = std::unique_ptr<AbstractAstNode>;

class AbstractAstNode
{
  public:
    std::vector<AstNodePtr> children;

  public:
    explicit AbstractAstNode() = default;
    explicit AbstractAstNode(std::string lexeme) : lexeme_(lexeme) {}

    AbstractAstNode(const AbstractAstNode& other) = delete;
    AbstractAstNode(AbstractAstNode&& other) = delete;

    AbstractAstNode& operator=(const AbstractAstNode& other) = delete;
    AbstractAstNode& operator=(AbstractAstNode&& other) = delete;

    virtual ~AbstractAstNode() = default;

  public:
    const std::string& get_string_lexeme() const { return lexeme_; }

  public:
    class Visitor;
    virtual void accept(Visitor& visitor) const = 0;

    template <typename T>
    bool is_node_type() const;

  protected:
    std::string lexeme_;
};

}; // namespace compiler
