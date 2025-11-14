#pragma once

#include "base_operator_tokens.hpp"
#include "visitor.hpp"

namespace compiler
{

class UnaryPlusToken final : public BaseUnaryOperatorToken<UnaryPlusToken>
{
  public:
    using BaseUnaryOperatorToken::BaseUnaryOperatorToken;
};

class UnaryMinusToken final : public BaseUnaryOperatorToken<UnaryMinusToken>
{
  public:
    using BaseUnaryOperatorToken::BaseUnaryOperatorToken;
};

class PrefixIncrementToken final : public BaseUnaryOperatorToken<PrefixIncrementToken>
{
  public:
    using BaseUnaryOperatorToken::BaseUnaryOperatorToken;
};

class PostfixIncrementToken final : public BaseUnaryOperatorToken<PostfixIncrementToken>
{
  public:
    using BaseUnaryOperatorToken::BaseUnaryOperatorToken;
};

class PrefixDecrementToken final : public BaseUnaryOperatorToken<PrefixDecrementToken>
{
  public:
    using BaseUnaryOperatorToken::BaseUnaryOperatorToken;
};

class PostfixDecrementToken final : public BaseUnaryOperatorToken<PostfixDecrementToken>
{
  public:
    using BaseUnaryOperatorToken::BaseUnaryOperatorToken;
};
    
} // namespace compiler
