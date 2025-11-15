#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "ast.pb.h"
#include "ast.hpp"
#include "driver.hpp"

#include "serialization.hpp"

namespace compiler
{

void traverse_ast(const AstNodePtr& node)
{
    if (node->is_node_type<AssignmentNode>())
    {
        std::cout << node->children[0]->get_string_lexeme() << node->get_string_lexeme()
                  << node->children[1]->get_string_lexeme() << std::endl;
    }

    for (const auto& cur : node->children)
    {
        traverse_ast(cur);
    }
}

void traverse_ast_root(const AstRootPtr& root)
{
    for (const auto& child : root->children)
    {
        traverse_ast(child);
    }
}

} // namespace compiler

int main(int argc, char* argv[])
{
    if(argc != 2) 
    {
        return 1;
    }

    Driver driver;

    int parsing_result = driver.parse_file(argv[argc - 1]);
    if(!parsing_result)
    {
        auto root = driver.get_ast_root();

    ast_protobuf::SerializedAstRoot serialized = serialize_ast(root);
	
	write_ast_to_file(serialized, "amogus_real.ast");

        traverse_ast_root(root);
    }

    return parsing_result;
}
