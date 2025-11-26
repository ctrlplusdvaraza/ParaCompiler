#pragma once

#include "visitor.hpp"

namespace compiler 
{

class TranslationUnitNode final : public BaseNode<TranslationUnitNode>
{
  public:
    using BaseNode<TranslationUnitNode>::BaseNode;

    void set_file_name(std::string file_name)
    {
        lexeme_ = file_name;
    }
};

using AstRoot = TranslationUnitNode;
using AstRootPtr = std::unique_ptr<AstRoot>;

} // namespace compiler
