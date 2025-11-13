#pragma once

#include "abstract_token.hpp"
#include "visitor.hpp"

namespace compiler
{

/*----------------------------------------------------------------------------------------------------*/

template <typename Derived>
class BaseOperatorToken : public BaseToken<Derived>
{
  protected:
    using BaseToken<Derived>::BaseToken;
};

/*----------------------------------------------------------------------------------------------------*/

class PlusToken final : public BaseOperatorToken<PlusToken>
{
  public:
    using BaseOperatorToken::BaseOperatorToken;
};

class MinusToken final : public BaseOperatorToken<MinusToken>
{
  public:
    using BaseOperatorToken::BaseOperatorToken;
};

class IncrementToken final : public BaseOperatorToken<IncrementToken>
{
  public:
    using BaseOperatorToken::BaseOperatorToken;
};

class DecrementToken final : public BaseOperatorToken<DecrementToken>
{
  public:
    using BaseOperatorToken::BaseOperatorToken;
};

/*----------------------------------------------------------------------------------------------------*/

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

/*----------------------------------------------------------------------------------------------------*/

class PositiveToken final : public BaseUnaryOperatorToken<PositiveToken>
{
  public:
    using BaseUnaryOperatorToken<PositiveToken>::BaseUnaryOperatorToken;
};

class AddToken final : public BaseBinaryOperatorToken<AddToken>
{
  public:
    using BaseBinaryOperatorToken<AddToken>::BaseBinaryOperatorToken;
};

/*----------------------------------------------------------------------------------------------------*/

class NegativeToken final : public BaseUnaryOperatorToken<NegativeToken>
{
  public:
    using BaseUnaryOperatorToken<NegativeToken>::BaseUnaryOperatorToken;
};

class SubToken final : public BaseBinaryOperatorToken<SubToken>
{
  public:
    using BaseBinaryOperatorToken<SubToken>::BaseBinaryOperatorToken;
};

/*----------------------------------------------------------------------------------------------------*/

class PrefixIncrementToken final : public BaseUnaryOperatorToken<PrefixIncrementToken>
{
  public:
    using BaseUnaryOperatorToken<PrefixIncrementToken>::BaseUnaryOperatorToken;
};

class PostfixIncrementToken final : public BaseUnaryOperatorToken<PostfixIncrementToken>
{
  public:
    using BaseUnaryOperatorToken<PostfixIncrementToken>::BaseUnaryOperatorToken;
};

// /*----------------------------------------------------------------------------------------------------*/

class PrefixDecrementToken final : public BaseUnaryOperatorToken<PrefixDecrementToken>
{
  public:
    using BaseUnaryOperatorToken<PrefixDecrementToken>::BaseUnaryOperatorToken;
};

class PostfixDecrementToken final : public BaseUnaryOperatorToken<PostfixDecrementToken>
{
  public:
    using BaseUnaryOperatorToken<PostfixDecrementToken>::BaseUnaryOperatorToken;
};

/*----------------------------------------------------------------------------------------------------*/

// add binary operators

}; // namespace compiler
