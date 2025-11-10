#pragma once

#include <type_traits>

#include "abstract_token.hpp"

namespace compiler
{

class AbstractToken::Visitor
{
  public:
    virtual void visit(const class RoundBracketToken& token) = 0;
    virtual void visit(const class CurlyBracketToken& token) = 0;

    virtual void visit(const class WhileToken& token) = 0;
    virtual void visit(const class IfToken& token) = 0;
    virtual void visit(const class PrintToken& token) = 0;
    virtual void visit(const class InputToken& token) = 0;

    virtual void visit(const class IdentifierToken& token) = 0;
    virtual void visit(const class LiteralToken& token) = 0;

    virtual void visit(const class PlusToken& token) = 0;
    virtual void visit(const class PositiveToken& token) = 0;
    virtual void visit(const class AddToken& token) = 0;
};

template <typename Derived>
class BaseToken : public AbstractToken
{
  public:
    using AbstractToken::AbstractToken;

  public:
    virtual void accept(Visitor& visitor) override { visitor.visit(static_cast<Derived&>(*this)); }
};

template <typename T>
class TypeCheckVisitor : public AbstractToken::Visitor
{
  public:
    bool is_type() const { return result_; }

  public:
    // clang-format off
    void visit(const RoundBracketToken& token) override { check_type<RoundBracketToken>(); }
    void visit(const CurlyBracketToken& token) override { check_type<CurlyBracketToken>(); }

    void visit(const WhileToken& token) override        { check_type<WhileToken>(); }
    void visit(const IfToken& token)    override        { check_type<IfToken>(); }
    void visit(const PrintToken& token) override        { check_type<PrintToken>(); }
    void visit(const InputToken& token) override        { check_type<InputToken>(); }

    void visit(const IdentifierToken& token) override   { check_type<IdentifierToken>(); }
    void visit(const LiteralToken& token)    override   { check_type<LiteralToken>(); }

    void visit(const PlusToken& token)     override     { check_type<PlusToken>(); }
    void visit(const PositiveToken& token) override     { check_type<PositiveToken>(); }
    void visit(const AddToken& token)      override     { check_type<AddToken>(); }
    // clang-format on

  private:
    template <typename U>
    void check_type()
    {
        result_ = std::is_same_v<T, U>;
    }

  private:
    bool result_ = false;
};

template <typename T>
bool AbstractToken::is_token_of_type()
{
    TypeCheckVisitor<T> visitor;
    accept(visitor);
    return visitor.is_type();
}

}; // namespace compiler
