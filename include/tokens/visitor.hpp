#pragma once

#include "abstract_token.hpp"

namespace compiler
{

class AbstractToken::Visitor
{
  public:
    virtual void visit(const class BracketToken& token) const = 0;
    virtual void visit(const class RoundBracketToken& token) const = 0;
    virtual void visit(const class CurlyBracketToken& token) const = 0;

    virtual void visit(const class KeywordToken& token) const = 0;
    virtual void visit(const class WhileToken& token) const = 0;
    virtual void visit(const class IfToken& token) const = 0;
    virtual void visit(const class PrintToken& token) const = 0;
    virtual void visit(const class InputToken& token) const = 0;

    virtual void visit(const class OperandToken& token) const = 0;
    virtual void visit(const class IdentifierToken& token) const = 0;
    virtual void visit(const class LiteralToken& token) const = 0;

    virtual void visit(const class OperatorToken& token) const = 0;
};

template <typename T>
class TypeCheckVisitor : public AbstractToken::Visitor
{
  public:
    bool is_type() const { return result; }

  public:
    // clang-format off
    void visit(const BracketToken& token) const override      { check_type<BracketToken>(); }
    void visit(const RoundBracketToken& token) const override { check_type<RoundBracketToken>(); }
    void visit(const CurlyBracketToken& token) const override { check_type<CurlyBracketToken>(); }

    void visit(const KeywordToken& token) const override { check_type<KeywordToken>(); }
    void visit(const WhileToken& token) const override   { check_type<WhileToken>(); }
    void visit(const IfToken& token) const override      { check_type<IfToken>(); }
    void visit(const PrintToken& token) const override   { check_type<PrintToken>(); }
    void visit(const InputToken& token) const override   { check_type<InputToken>(); }

    void visit(const OperandToken& token) const override    { check_type<OperandToken>(); }
    void visit(const IdentifierToken& token) const override { check_type<IdentifierToken>(); }
    void visit(const LiteralToken& token) const override    { check_type<LiteralToken>(); }

    void visit(const OperatorToken& token) const override { check_type<OperatorToken>(); }
    // clang-format on

  private:
    template <typename U>
    void check_type() const
    {
        result = std::is_same_v<T, U>;
    }

  private:
    bool result = false;
};

}; // namespace compiler
