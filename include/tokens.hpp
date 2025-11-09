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
};

class WhileToken final : public KeywordToken
{
};

class IfToken final : public KeywordToken
{
};

class PrintToken final : public KeywordToken
{
};

class InputToken final : public KeywordToken
{
};

/*----------------------------------------------------------------------------------------------------*/

class OperatorToken : public AbstractToken
{
};

class UnaryOperatorToken : public OperatorToken
{
};

class PositiveToken final : public UnaryOperatorToken
{
  public:
    using AbstractToken::AbstractToken;
};

class NegativeToken final : public UnaryOperatorToken
{
  public:
    using AbstractToken::AbstractToken;
};

class PrefixIncrementToken final : public UnaryOperatorToken
{
  public:
    using AbstractToken::AbstractToken;
};

class PostfixIncrementToken final : public UnaryOperatorToken
{
  public:
    using AbstractToken::AbstractToken;
};

class PrefixDecrementToken final : public UnaryOperatorToken
{
  public:
    using AbstractToken::AbstractToken;
};

class PostfixDecrementToken final : public UnaryOperatorToken
{
  public:
    using AbstractToken::AbstractToken;
};

class BinaryOperatorToken : public OperatorToken
{
};

// add binary operators

/*----------------------------------------------------------------------------------------------------*/

class BracketToken : public AbstractToken
{
  public:
    virtual bool is_opening() const = 0;
};

class RoundBracketToken final : public BracketToken
{
  public:
    using AbstractToken::AbstractToken;

    bool is_opening() const override
    {
        return get_string_token() == "(";
    }
};

class CurlyBracketToken final : public BracketToken
{
  public:
    using AbstractToken::AbstractToken;

    bool is_opening() const override
    {
        return get_string_token() == "{";
    }
};

/*----------------------------------------------------------------------------------------------------*/

class OperandToken : public AbstractToken
{
};

class IdentifierToken final : public OperandToken
{
  public:
    using AbstractToken::AbstractToken;
};

template <typename T>
class LiteralToken final : public OperandToken
{
  public:
    using AbstractToken::AbstractToken;

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
