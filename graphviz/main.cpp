#include <string>

#include "ast.hpp"
#include "dot_graph.hpp"
#include "serialization.hpp"

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        std::cerr << "No serialized AST provided!" << std::endl;
        return 1;
    }

    if (argc < 3)
    {
        std::cerr << "No output image path provided!" << std::endl;
        return 1;
    }

    try
    {
        ast_protobuf::SerializedAstRoot serialized_ast = compiler::read_ast_from_file(argv[1]);
        compiler::AstRootPtr                  ast_root = compiler::deserialize_ast(serialized_ast);

        compiler::graphvizer::DotGraph dot_graph;
        dot_graph.create_from_ast_tree(ast_root);
        dot_graph.convert_to_image(argv[2]);
    }
    catch (std::exception& ex)
    {
        std::cerr << "Error during dot image generation " << ex.what() << std::endl;
        return 1;
    }

    return 0;
}
