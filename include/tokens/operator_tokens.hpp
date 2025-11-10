#pragma once

#include "abstract_token.hpp"
#include "visitor.hpp"

namespace compiler
{

/*----------------------------------------------------------------------------------------------------*/

template <typename Derived>
class BaseOperatorToken : public BaseToken<Derived>
{
  protected:
    using BaseToken<Derived>::BaseToken;
};

/*----------------------------------------------------------------------------------------------------*/

class PlusToken final : public BaseOperatorToken<PlusToken>
{
  public:
    using BaseOperatorToken::BaseOperatorToken;
};

class MinusToken final : public BaseOperatorToken<MinusToken>
{
  public:
    using BaseOperatorToken::BaseOperatorToken;
};

class IncrementToken final : public BaseOperatorToken<IncrementToken>
{
  public:
    using BaseOperatorToken::BaseOperatorToken;
};

class DecrementToken final : public BaseOperatorToken<DecrementToken>
{
  public:
    using BaseOperatorToken::BaseOperatorToken;
};

/*----------------------------------------------------------------------------------------------------*/

template <typename Derived, typename T>
class BaseUnaryOperatorToken : public BaseOperatorToken<Derived>
{
  protected:
    using BaseOperatorToken<Derived>::BaseOperatorToken;

    virtual T calculate(const T& operand) const = 0;
};

template <typename Derived, typename T>
class BaseBinaryOperatorToken : public BaseOperatorToken<Derived>
{
  protected:
    using BaseOperatorToken<Derived>::BaseOperatorToken;

    virtual T calculate(const T& lhs, const T& rhs) const = 0;
};

/*----------------------------------------------------------------------------------------------------*/

template <typename T>
class PositiveToken final : public BaseUnaryOperatorToken<PositiveToken<T>, T>
{
  public:
    using BaseUnaryOperatorToken<PositiveToken<T>, T>::BaseUnaryOperatorToken;

    T calculate(const T& operand) const override { return +operand; }
};

template <typename T>
class AddToken final : public BaseBinaryOperatorToken<AddToken<T>, T>
{
  public:
    using BaseBinaryOperatorToken<AddToken<T>, T>::BaseBinaryOperatorToken;

    T calculate(const T& lhs, const T& rhs) const { return lhs + rhs; }
};

/*----------------------------------------------------------------------------------------------------*/

template <typename T>
class NegativeToken final : public BaseUnaryOperatorToken<NegativeToken<T>, T>
{
  public:
    using BaseUnaryOperatorToken<NegativeToken<T>, T>::BaseUnaryOperatorToken;

    T calculate(const T& operand) const override { return -operand; }
};

template <typename T>
class SubToken final : public BaseBinaryOperatorToken<SubToken<T>, T>
{
  public:
    using BaseBinaryOperatorToken<SubToken<T>, T>::BaseBinaryOperatorToken;

    T calculate(const T& lhs, const T& rhs) const { return lhs - rhs; }
};

/*----------------------------------------------------------------------------------------------------*/

template <typename T>
class PrefixIncrementToken final : public BaseUnaryOperatorToken<PrefixIncrementToken<T>, T>
{
  public:
    using BaseUnaryOperatorToken<PrefixIncrementToken<T>, T>::BaseUnaryOperatorToken;

    T calculate(const T& operand) const { return ++operand; }
};

template <typename T>
class PostfixIncrementToken final : public BaseUnaryOperatorToken<PostfixIncrementToken<T>, T>
{
  public:
    using BaseUnaryOperatorToken<PostfixIncrementToken<T>, T>::BaseUnaryOperatorToken;

    T calculate(const T& operand) const { return operand++; }
};

// /*----------------------------------------------------------------------------------------------------*/

template <typename T>
class PrefixDecrementToken final : public BaseUnaryOperatorToken<PrefixDecrementToken<T>, T>
{
  public:
    using BaseUnaryOperatorToken<PrefixDecrementToken<T>, T>::BaseUnaryOperatorToken;

    T calculate(const T& operand) const override { return --operand; }
};

template <typename T>
class PostfixDecrementToken final : public BaseUnaryOperatorToken<PostfixDecrementToken<T>, T>
{
  public:
    using BaseUnaryOperatorToken<PostfixDecrementToken<T>, T>::BaseUnaryOperatorToken;

    T calculate(const T& operand) const override { return operand--; }
};

/*----------------------------------------------------------------------------------------------------*/

// add binary operators

}; // namespace compiler
