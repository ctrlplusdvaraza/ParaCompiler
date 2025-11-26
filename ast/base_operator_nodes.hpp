#pragma once

#include "abstract_node.hpp"
#include "visitor.hpp"

namespace compiler
{

template <typename Derived>
class BaseOperatorNode : public BaseNode<Derived>
{
  public:
    using BaseNode<Derived>::BaseNode;
};

template <typename Derived>
class BaseUnaryOperatorNode : public BaseOperatorNode<Derived>
{
  public:
    using BaseOperatorNode<Derived>::BaseOperatorNode;
};

template <typename Derived>
class BaseBinaryOperatorNode : public BaseOperatorNode<Derived>
{
  public:
    using BaseOperatorNode<Derived>::BaseOperatorNode;
};

} // namespace compiler
