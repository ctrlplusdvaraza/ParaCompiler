#pragma once

#include <string>

namespace compiler
{

class AbstractToken
{
  public:
    AbstractToken(std::string token, std::size_t line, std::size_t line_offset,
                  std::size_t abs_offset)
        : token_(token_), line_(line), line_offset_(line_offset), abs_offset_(abs_offset)
    {
    }

    virtual ~AbstractToken() = default;

  public:
    const std::string& get_string_token() const { return token_; }
    std::size_t get_line() const { return line_; }
    std::size_t get_line_offset() const { return line_offset_; }
    std::size_t get_abs_offset() const { return abs_offset_; }

  public:
    class Visitor;
    virtual void accept(const Visitor& visitor) const = 0;

  private:
    std::string token_;
    std::size_t line_;
    std::size_t line_offset_;
    std::size_t abs_offset_;
};

template <typename Derived>
class BaseToken : public AbstractToken
{
    void accept(const Visitor& visitor) const override
    {
        visitor.visit(static_cast<Derived&>(*this));
    }
};

}; // namespace compiler
