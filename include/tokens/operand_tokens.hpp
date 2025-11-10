#pragma once

#include "abstract_token.hpp"

namespace compiler
{

class OperandToken : public BaseToken<OperandToken>
{
  public:
    using AbstractToken::AbstractToken;
};

class IdentifierToken final : public OperandToken
{
  public:
    using OperandToken::OperandToken;
};

template <typename T>
class LiteralToken final : public OperandToken
{
  public:
    LiteralToken(const std::string& token, std::size_t line, std::size_t line_offset,
                 std::size_t abs_offset)
        : ()
    {
    }

    LiteralToken(const T& value, std::size_t line, std::size_t line_offset, std::size_t abs_offset)
        : ()
    {
    }

  public:
    const T& get_value() const { return value_; }

  private:
    T value_;
};

}; // namespace compiler
