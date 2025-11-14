#pragma once

#include "base_operator_tokens.hpp"

namespace compiler
{

class AssignmentToken final : public BaseBinaryOperatorToken<AssignmentToken>
{
  public:
    using BaseBinaryOperatorToken<AssignmentToken>::BaseBinaryOperatorToken;
};

class AddAssignmentToken final : public BaseBinaryOperatorToken<AddAssignmentToken>
{
  public:
    using BaseBinaryOperatorToken<AddAssignmentToken>::BaseBinaryOperatorToken;
};

class SubAssignmentToken final : public BaseBinaryOperatorToken<SubAssignmentToken>
{
  public:
    using BaseBinaryOperatorToken<SubAssignmentToken>::BaseBinaryOperatorToken;
};

class MulAssignmentToken final : public BaseBinaryOperatorToken<MulAssignmentToken>
{
  public:
    using BaseBinaryOperatorToken<MulAssignmentToken>::BaseBinaryOperatorToken;
};

class DivAssignmentToken final : public BaseBinaryOperatorToken<DivAssignmentToken>
{
  public:
    using BaseBinaryOperatorToken<DivAssignmentToken>::BaseBinaryOperatorToken;
};

class ModAssignmentToken final : public BaseBinaryOperatorToken<ModAssignmentToken>
{
  public:
    using BaseBinaryOperatorToken<ModAssignmentToken>::BaseBinaryOperatorToken;
};

} // namespace compiler
