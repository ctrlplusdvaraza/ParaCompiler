#pragma once

#include "visitor.hpp"

namespace compiler 
{

class ScopeNode final : public BaseNode<ScopeNode>
{
  public:
    using BaseNode<ScopeNode>::BaseNode;
};

} // namespace compiler
