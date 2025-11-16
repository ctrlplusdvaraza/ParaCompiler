#include "ast.hpp"
#include "ast.pb.h"
#include "nodes.pb.h"
#include <fstream>
#include <memory>
#include <stdexcept>

namespace compiler
{

static void fill_node_oneof(const AstNode& node, ast_protobuf::SerializedAstNode& msg);
static AstNodePtr make_node_from_oneof(const ast_protobuf::SerializedAstNode& msg);

ast_protobuf::SerializedAstNode serialize_node(const AstNode& node)
{
    ast_protobuf::SerializedAstNode msg;

    fill_node_oneof(node, msg);

    for (const auto& child : node.children)
    {
        *msg.add_children() = serialize_node(*child);
    }
    return msg;
}

ast_protobuf::SerializedAstRoot serialize_ast(const AstRootPtr& root)
{
    ast_protobuf::SerializedAstRoot serialized_root;
    if (!root)
    {
        return serialized_root;
    }

    serialized_root.mutable_children()->Reserve(root->children.size());

    for (const auto& child : root->children)
    {
        *serialized_root.add_children() = serialize_node(*child);
    }

    return serialized_root;
}

AstNodePtr deserialize_node(const ast_protobuf::SerializedAstNode& msg)
{
    AstNodePtr node = make_node_from_oneof(msg);

    node->children.reserve(msg.children_size());
    for (int i = 0; i < msg.children_size(); ++i)
    {
        node->children.push_back(deserialize_node(msg.children(i)));
    }

    return node;
}

AstRootPtr deserialize_ast(const ast_protobuf::SerializedAstRoot& serialized_root)
{
    std::unique_ptr<AstRoot> root = std::make_unique<AstRoot>(serialized_root.lexeme());

    root->children.reserve(serialized_root.children_size());
    for (int i = 0; i < serialized_root.children_size(); ++i)
    {
        root->children.push_back(deserialize_node(serialized_root.children(i)));
    }

    return root;
}

ast_protobuf::SerializedAstRoot read_ast_from_file(const std::string filepath)
{
    ast_protobuf::SerializedAstRoot root;

    std::ifstream in(filepath, std::ios::binary);
    if (!in)
    {
        throw std::runtime_error("failed to open file for reading: " + filepath);
    }

    if (!root.ParseFromIstream(&in))
    {
        throw std::runtime_error("failed to parse SerializedAstRoot from file: " + filepath);
    }

    return root;
}

void write_ast_to_file(const ast_protobuf::SerializedAstRoot& root, const std::string filepath)
{
    std::ofstream out(filepath, std::ios::binary);
    if (!out)
    {
        throw std::runtime_error("failed to open file for writing: " + filepath);
    }

    if (!root.SerializeToOstream(&out))
    {
        throw std::runtime_error("failed to write SerializedAstRoot to file: " + filepath);
    }
}

static void fill_node_oneof(const AstNode& node, ast_protobuf::SerializedAstNode& msg)
{
    if (node.is_node_type<WhileNode>())
    {
        auto* out = msg.mutable_while_node();
        out->set_lexeme(node.get_string_lexeme());
    }
    if (node.is_node_type<IfNode>())
    {
        auto* out = msg.mutable_if_node();
        out->set_lexeme(node.get_string_lexeme());
    }
    if (node.is_node_type<PrintNode>())
    {
        auto* out = msg.mutable_print_node();
        out->set_lexeme(node.get_string_lexeme());
    }
    if (node.is_node_type<InputNode>())
    {
        auto* out = msg.mutable_input_node();
        out->set_lexeme(node.get_string_lexeme());
    }
    if (node.is_node_type<IdentifierNode>())
    {
        auto* out = msg.mutable_identifier_node();
        out->set_lexeme(node.get_string_lexeme());
    }
    if (node.is_node_type<LiteralNode>())
    {
        auto* out = msg.mutable_literal_node();
        out->set_lexeme(node.get_string_lexeme());
    }
    if (node.is_node_type<AssignmentNode>())
    {
        auto* out = msg.mutable_assignment_node();
        out->set_lexeme(node.get_string_lexeme());
    }
    if (node.is_node_type<AddAssignmentNode>())
    {
        auto* out = msg.mutable_add_assignment_node();
        out->set_lexeme(node.get_string_lexeme());
    }
    if (node.is_node_type<SubAssignmentNode>())
    {
        auto* out = msg.mutable_sub_assignment_node();
        out->set_lexeme(node.get_string_lexeme());
    }
    if (node.is_node_type<MulAssignmentNode>())
    {
        auto* out = msg.mutable_mul_assignment_node();
        out->set_lexeme(node.get_string_lexeme());
    }
    if (node.is_node_type<DivAssignmentNode>())
    {
        auto* out = msg.mutable_mul_assignment_node();
        out->set_lexeme(node.get_string_lexeme());
    }
    if (node.is_node_type<ModAssignmentNode>())
    {
        auto* out = msg.mutable_mod_assignment_node();
        out->set_lexeme(node.get_string_lexeme());
    }
    if (node.is_node_type<AddNode>())
    {
        auto* out = msg.mutable_add_node();
        out->set_lexeme(node.get_string_lexeme());
    }
    if (node.is_node_type<SubNode>())
    {
        auto* out = msg.mutable_sub_node();
        out->set_lexeme(node.get_string_lexeme());
    }
    if (node.is_node_type<MulNode>())
    {
        auto* out = msg.mutable_mul_node();
        out->set_lexeme(node.get_string_lexeme());
    }
    if (node.is_node_type<DivNode>())
    {
        auto* out = msg.mutable_div_node();
        out->set_lexeme(node.get_string_lexeme());
    }
    if (node.is_node_type<ModNode>())
    {
        auto* out = msg.mutable_mod_node();
        out->set_lexeme(node.get_string_lexeme());
    }
    if (node.is_node_type<EqualNode>())
    {
        auto* out = msg.mutable_equal_node();
        out->set_lexeme(node.get_string_lexeme());
    }
    if (node.is_node_type<NotEqualNode>())
    {
        auto* out = msg.mutable_not_equal_node();
        out->set_lexeme(node.get_string_lexeme());
    }
    if (node.is_node_type<LessNode>())
    {
        auto* out = msg.mutable_less_node();
        out->set_lexeme(node.get_string_lexeme());
    }
    if (node.is_node_type<LessEqualNode>())
    {
        auto* out = msg.mutable_less_equal_node();
        out->set_lexeme(node.get_string_lexeme());
    }
    if (node.is_node_type<GreaterNode>())
    {
        auto* out = msg.mutable_greater_node();
        out->set_lexeme(node.get_string_lexeme());
    }
    if (node.is_node_type<GreaterEqualNode>())
    {
        auto* out = msg.mutable_greater_equal_node();
        out->set_lexeme(node.get_string_lexeme());
    }
    if (node.is_node_type<UnaryPlusNode>())
    {
        auto* out = msg.mutable_unary_plus_node();
        out->set_lexeme(node.get_string_lexeme());
    }
    if (node.is_node_type<UnaryMinusNode>())
    {
        auto* out = msg.mutable_unary_minus_node();
        out->set_lexeme(node.get_string_lexeme());
    }
    if (node.is_node_type<PrefixIncrementNode>())
    {
        auto* out = msg.mutable_prefix_increment_node();
        out->set_lexeme(node.get_string_lexeme());
    }
    if (node.is_node_type<PostfixIncrementNode>())
    {
        auto* out = msg.mutable_postfix_increment_node();
        out->set_lexeme(node.get_string_lexeme());
    }
    if (node.is_node_type<PrefixDecrementNode>())
    {
        auto* out = msg.mutable_prefix_decrement_node();
        out->set_lexeme(node.get_string_lexeme());
    }
    if (node.is_node_type<PostfixDecrementNode>())
    {
        auto* out = msg.mutable_postfix_decrement_node();
        out->set_lexeme(node.get_string_lexeme());
    }
}

static AstNodePtr make_node_from_oneof(const ast_protobuf::SerializedAstNode& msg)
{
    switch (msg.node_case())
    {
        case ast_protobuf::SerializedAstNode::kWhileNode:
            {
                const auto& node = msg.while_node();
                return std::make_unique<WhileNode>(std::string(node.lexeme()));
            }
        case ast_protobuf::SerializedAstNode::kIfNode:
            {
                const auto& node = msg.if_node();
                return std::make_unique<IfNode>(std::string(node.lexeme()));
            }
        case ast_protobuf::SerializedAstNode::kPrintNode:
            {
                const auto& node = msg.print_node();
                return std::make_unique<PrintNode>(std::string(node.lexeme()));
            }
        case ast_protobuf::SerializedAstNode::kInputNode:
            {
                const auto& node = msg.input_node();
                return std::make_unique<InputNode>(std::string(node.lexeme()));
            }
        case ast_protobuf::SerializedAstNode::kIdentifierNode:
            {
                const auto& node = msg.identifier_node();
                return std::make_unique<IdentifierNode>(std::string(node.lexeme()));
            }
        case ast_protobuf::SerializedAstNode::kLiteralNode:
            {
                const auto& node = msg.literal_node();
                return std::make_unique<LiteralNode>(std::string(node.lexeme()));
            }
        case ast_protobuf::SerializedAstNode::kAssignmentNode:
            {
                const auto& node = msg.assignment_node();
                return std::make_unique<AssignmentNode>(std::string(node.lexeme()));
            }
        case ast_protobuf::SerializedAstNode::kAddAssignmentNode:
            {
                const auto& node = msg.add_assignment_node();
                return std::make_unique<AddAssignmentNode>(std::string(node.lexeme()));
            }
        case ast_protobuf::SerializedAstNode::kSubAssignmentNode:
            {
                const auto& node = msg.sub_assignment_node();
                return std::make_unique<SubAssignmentNode>(std::string(node.lexeme()));
            }
        case ast_protobuf::SerializedAstNode::kMulAssignmentNode:
            {
                const auto& node = msg.mul_assignment_node();
                return std::make_unique<MulAssignmentNode>(std::string(node.lexeme()));
            }
        case ast_protobuf::SerializedAstNode::kDivAssignmentNode:
            {
                const auto& node = msg.div_assignment_node();
                return std::make_unique<DivAssignmentNode>(std::string(node.lexeme()));
            }
        case ast_protobuf::SerializedAstNode::kModAssignmentNode:
            {
                const auto& node = msg.mod_assignment_node();
                return std::make_unique<ModAssignmentNode>(std::string(node.lexeme()));
            }
        case ast_protobuf::SerializedAstNode::kAddNode:
            {
                const auto& node = msg.add_assignment_node();
                return std::make_unique<AddNode>(std::string(node.lexeme()));
            }
        case ast_protobuf::SerializedAstNode::kSubNode:
            {
                const auto& node = msg.sub_node();
                return std::make_unique<SubNode>(std::string(node.lexeme()));
            }
        case ast_protobuf::SerializedAstNode::kMulNode:
            {
                const auto& node = msg.mul_node();
                return std::make_unique<MulNode>(std::string(node.lexeme()));
            }
        case ast_protobuf::SerializedAstNode::kDivNode:
            {
                const auto& node = msg.div_node();
                return std::make_unique<DivNode>(std::string(node.lexeme()));
            }
        case ast_protobuf::SerializedAstNode::kModNode:
            {
                const auto& node = msg.mod_node();
                return std::make_unique<ModNode>(std::string(node.lexeme()));
            }
        case ast_protobuf::SerializedAstNode::kEqualNode:
            {
                const auto& node = msg.equal_node();
                return std::make_unique<EqualNode>(std::string(node.lexeme()));
            }
        case ast_protobuf::SerializedAstNode::kNotEqualNode:
            {
                const auto& node = msg.not_equal_node();
                return std::make_unique<NotEqualNode>(std::string(node.lexeme()));
            }
        case ast_protobuf::SerializedAstNode::kLessNode:
            {
                const auto& node = msg.less_node();
                return std::make_unique<LessNode>(std::string(node.lexeme()));
            }
        case ast_protobuf::SerializedAstNode::kLessEqualNode:
            {
                const auto& node = msg.less_equal_node();
                return std::make_unique<LessEqualNode>(std::string(node.lexeme()));
            }
        case ast_protobuf::SerializedAstNode::kGreaterNode:
            {
                const auto& node = msg.greater_node();
                return std::make_unique<GreaterNode>(std::string(node.lexeme()));
            }
        case ast_protobuf::SerializedAstNode::kGreaterEqualNode:
            {
                const auto& node = msg.greater_equal_node();
                return std::make_unique<GreaterEqualNode>(std::string(node.lexeme()));
            }
        case ast_protobuf::SerializedAstNode::kUnaryPlusNode:
            {
                const auto& node = msg.unary_plus_node();
                return std::make_unique<UnaryPlusNode>(std::string(node.lexeme()));
            }
        case ast_protobuf::SerializedAstNode::kUnaryMinusNode:
            {
                const auto& node = msg.unary_minus_node();
                return std::make_unique<UnaryMinusNode>(std::string(node.lexeme()));
            }
        case ast_protobuf::SerializedAstNode::kPrefixIncrementNode:
            {
                const auto& node = msg.prefix_increment_node();
                return std::make_unique<PrefixIncrementNode>(std::string(node.lexeme()));
            }
        case ast_protobuf::SerializedAstNode::kPostfixIncrementNode:
            {
                const auto& node = msg.postfix_increment_node();
                return std::make_unique<PostfixIncrementNode>(std::string(node.lexeme()));
            }
        case ast_protobuf::SerializedAstNode::kPrefixDecrementNode:
            {
                const auto& node = msg.prefix_decrement_node();
                return std::make_unique<PrefixDecrementNode>(std::string(node.lexeme()));
            }
        case ast_protobuf::SerializedAstNode::kPostfixDecrementNode:
            {
                const auto& node = msg.postfix_decrement_node();
                return std::make_unique<PostfixDecrementNode>(std::string(node.lexeme()));
            }

        default:
            throw std::runtime_error("SerializedAstNode has no node set");
    }
}

} // namespace compiler
