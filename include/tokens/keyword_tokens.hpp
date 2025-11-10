#pragma once

#include "abstract_token.hpp"

namespace compiler
{

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

}; //namespace compiler
