#pragma once

#include "abstract_token.hpp"

namespace compiler
{

/*----------------------------------------------------------------------------------------------------*/

class OperatorToken : public BaseToken<OperatorToken>
{
  public:
    using AbstractToken::AbstractToken;
};

/*----------------------------------------------------------------------------------------------------*/

class PlusToken final : public OperatorToken
{
  public:
    using OperatorToken::OperatorToken;
};

class MinusToken final : public OperatorToken
{
  public:
    using OperatorToken::OperatorToken;
};

class IncrementToken final : public OperatorToken
{
  public:
    using OperatorToken::OperatorToken;
};

class DecrementToken final : public OperatorToken
{
  public:
    using OperatorToken::OperatorToken;
};

/*----------------------------------------------------------------------------------------------------*/

template <typename T>
class UnaryOperatorToken : public OperatorToken
{
  public:
    using OperatorToken::OperatorToken;

    virtual T calculate(const T& operand) const = 0;
};

template <typename T>
class BinaryOperatorToken : public OperatorToken
{
  public:
    using OperatorToken::OperatorToken;

    virtual T calculate(const T& lhs, const T& rhs) const = 0;
};

/*----------------------------------------------------------------------------------------------------*/

template <typename T>
class PositiveToken final : public UnaryOperatorToken<T>
{
  public:
    using UnaryOperatorToken<T>::UnaryOperatorToken;

    T calculate(const T& operand) const override { return +operand; }
};

template <typename T>
class AddToken final : public BinaryOperatorToken<T>
{
  public:
    using BinaryOperatorToken<T>::BinaryOperatorToken;

    T calculate(const T& lhs, const T& rhs) const { return lhs + rhs; }
};

/*----------------------------------------------------------------------------------------------------*/

template <typename T>
class NegativeToken final : public UnaryOperatorToken<T>
{
  public:
    using UnaryOperatorToken<T>::UnaryOperatorToken;

    T calculate(const T& operand) const override { return -operand; }
};

template <typename T>
class SubToken final : public BinaryOperatorToken<T>
{
  public:
    using BinaryOperatorToken<T>::BinaryOperatorToken;

    T calculate(const T& lhs, const T& rhs) const { return lhs - rhs; }
};

/*----------------------------------------------------------------------------------------------------*/

template <typename T>
class PrefixIncrementToken final : public UnaryOperatorToken<T>
{
  public:
    using UnaryOperatorToken<T>::UnaryOperatorToken;

    T calculate(const T& operand) const { return ++operand; }
};

template <typename T>
class PostfixIncrementToken final : public UnaryOperatorToken<T>
{
  public:
    using UnaryOperatorToken<T>::UnaryOperatorToken;

    T calculate(const T& operand) const { return operand++; }
};

// /*----------------------------------------------------------------------------------------------------*/

template <typename T>
class PrefixDecrementToken final : public UnaryOperatorToken<T>
{
  public:
    using UnaryOperatorToken<T>::UnaryOperatorToken;

    T calculate(const T& operand) const override { return --operand; }
};

template <typename T>
class PostfixDecrementToken final : public UnaryOperatorToken<T>
{
  public:
    using UnaryOperatorToken<T>::UnaryOperatorToken;

    T calculate(const T& operand) const override { return operand--; }
};

/*----------------------------------------------------------------------------------------------------*/

// add binary operators

}; // namespace compiler
