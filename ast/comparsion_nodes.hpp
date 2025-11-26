#pragma once

#include "base_operator_nodes.hpp"

namespace compiler
{

class EqualNode final : public BaseBinaryOperatorNode<EqualNode>
{
  public:
    using BaseBinaryOperatorNode::BaseBinaryOperatorNode;
};

class NotEqualNode final : public BaseBinaryOperatorNode<NotEqualNode>
{
  public:
    using BaseBinaryOperatorNode::BaseBinaryOperatorNode;
};

class LessNode final : public BaseBinaryOperatorNode<LessNode>
{
  public:
    using BaseBinaryOperatorNode::BaseBinaryOperatorNode;
};

class LessEqualNode final : public BaseBinaryOperatorNode<LessEqualNode>
{
  public:
    using BaseBinaryOperatorNode::BaseBinaryOperatorNode;
};

class GreaterNode final : public BaseBinaryOperatorNode<GreaterNode>
{
  public:
    using BaseBinaryOperatorNode::BaseBinaryOperatorNode;
};

class GreaterEqualNode final : public BaseBinaryOperatorNode<GreaterEqualNode>
{
  public:
    using BaseBinaryOperatorNode::BaseBinaryOperatorNode;
};

} // namespace compiler
