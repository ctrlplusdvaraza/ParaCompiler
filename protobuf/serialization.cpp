#include "ast.hpp"
#include "ast.pb.h"
#include "keyword_tokens.hpp"
#include "tokens.pb.h"
#include <fstream>
#include <memory>
#include <stdexcept>

namespace compiler
{

static void fill_token_oneof(const AbstractToken& token, ast_protobuf::SerializedAstNode& msg)
{
    AbstractToken& tok = const_cast<AbstractToken&>(token);

    if (tok.is_token_of_type<WhileToken>())
    {
        auto* out = msg.mutable_while_token();
        out->set_token(tok.get_string_token());
    }
    if (tok.is_token_of_type<WhileToken>())
    {
        auto* out = msg.mutable_while_token();
        out->set_token(tok.get_string_token());
    }
    if (tok.is_token_of_type<IfToken>())
    {
        auto* out = msg.mutable_if_token();
        out->set_token(tok.get_string_token());
    }
    if (tok.is_token_of_type<PrintToken>())
    {
        auto* out = msg.mutable_print_token();
        out->set_token(tok.get_string_token());
    }
    if (tok.is_token_of_type<InputToken>())
    {
        auto* out = msg.mutable_input_token();
        out->set_token(tok.get_string_token());
    }
    if (tok.is_token_of_type<IdentifierToken>())
    {
        auto* out = msg.mutable_identifier_token();
        out->set_token(tok.get_string_token());
    }
    if (tok.is_token_of_type<LiteralToken>())
    {
        auto* out = msg.mutable_literal_token();
        out->set_token(tok.get_string_token());
    }
    if (tok.is_token_of_type<AssignmentToken>())
    {
        auto* out = msg.mutable_assignment_token();
        out->set_token(tok.get_string_token());
    }
    if (tok.is_token_of_type<AddAssignmentToken>())
    {
        auto* out = msg.mutable_add_assignment_token();
        out->set_token(tok.get_string_token());
    }
    if (tok.is_token_of_type<SubAssignmentToken>())
    {
        auto* out = msg.mutable_sub_assignment_token();
        out->set_token(tok.get_string_token());
    }
    if (tok.is_token_of_type<MulAssignmentToken>())
    {
        auto* out = msg.mutable_mul_assignment_token();
        out->set_token(tok.get_string_token());
    }
    if (tok.is_token_of_type<DivAssignmentToken>())
    {
        auto* out = msg.mutable_mul_assignment_token();
        out->set_token(tok.get_string_token());
    }
    if (tok.is_token_of_type<ModAssignmentToken>())
    {
        auto* out = msg.mutable_mod_assignment_token();
        out->set_token(tok.get_string_token());
    }
    if (tok.is_token_of_type<AddToken>())
    {
        auto* out = msg.mutable_add_token();
        out->set_token(tok.get_string_token());
    }
    if (tok.is_token_of_type<SubToken>())
    {
        auto* out = msg.mutable_sub_token();
        out->set_token(tok.get_string_token());
    }
    if (tok.is_token_of_type<MulToken>())
    {
        auto* out = msg.mutable_mul_token();
        out->set_token(tok.get_string_token());
    }
    if (tok.is_token_of_type<DivToken>())
    {
        auto* out = msg.mutable_div_token();
        out->set_token(tok.get_string_token());
    }
    if (tok.is_token_of_type<ModToken>())
    {
        auto* out = msg.mutable_mod_token();
        out->set_token(tok.get_string_token());
    }
    if (tok.is_token_of_type<EqualToken>())
    {
        auto* out = msg.mutable_equal_token();
        out->set_token(tok.get_string_token());
    }
    if (tok.is_token_of_type<NotEqualToken>())
    {
        auto* out = msg.mutable_not_equal_token();
        out->set_token(tok.get_string_token());
    }
    if (tok.is_token_of_type<LessToken>())
    {
        auto* out = msg.mutable_less_token();
        out->set_token(tok.get_string_token());
    }
    if (tok.is_token_of_type<LessEqualToken>())
    {
        auto* out = msg.mutable_less_equal_token();
        out->set_token(tok.get_string_token());
    }
    if (tok.is_token_of_type<GreaterToken>())
    {
        auto* out = msg.mutable_greater_token();
        out->set_token(tok.get_string_token());
    }
    if (tok.is_token_of_type<GreaterEqualToken>())
    {
        auto* out = msg.mutable_greater_equal_token();
        out->set_token(tok.get_string_token());
    }
    if (tok.is_token_of_type<UnaryPlusToken>())
    {
        auto* out = msg.mutable_unary_plus_token();
        out->set_token(tok.get_string_token());
    }
    if (tok.is_token_of_type<UnaryMinusToken>())
    {
        auto* out = msg.mutable_unary_minus_token();
        out->set_token(tok.get_string_token());
    }
    if (tok.is_token_of_type<PrefixIncrementToken>())
    {
        auto* out = msg.mutable_prefix_increment_token();
        out->set_token(tok.get_string_token());
    }
    if (tok.is_token_of_type<PostfixIncrementToken>())
    {
        auto* out = msg.mutable_postfix_increment_token();
        out->set_token(tok.get_string_token());
    }
    if (tok.is_token_of_type<PrefixDecrementToken>())
    {
        auto* out = msg.mutable_prefix_decrement_token();
        out->set_token(tok.get_string_token());
    }
    if (tok.is_token_of_type<PostfixDecrementToken>())
    {
        auto* out = msg.mutable_postfix_decrement_token();
        out->set_token(tok.get_string_token());
    }
}

static AbstractTokenPtr make_token_from_oneof(const ast_protobuf::SerializedAstNode& msg)
{
    switch (msg.token_case())
    {
        case ast_protobuf::SerializedAstNode::kWhileToken:
            {
                const auto& token = msg.while_token();
                return std::make_unique<WhileToken>(std::string(token.token()));
            }
        case ast_protobuf::SerializedAstNode::kIfToken:
            {
                const auto& token = msg.if_token();
                return std::make_unique<IfToken>(std::string(token.token()));
            }
        case ast_protobuf::SerializedAstNode::kPrintToken:
            {
                const auto& token = msg.print_token();
                return std::make_unique<PrintToken>(std::string(token.token()));
            }
        case ast_protobuf::SerializedAstNode::kInputToken:
            {
                const auto& token = msg.input_token();
                return std::make_unique<InputToken>(std::string(token.token()));
            }
        case ast_protobuf::SerializedAstNode::kIdentifierToken:
            {
                const auto& token = msg.identifier_token();
                return std::make_unique<IdentifierToken>(std::string(token.token()));
            }
        case ast_protobuf::SerializedAstNode::kLiteralToken:
            {
                const auto& token = msg.literal_token();
                return std::make_unique<LiteralToken>(std::string(token.token()));
            }
        case ast_protobuf::SerializedAstNode::kAssignmentToken:
            {
                const auto& token = msg.assignment_token();
                return std::make_unique<AssignmentToken>(std::string(token.token()));
            }
        case ast_protobuf::SerializedAstNode::kAddAssignmentToken:
            {
                const auto& token = msg.add_assignment_token();
                return std::make_unique<AddAssignmentToken>(std::string(token.token()));
            }
        case ast_protobuf::SerializedAstNode::kSubAssignmentToken:
            {
                const auto& token = msg.sub_assignment_token();
                return std::make_unique<SubAssignmentToken>(std::string(token.token()));
            }
        case ast_protobuf::SerializedAstNode::kMulAssignmentToken:
            {
                const auto& token = msg.mul_assignment_token();
                return std::make_unique<MulAssignmentToken>(std::string(token.token()));
            }
        case ast_protobuf::SerializedAstNode::kDivAssignmentToken:
            {
                const auto& token = msg.div_assignment_token();
                return std::make_unique<DivAssignmentToken>(std::string(token.token()));
            }
        case ast_protobuf::SerializedAstNode::kModAssignmentToken:
            {
                const auto& token = msg.mod_assignment_token();
                return std::make_unique<ModAssignmentToken>(std::string(token.token()));
            }
        case ast_protobuf::SerializedAstNode::kAddToken:
            {
                const auto& token = msg.add_assignment_token();
                return std::make_unique<AddToken>(std::string(token.token()));
            }
        case ast_protobuf::SerializedAstNode::kSubToken:
            {
                const auto& token = msg.sub_token();
                return std::make_unique<SubToken>(std::string(token.token()));
            }
        case ast_protobuf::SerializedAstNode::kMulToken:
            {
                const auto& token = msg.mul_token();
                return std::make_unique<MulToken>(std::string(token.token()));
            }
        case ast_protobuf::SerializedAstNode::kDivToken:
            {
                const auto& token = msg.div_token();
                return std::make_unique<DivToken>(std::string(token.token()));
            }
        case ast_protobuf::SerializedAstNode::kModToken:
            {
                const auto& token = msg.mod_token();
                return std::make_unique<ModToken>(std::string(token.token()));
            }
        case ast_protobuf::SerializedAstNode::kEqualToken:
            {
                const auto& token = msg.equal_token();
                return std::make_unique<EqualToken>(std::string(token.token()));
            }
        case ast_protobuf::SerializedAstNode::kNotEqualToken:
            {
                const auto& token = msg.not_equal_token();
                return std::make_unique<NotEqualToken>(std::string(token.token()));
            }
        case ast_protobuf::SerializedAstNode::kLessToken:
            {
                const auto& token = msg.less_token();
                return std::make_unique<LessToken>(std::string(token.token()));
            }
        case ast_protobuf::SerializedAstNode::kLessEqualToken:
            {
                const auto& token = msg.less_equal_token();
                return std::make_unique<LessEqualToken>(std::string(token.token()));
            }
        case ast_protobuf::SerializedAstNode::kGreaterToken:
            {
                const auto& token = msg.greater_token();
                return std::make_unique<GreaterToken>(std::string(token.token()));
            }
        case ast_protobuf::SerializedAstNode::kGreaterEqualToken:
            {
                const auto& token = msg.greater_equal_token();
                return std::make_unique<GreaterEqualToken>(std::string(token.token()));
            }
        case ast_protobuf::SerializedAstNode::kUnaryPlusToken:
            {
                const auto& token = msg.unary_plus_token();
                return std::make_unique<UnaryPlusToken>(std::string(token.token()));
            }
        case ast_protobuf::SerializedAstNode::kUnaryMinusToken:
            {
                const auto& token = msg.unary_minus_token();
                return std::make_unique<UnaryMinusToken>(std::string(token.token()));
            }
        case ast_protobuf::SerializedAstNode::kPrefixIncrementToken:
            {
                const auto& token = msg.prefix_increment_token();
                return std::make_unique<PrefixIncrementToken>(std::string(token.token()));
            }
        case ast_protobuf::SerializedAstNode::kPostfixIncrementToken:
            {
                const auto& token = msg.postfix_increment_token();
                return std::make_unique<PostfixIncrementToken>(std::string(token.token()));
            }
        case ast_protobuf::SerializedAstNode::kPrefixDecrementToken:
            {
                const auto& token = msg.prefix_decrement_token();
                return std::make_unique<PrefixDecrementToken>(std::string(token.token()));
            }
        case ast_protobuf::SerializedAstNode::kPostfixDecrementToken:
            {
                const auto& token = msg.postfix_decrement_token();
                return std::make_unique<PostfixDecrementToken>(std::string(token.token()));
            }

        default:
            throw std::runtime_error("SerializedAstNode has no token set");
    }
}

ast_protobuf::SerializedAstNode serialize_node(const AstNode& node)
{
    ast_protobuf::SerializedAstNode msg;

    if (node.token)
    {
        fill_token_oneof(*node.token, msg);
    }

    for (const auto& child : node.children)
    {
        *msg.add_children() = serialize_node(*child);
    }
    return msg;
}

AstNodePtr deserialize_node(const ast_protobuf::SerializedAstNode& msg)
{
    AbstractTokenPtr tok = make_token_from_oneof(msg);

    auto node = std::make_unique<AstNode>(std::move(tok));

    node->children.reserve(msg.children_size());
    for (int i = 0; i < msg.children_size(); ++i)
    {
        node->children.push_back(deserialize_node(msg.children(i)));
    }

    return node;
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

AstRootPtr deserialize_ast(const ast_protobuf::SerializedAstRoot& serialized_root)
{
    auto root = std::make_unique<AstRoot>();

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

} // namespace compiler
