#pragma once

#include "abstract_token.hpp"

namespace compiler
{

class BracketToken : public BaseToken<BracketToken>
{
  public:
    using AbstractToken::AbstractToken;

    virtual bool is_opening() const = 0;
};

class RoundBracketToken final : public BracketToken
{
  public:
    using BracketToken::BracketToken;

    bool is_opening() const override { return get_string_token() == "("; }
};

class CurlyBracketToken final : public BracketToken
{
  public:
    using BracketToken::BracketToken;

    bool is_opening() const override { return get_string_token() == "{"; }
};

}; // namespace compiler
