#pragma once

#include "base_operator_nodes.hpp"

namespace compiler
{

class NotNode final : public BaseUnaryOperatorNode<NotNode>
{
  public:
    using BaseUnaryOperatorNode::BaseUnaryOperatorNode;
};

class AndNode final : public BaseBinaryOperatorNode<AndNode>
{
  public:
    using BaseBinaryOperatorNode::BaseBinaryOperatorNode;
};

class OrNode final : public BaseBinaryOperatorNode<OrNode>
{
  public:
    using BaseBinaryOperatorNode::BaseBinaryOperatorNode;
};

} // namespace compiler
