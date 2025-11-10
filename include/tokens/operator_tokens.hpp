#pragma once

#include "abstract_token.hpp"

namespace compiler
{

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
    
}; //namespace compiler
