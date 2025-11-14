#pragma once

#include "abstract_token.hpp"
#include "visitor.hpp"

namespace compiler
{

template <typename Derived>
class BaseOperatorToken : public BaseToken<Derived>
{
  public:
    using BaseToken<Derived>::BaseToken;
};

template <typename Derived>
class BaseUnaryOperatorToken : public BaseOperatorToken<Derived>
{
  public:
    using BaseOperatorToken<Derived>::BaseOperatorToken;
};

template <typename Derived>
class BaseBinaryOperatorToken : public BaseOperatorToken<Derived>
{
  public:
    using BaseOperatorToken<Derived>::BaseOperatorToken;
};

} // namespace compiler
