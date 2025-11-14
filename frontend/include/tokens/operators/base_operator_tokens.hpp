#pragma once

#include "abstract_token.hpp"
#include "visitor.hpp"

namespace compiler
{

template <typename Derived>
class BaseOperatorToken : public BaseToken<Derived>
{
  protected:
    using BaseToken<Derived>::BaseToken;
};

template <typename Derived>
class BaseUnaryOperatorToken : public BaseOperatorToken<Derived>
{
  protected:
    using BaseOperatorToken<Derived>::BaseOperatorToken;
};

template <typename Derived>
class BaseBinaryOperatorToken : public BaseOperatorToken<Derived>
{
  protected:
    using BaseOperatorToken<Derived>::BaseOperatorToken;
};

} // namespace compiler
