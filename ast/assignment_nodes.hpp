#pragma once

#include "base_operator_nodes.hpp"

namespace compiler
{

class AssignmentNode final : public BaseBinaryOperatorNode<AssignmentNode>
{
  public:
    using BaseBinaryOperatorNode::BaseBinaryOperatorNode;
};

class AddAssignmentNode final : public BaseBinaryOperatorNode<AddAssignmentNode>
{
  public:
    using BaseBinaryOperatorNode::BaseBinaryOperatorNode;
};

class SubAssignmentNode final : public BaseBinaryOperatorNode<SubAssignmentNode>
{
  public:
    using BaseBinaryOperatorNode::BaseBinaryOperatorNode;
};

class MulAssignmentNode final : public BaseBinaryOperatorNode<MulAssignmentNode>
{
  public:
    using BaseBinaryOperatorNode::BaseBinaryOperatorNode;
};

class DivAssignmentNode final : public BaseBinaryOperatorNode<DivAssignmentNode>
{
  public:
    using BaseBinaryOperatorNode::BaseBinaryOperatorNode;
};

class ModAssignmentNode final : public BaseBinaryOperatorNode<ModAssignmentNode>
{
  public:
    using BaseBinaryOperatorNode::BaseBinaryOperatorNode;
};

} // namespace compiler
