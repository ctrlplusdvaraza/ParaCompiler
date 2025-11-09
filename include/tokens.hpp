#pragma once

#include <string>

namespace compiler
{

/*----------------------------------------------------------------------------------------------------*/

class AbstractToken
{
  public:
    AbstractToken(std::string token, std::size_t line, std::size_t line_offset, std::size_t abs_offset)
        : token_(token_), line_(line), line_offset_(line_offset), abs_offset_(abs_offset)
    {}

    virtual ~AbstractToken() = default;

  public:
    const std::string& get_string_token() const
    {
        return token_;
    }

    std::size_t get_line() const
    {
        return line_;
    }

    std::size_t get_line_offset() const
    {
        return line_offset_;
    }

    std::size_t get_abs_offset() const
    {
        return abs_offset_;
    }

  private:
    std::string token_;
    std::size_t line_;
    std::size_t line_offset_;
    std::size_t abs_offset_;
};

/*----------------------------------------------------------------------------------------------------*/

class KeywordToken : public AbstractToken
{
  public:
    using AbstractToken::AbstractToken;
};

class WhileToken final : public KeywordToken
{
  public:
    using KeywordToken::KeywordToken;
};

class IfToken final : public KeywordToken
{
  public:
    using KeywordToken::KeywordToken;
};

class PrintToken final : public KeywordToken
{
  public:
    using KeywordToken::KeywordToken;
};

class InputToken final : public KeywordToken
{
  public:
    using KeywordToken::KeywordToken;
};

/*----------------------------------------------------------------------------------------------------*/

class OperatorToken : public AbstractToken
{
  public:
    using AbstractToken::AbstractToken;
};

template <typename T>
class UnaryOperatorToken : public OperatorToken
{
  public:
    using OperatorToken::OperatorToken;

    virtual T calculate(const T& operand) const = 0;
};

template <typename T>
class PositiveToken final : public UnaryOperatorToken<T>
{
  public:
    using UnaryOperatorToken::UnaryOperatorToken;

    T calculate(const T& operand) const 
    {
        return +operand;
    }
};

template <typename T>
class NegativeToken final : public UnaryOperatorToken
{
  public:
    using UnaryOperatorToken::UnaryOperatorToken;

    T calculate(const T& operand) const 
    {
        return -operand;
    }
};

template <typename T>
class PrefixIncrementToken final : public UnaryOperatorToken
{
  public:
    using UnaryOperatorToken::UnaryOperatorToken;

    T calculate(const T& operand) const 
    {
        return ++operand;
    }
};

template <typename T>
class PostfixIncrementToken final : public UnaryOperatorToken
{
  public:
    using UnaryOperatorToken::UnaryOperatorToken;

    T calculate(const T& operand) const 
    {
        return operand++;
    }
};

template <typename T>
class PrefixDecrementToken final : public UnaryOperatorToken
{
  public:
    using UnaryOperatorToken::UnaryOperatorToken;

    T calculate(const T& operand) const 
    {
        return --operand;
    }
};

template <typename T>
class PostfixDecrementToken final : public UnaryOperatorToken
{
  public:
    using UnaryOperatorToken::UnaryOperatorToken;

    T calculate(const T& operand) const 
    {
        return operand--;
    }
};

template <typename T>
class BinaryOperatorToken : public OperatorToken
{
  public:
    using OperatorToken::OperatorToken;

    virtual T calculate(const T& lhs, const T& rhs) const = 0;
};

// add binary operators

/*----------------------------------------------------------------------------------------------------*/

class BracketToken : public AbstractToken
{
  public:
    using AbstractToken::AbstractToken;

    virtual bool is_opening() const = 0;
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

/*----------------------------------------------------------------------------------------------------*/

class OperandToken : public AbstractToken
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
    LiteralToken(const std::string& token, std::size_t line, std::size_t line_offset, std::size_t abs_offset) : ()
    {}

    LiteralToken(const T& value, std::size_t line, std::size_t line_offset, std::size_t abs_offset) : ()
    {}

  public:
    const T& get_value() const
    {
        return value_;
    }

  private:
    T value_;
};

/*----------------------------------------------------------------------------------------------------*/

}; // namespace compiler
