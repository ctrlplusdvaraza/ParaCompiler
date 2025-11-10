#pragma once

#include "abstract_token.hpp"
#include "visitor.hpp"

namespace compiler
{

template <typename Derived>
class BaseOperandToken : public BaseToken<Derived>
{
  protected:
    using BaseToken<Derived>::BaseToken;
};

class IdentifierToken final : public BaseOperandToken<IdentifierToken>
{
  public:
    using BaseOperandToken::BaseOperandToken;
};

class LiteralToken final : public BaseOperandToken<LiteralToken>
{
  public:
    using BaseOperandToken::BaseOperandToken;
};

}; // namespace compiler
