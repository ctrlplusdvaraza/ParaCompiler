#pragma once

#include "abstract_token.hpp"
#include "visitor.hpp"

namespace compiler
{

template <typename Derived>
class BaseOperandToken : public BaseToken<Derived>
{
  protected:
    using BaseToken<Derived>::BaseToken;
};

class IdentifierToken final : public BaseOperandToken<IdentifierToken>
{
  public:
    using BaseOperandToken::BaseOperandToken;
};

// template <typename T>
class LiteralToken final : public BaseOperandToken<LiteralToken>
{
  public:
    using BaseOperandToken::BaseOperandToken;

    // LiteralToken(const std::string& token, std::size_t line, std::size_t line_offset,
    //              std::size_t abs_offset)
    //     : ()
    // {
    // }

    // LiteralToken(const T& value, std::size_t line, std::size_t line_offset, std::size_t abs_offset)
    //     : ()
    // {
    // }

//   public:
    // const T& get_value() const { return value_; }

  private:
    // T value_;
};

}; // namespace compiler
