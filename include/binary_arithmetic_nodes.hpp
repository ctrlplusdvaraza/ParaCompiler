#pragma once

#include "base_operator_nodes.hpp"
#include "visitor.hpp"

namespace compiler
{

class AddNode final : public BaseBinaryOperatorNode<AddNode>
{
  public:
    using BaseBinaryOperatorNode::BaseBinaryOperatorNode;
};

class SubNode final : public BaseBinaryOperatorNode<SubNode>
{
  public:
    using BaseBinaryOperatorNode::BaseBinaryOperatorNode;
};

class MulNode final : public BaseBinaryOperatorNode<MulNode>
{
  public:
    using BaseBinaryOperatorNode::BaseBinaryOperatorNode;
};

class DivNode final : public BaseBinaryOperatorNode<DivNode>
{
  public:
    using BaseBinaryOperatorNode::BaseBinaryOperatorNode;
};

class ModNode final : public BaseBinaryOperatorNode<ModNode>
{
  public:
    using BaseBinaryOperatorNode::BaseBinaryOperatorNode;
};
    
} // namespace compiler
