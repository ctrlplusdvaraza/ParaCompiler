#pragma once

#include <string>

namespace compiler
{

class AbstractToken
{
  public:
    virtual ~AbstractToken() = default;

  private:
    std::string token_;
    std::size_t line;
    std::size_t line_offset;
    std::size_t abs_offset;
};

class KeywordToken : public AbstractToken
{
};

class OperatorToken : public AbstractToken
{
};

class BracketToken : public AbstractToken
{
};

class ScopeToken : public AbstractToken
{
};

class IdentifierToken : public AbstractToken
{
};

class LiteralToken : public AbstractToken
{
};

}; // namespace compiler
