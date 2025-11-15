#pragma once

#include <type_traits>

#include "abstract_node.hpp"

namespace compiler
{

class AbstractAstNode::Visitor
{
  public:
    virtual void visit(const class TranslationUnitNode& token) = 0;

    virtual void visit(const class IdentifierNode& token) = 0;
    virtual void visit(const class LiteralNode& token) = 0;

    virtual void visit(const class WhileNode& token) = 0;
    virtual void visit(const class IfNode& token) = 0;
    virtual void visit(const class PrintNode& token) = 0;
    virtual void visit(const class InputNode& token) = 0;

    virtual void visit(const class AssignmentNode& token) = 0;
    virtual void visit(const class AddAssignmentNode& token) = 0;
    virtual void visit(const class SubAssignmentNode& token) = 0;
    virtual void visit(const class MulAssignmentNode& token) = 0;
    virtual void visit(const class DivAssignmentNode& token) = 0;
    virtual void visit(const class ModAssignmentNode& token) = 0;

    virtual void visit(const class UnaryPlusNode& token) = 0;
    virtual void visit(const class UnaryMinusNode& token) = 0;
    virtual void visit(const class PrefixIncrementNode& token) = 0;
    virtual void visit(const class PostfixIncrementNode& token) = 0;
    virtual void visit(const class PrefixDecrementNode& token) = 0;
    virtual void visit(const class PostfixDecrementNode& token) = 0;

    virtual void visit(const class AddNode& token) = 0;
    virtual void visit(const class SubNode& token) = 0;
    virtual void visit(const class MulNode& token) = 0;
    virtual void visit(const class DivNode& token) = 0;
    virtual void visit(const class ModNode& token) = 0;

    virtual void visit(const class EqualNode& token) = 0;
    virtual void visit(const class NotEqualNode& token) = 0;
    virtual void visit(const class LessNode& token) = 0;
    virtual void visit(const class LessEqualNode& token) = 0;
    virtual void visit(const class GreaterNode& token) = 0;
    virtual void visit(const class GreaterEqualNode& token) = 0;
};

template <typename Derived>
class BaseNode : public AbstractAstNode
{
  public:
    using AbstractAstNode::AbstractAstNode;

  public:
    virtual void accept(Visitor& visitor) const override
    {
        visitor.visit(static_cast<const Derived&>(*this));
    }
};

template <typename T>
class TypeCheckVisitor : public AbstractAstNode::Visitor
{
  public:
    // clang-format off
    void visit(const TranslationUnitNode&)  override { check_type<TranslationUnitNode>(); }

    void visit(const IdentifierNode&)       override { check_type<IdentifierNode>(); }
    void visit(const LiteralNode&)          override { check_type<LiteralNode>(); }

    void visit(const WhileNode&)            override { check_type<IdentifierNode>(); }
    void visit(const IfNode&)               override { check_type<IfNode>(); }
    void visit(const PrintNode&)            override { check_type<PrintNode>(); }
    void visit(const InputNode&)            override { check_type<InputNode>(); }

    void visit(const AssignmentNode&)       override { check_type<AssignmentNode>(); }
    void visit(const AddAssignmentNode&)    override { check_type<AddAssignmentNode>(); }
    void visit(const SubAssignmentNode&)    override { check_type<SubAssignmentNode>(); }
    void visit(const MulAssignmentNode&)    override { check_type<MulAssignmentNode>(); }
    void visit(const DivAssignmentNode&)    override { check_type<DivAssignmentNode>(); }
    void visit(const ModAssignmentNode&)    override { check_type<ModAssignmentNode>(); }

    void visit(const UnaryPlusNode&)        override { check_type<UnaryPlusNode>(); }
    void visit(const UnaryMinusNode&)       override { check_type<UnaryMinusNode>(); }
    void visit(const PrefixIncrementNode&)  override { check_type<PrefixIncrementNode>(); }
    void visit(const PostfixIncrementNode&) override { check_type<PostfixIncrementNode>(); }
    void visit(const PrefixDecrementNode&)  override { check_type<PrefixDecrementNode>(); }
    void visit(const PostfixDecrementNode&) override { check_type<PostfixDecrementNode>(); }

    void visit(const AddNode&)              override { check_type<AddNode>(); }
    void visit(const SubNode&)              override { check_type<SubNode>(); }
    void visit(const MulNode&)              override { check_type<MulNode>(); }
    void visit(const DivNode&)              override { check_type<DivNode>(); }
    void visit(const ModNode&)              override { check_type<ModNode>(); }

    void visit(const EqualNode&)            override { check_type<EqualNode>(); }
    void visit(const NotEqualNode&)         override { check_type<NotEqualNode>(); }
    void visit(const LessNode&)             override { check_type<LessNode>(); }
    void visit(const LessEqualNode&)        override { check_type<LessEqualNode>(); }
    void visit(const GreaterNode&)          override { check_type<GreaterNode>(); }
    void visit(const GreaterEqualNode&)     override { check_type<GreaterEqualNode>(); }
    // clang-format on

    bool is_type() const { return result_; }

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
bool AbstractAstNode::is_node_type() const
{
    TypeCheckVisitor<T> visitor;
    accept(visitor);
    return visitor.is_type();
}

}; // namespace compiler
