#pragma once

#include <stdexcept>

#include "ast.hpp"
#include "ast.pb.h"

namespace compiler
{

ast_protobuf::SerializedAstNode serialize_node(const AstNode& node);
ast_protobuf::SerializedAstRoot serialize_ast(const AstRootPtr& root);

AstRootPtr deserialize_ast(const ast_protobuf::SerializedAstRoot& serialized_root);
AstNodePtr deserialize_node(const ast_protobuf::SerializedAstNode& msg);

ast_protobuf::SerializedAstRoot read_ast_from_file(const std::string filepath);
void write_ast_to_file(const ast_protobuf::SerializedAstRoot& root, const std::string filepath);

class ProtobufException : public std::runtime_error
{
  public:
    using std::runtime_error::runtime_error;
};

}; // namespace compiler
