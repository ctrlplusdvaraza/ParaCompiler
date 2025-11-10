#pragma once

#include "abstract_token.hpp"
#include "visitor.hpp"

namespace compiler
{

template <typename Derived>
class BaseBracketToken : public BaseToken<Derived>
{
  protected:
    using BaseToken<Derived>::BaseToken;

  public:
    virtual bool is_opening() const = 0;
};

class RoundBracketToken final : public BaseBracketToken<RoundBracketToken>
{
  public:
    using BaseBracketToken::BaseBracketToken;

    bool is_opening() const override { return get_string_token() == "("; }
};

class CurlyBracketToken final : public BaseBracketToken<CurlyBracketToken>
{
  public:
    using BaseBracketToken::BaseBracketToken;

    bool is_opening() const override { return get_string_token() == "{"; }
};

}; // namespace compiler
