#pragma once

#include "base_operator_tokens.hpp"

namespace compiler
{

class EqualToken final : public BaseBinaryOperatorToken<EqualToken>
{
  public:
    using BaseBinaryOperatorToken::BaseBinaryOperatorToken;
};

class NotEqualToken final : public BaseBinaryOperatorToken<NotEqualToken>
{
  public:
    using BaseBinaryOperatorToken::BaseBinaryOperatorToken;
};

class LessToken final : public BaseBinaryOperatorToken<LessToken>
{
  public:
    using BaseBinaryOperatorToken::BaseBinaryOperatorToken;
};

class LessEqualToken final : public BaseBinaryOperatorToken<LessEqualToken>
{
  public:
    using BaseBinaryOperatorToken::BaseBinaryOperatorToken;
};

class GreaterToken final : public BaseBinaryOperatorToken<GreaterToken>
{
  public:
    using BaseBinaryOperatorToken::BaseBinaryOperatorToken;
};

class GreaterEqualToken final : public BaseBinaryOperatorToken<GreaterEqualToken>
{
  public:
    using BaseBinaryOperatorToken::BaseBinaryOperatorToken;
};

} // namespace compiler
