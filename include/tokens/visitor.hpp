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

    virtual void visit(class KeywordToken& token) = 0;
    virtual void visit(class OperatorToken& token) = 0;
};

template <typename T>
class TypeCheckVisitor : public AbstractToken::Visitor
{
  public:
    bool is_type() const
    {
        return result;
    }

  public:
    void visit(const BracketToken& token) const override
    {
        check_type<BracketToken>();
    }

    void visit(const RoundBracketToken& token) const override
    {
        check_type<RoundBracketToken>();
    }

    void visit(const CurlyBracketToken& token) const override
    {
        check_type<CurlyBracketToken>();
    }

    // void visit(const KeywordToken& token) const override
    // {
    //     result = std::is_same_v<T, KeywordToken>;
    // }

    // void visit(const OperatorToken& token) const override
    // {
    //     result = std::is_same_v<T, OperatorToken>;
    // }

    // void visit(const PlusToken& token) const override
    // {
    //     result = std::is_same_v<T, PlusToken>;
    // }

    // void visit(const MinusToken& token) const override
    // {
    //     result = std::is_same_v<T, MinusToken>;
    // }

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
