#pragma once

#include "abstract_node.hpp"
#include "visitor.hpp"

namespace compiler
{

template <typename Derived>
class BaseOperandNode : public BaseNode<Derived>
{
  public:
    using BaseNode<Derived>::BaseNode;
};

class IdentifierNode final : public BaseOperandNode<IdentifierNode>
{
  public:
    using BaseOperandNode::BaseOperandNode;
};

class LiteralNode final : public BaseOperandNode<LiteralNode>
{
  public:
    using BaseOperandNode::BaseOperandNode;
};

}; // namespace compiler
