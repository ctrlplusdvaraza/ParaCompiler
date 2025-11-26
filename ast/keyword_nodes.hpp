#pragma once

#include "abstract_node.hpp"
#include "visitor.hpp"

namespace compiler
{

template <typename Derived>
class BaseKeywordNode : public BaseNode<Derived>
{
  protected:
    using BaseNode<Derived>::BaseNode;
};

class WhileNode final : public BaseKeywordNode<WhileNode>
{
  public:
    using BaseKeywordNode::BaseKeywordNode;
};

class IfNode final : public BaseKeywordNode<IfNode>
{
  public:
    using BaseKeywordNode::BaseKeywordNode;
};

class PrintNode final : public BaseKeywordNode<PrintNode>
{
  public:
    using BaseKeywordNode::BaseKeywordNode;
};

class InputNode final : public BaseKeywordNode<InputNode>
{
  public:
    using BaseKeywordNode::BaseKeywordNode;
};

}; // namespace compiler
