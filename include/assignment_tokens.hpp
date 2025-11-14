#pragma once

#include "base_operator_tokens.hpp"

namespace compiler
{

class AssignmentToken final : public BaseBinaryOperatorToken<AssignmentToken>
{
  public:
    using BaseBinaryOperatorToken::BaseBinaryOperatorToken;
};

class AddAssignmentToken final : public BaseBinaryOperatorToken<AddAssignmentToken>
{
  public:
    using BaseBinaryOperatorToken::BaseBinaryOperatorToken;
};

class SubAssignmentToken final : public BaseBinaryOperatorToken<SubAssignmentToken>
{
  public:
    using BaseBinaryOperatorToken::BaseBinaryOperatorToken;
};

class MulAssignmentToken final : public BaseBinaryOperatorToken<MulAssignmentToken>
{
  public:
    using BaseBinaryOperatorToken::BaseBinaryOperatorToken;
};

class DivAssignmentToken final : public BaseBinaryOperatorToken<DivAssignmentToken>
{
  public:
    using BaseBinaryOperatorToken::BaseBinaryOperatorToken;
};

class ModAssignmentToken final : public BaseBinaryOperatorToken<ModAssignmentToken>
{
  public:
    using BaseBinaryOperatorToken::BaseBinaryOperatorToken;
};

} // namespace compiler
