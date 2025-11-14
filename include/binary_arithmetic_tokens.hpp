#pragma once

#include "base_operator_tokens.hpp"
#include "visitor.hpp"

namespace compiler
{

class AddToken final : public BaseBinaryOperatorToken<AddToken>
{
  public:
    using BaseBinaryOperatorToken::BaseBinaryOperatorToken;
};

class SubToken final : public BaseBinaryOperatorToken<SubToken>
{
  public:
    using BaseBinaryOperatorToken::BaseBinaryOperatorToken;
};

class MulToken final : public BaseBinaryOperatorToken<MulToken>
{
  public:
    using BaseBinaryOperatorToken::BaseBinaryOperatorToken;
};

class DivToken final : public BaseBinaryOperatorToken<DivToken>
{
  public:
    using BaseBinaryOperatorToken::BaseBinaryOperatorToken;
};

class ModToken final : public BaseBinaryOperatorToken<ModToken>
{
  public:
    using BaseBinaryOperatorToken::BaseBinaryOperatorToken;
};
    
} // namespace compiler
