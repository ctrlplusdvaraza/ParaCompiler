#pragma once

#include "base_operator_tokens.hpp"
#include "visitor.hpp"

namespace compiler
{

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
