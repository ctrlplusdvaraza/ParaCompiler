#pragma once

#include "abstract_token.hpp"
#include "visitor.hpp"

namespace compiler
{

class BracketToken : public AbstractToken
{
  public:
    using AbstractToken::AbstractToken;

    virtual bool is_opening() const = 0;

  public:
    void accept(const Visitor& visitor) const
    {
        visitor.visit(*this);
    }
};

class RoundBracketToken final : public BracketToken
{
  public:
    using BracketToken::BracketToken;

    bool is_opening() const override
    {
        return get_string_token() == "(";
    }
};

class CurlyBracketToken final : public BracketToken
{
  public:
    using BracketToken::BracketToken;

    bool is_opening() const override
    {
        return get_string_token() == "{";
    }
};

}; // namespace compiler
