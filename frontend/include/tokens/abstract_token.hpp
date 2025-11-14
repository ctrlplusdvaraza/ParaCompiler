#pragma once

#include <iostream>
#include <string>

namespace compiler
{

class AbstractToken
{
  public:
    AbstractToken(std::string token) : token_(token) {}

    AbstractToken(const AbstractToken& other) = delete;
    AbstractToken(AbstractToken&& other) = delete;

    AbstractToken& operator=(const AbstractToken& other) = delete;
    AbstractToken& operator=(AbstractToken&& other) = delete;

    virtual ~AbstractToken() = default;

  public:
    const std::string& get_string_token() const { return token_; }

  public:
    class Visitor;
    virtual void accept(Visitor& visitor) = 0;

    template <typename T>
    bool is_token_of_type();

  private:
    std::string token_;
};

}; // namespace compiler
