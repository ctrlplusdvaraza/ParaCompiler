#pragma once

#include "abstract_token.hpp"
#include "visitor.hpp"

namespace compiler
{

template <typename Derived>
class BaseKeywordToken : public BaseToken<Derived>
{
  protected:
    using BaseToken<Derived>::BaseToken;
};

class WhileToken final : public BaseKeywordToken<WhileToken>
{
  public:
    using BaseKeywordToken::BaseKeywordToken;
};

class IfToken final : public BaseKeywordToken<IfToken>
{
  public:
    using BaseKeywordToken::BaseKeywordToken;
};

class PrintToken final : public BaseKeywordToken<PrintToken>
{
  public:
    using BaseKeywordToken::BaseKeywordToken;
};

class InputToken final : public BaseKeywordToken<InputToken>
{
  public:
    using BaseKeywordToken::BaseKeywordToken;
};

}; // namespace compiler
