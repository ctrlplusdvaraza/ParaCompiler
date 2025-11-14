#pragma once

#include <type_traits>

#include "abstract_token.hpp"

namespace compiler
{

class AbstractToken::Visitor
{
  public:
    virtual void visit(const class WhileToken& token) = 0;
    virtual void visit(const class IfToken& token) = 0;
    virtual void visit(const class PrintToken& token) = 0;
    virtual void visit(const class InputToken& token) = 0;

    virtual void visit(const class IdentifierToken& token) = 0;
    virtual void visit(const class LiteralToken& token) = 0;

    virtual void visit(const class RoundBracketToken& token) = 0;
    virtual void visit(const class CurlyBracketToken& token) = 0;

    virtual void visit(const class SemicolonToken& token) = 0;
    virtual void visit(const class PlusToken& token) = 0;
    virtual void visit(const class MinusToken& token) = 0;
    virtual void visit(const class PlusPlusToken& token) = 0;
    virtual void visit(const class MinusMinusToken& token) = 0;

    virtual void visit(const class AssignmentToken& token) = 0;
    virtual void visit(const class AddAssignmentToken& token) = 0;
    virtual void visit(const class SubAssignmentToken& token) = 0;
    virtual void visit(const class MulAssignmentToken& token) = 0;
    virtual void visit(const class DivAssignmentToken& token) = 0;
    virtual void visit(const class ModAssignmentToken& token) = 0;

    virtual void visit(const class PositiveToken& token) = 0;
    virtual void visit(const class NegativeToken& token) = 0;

    virtual void visit(const class PrefixIncrementToken& token) = 0;
    virtual void visit(const class PostfixIncrementToken& token) = 0;
    virtual void visit(const class PrefixDecrementToken& token) = 0;
    virtual void visit(const class PostfixDecrementToken& token) = 0;

    virtual void visit(const class AddToken& token) = 0;
    virtual void visit(const class SubToken& token) = 0;
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
    void visit(const WhileToken& token)            override { check_type<WhileToken>(); }
    void visit(const IfToken& token)               override { check_type<IfToken>(); }
    void visit(const PrintToken& token)            override { check_type<PrintToken>(); }
    void visit(const InputToken& token)            override { check_type<InputToken>(); }

    void visit(const IdentifierToken& token)       override { check_type<IdentifierToken>(); }
    void visit(const LiteralToken& token)          override { check_type<LiteralToken>(); }

    void visit(const RoundBracketToken& token)     override { check_type<RoundBracketToken>(); }
    void visit(const CurlyBracketToken& token)     override { check_type<CurlyBracketToken>(); }

    void visit(const SemicolonToken& token)        override { check_type<SemicolonToken>(); }
    void visit(const PlusToken& token)             override { check_type<PlusToken>(); }
    void visit(const MinusToken& token)            override { check_type<MinusToken>(); }
    void visit(const PlusPlusToken& token)         override { check_type<PlusPlusToken>(); }
    void visit(const MinusMinusToken& token)       override { check_type<MinusMinusToken>(); }

    void visit(const AssignmentToken& token)       override { check_type<AssignmentToken>(); }
    void visit(const AddAssignmentToken& token)    override { check_type<AddAssignmentToken>(); }
    void visit(const SubAssignmentToken& token)    override { check_type<SubAssignmentToken>(); }
    void visit(const MulAssignmentToken& token)    override { check_type<MulAssignmentToken>(); }
    void visit(const DivAssignmentToken& token)    override { check_type<DivAssignmentToken>(); }
    void visit(const ModAssignmentToken& token)    override { check_type<DivAssignmentToken>(); }

    void visit(const PositiveToken& token)         override { check_type<PositiveToken>(); }
    void visit(const NegativeToken& token)         override { check_type<NegativeToken>(); }

    void visit(const PrefixIncrementToken& token)  override { check_type<PrefixIncrementToken>(); }
    void visit(const PostfixIncrementToken& token) override { check_type<PostfixIncrementToken>(); }
    void visit(const PrefixDecrementToken& token)  override { check_type<PrefixDecrementToken>(); }
    void visit(const PostfixDecrementToken& token) override { check_type<PostfixDecrementToken>(); }
    
    void visit(const AddToken& token)              override { check_type<AddToken>(); }
    void visit(const SubToken& token)              override { check_type<SubToken>(); }
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
