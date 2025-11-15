#pragma once

#include "visitor.hpp"

namespace compiler 
{

class TranslationUnitNode final : public BaseNode<TranslationUnitNode>
{
  public:
    using BaseNode<TranslationUnitNode>::BaseNode;
};

} // namespace compiler
