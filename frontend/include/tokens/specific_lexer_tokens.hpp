#pragma once

#include "base_operator_tokens.hpp"
#include "visitor.hpp"

namespace compiler
{

class SemicolonToken final : public BaseToken<SemicolonToken>
{
  public:
    using BaseToken::BaseToken;
};

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

class StarToken final : public BaseOperatorToken<StarToken>
{
  public:
    using BaseOperatorToken::BaseOperatorToken;
};

class SlashToken final : public BaseOperatorToken<SlashToken>
{
  public:
    using BaseOperatorToken::BaseOperatorToken;
};

class PercentToken final : public BaseOperatorToken<PercentToken>
{
  public:
    using BaseOperatorToken::BaseOperatorToken;
};

class PlusPlusToken final : public BaseOperatorToken<PlusPlusToken>
{
  public:
    using BaseOperatorToken::BaseOperatorToken;
};

class MinusMinusToken final : public BaseOperatorToken<MinusMinusToken>
{
  public:
    using BaseOperatorToken::BaseOperatorToken;
};

} // namespace compiler
