#pragma once

#include "base_operator_nodes.hpp"
#include "visitor.hpp"

namespace compiler
{

class UnaryPlusNode final : public BaseUnaryOperatorNode<UnaryPlusNode>
{
  public:
    using BaseUnaryOperatorNode::BaseUnaryOperatorNode;
};

class UnaryMinusNode final : public BaseUnaryOperatorNode<UnaryMinusNode>
{
  public:
    using BaseUnaryOperatorNode::BaseUnaryOperatorNode;
};

class PrefixIncrementNode final : public BaseUnaryOperatorNode<PrefixIncrementNode>
{
  public:
    using BaseUnaryOperatorNode::BaseUnaryOperatorNode;
};

class PostfixIncrementNode final : public BaseUnaryOperatorNode<PostfixIncrementNode>
{
  public:
    using BaseUnaryOperatorNode::BaseUnaryOperatorNode;
};

class PrefixDecrementNode final : public BaseUnaryOperatorNode<PrefixDecrementNode>
{
  public:
    using BaseUnaryOperatorNode::BaseUnaryOperatorNode;
};

class PostfixDecrementNode final : public BaseUnaryOperatorNode<PostfixDecrementNode>
{
  public:
    using BaseUnaryOperatorNode::BaseUnaryOperatorNode;
};
    
} // namespace compiler
