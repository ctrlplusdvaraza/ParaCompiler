#pragma once

#include <memory>
#include <string>
#include <vector>

namespace compiler
{

class AbstractAstNode;

using AstNodePtr = std::unique_ptr<AbstractAstNode>;

class AbstractAstNode
{
  public:
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
    virtual void accept(Visitor& visitor) = 0;

    template <typename T>
    bool is_node_type();

  private:
    std::string lexeme_;
    std::vector<AstNodePtr> children;
};

}; // namespace compiler
