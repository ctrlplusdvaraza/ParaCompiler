#include <string>

#include "ast.hpp"
#include "serialization.hpp"
#include "dot_graph.hpp"

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
        auto serialized_ast = compiler::read_ast_from_file(argv[1]);
        auto root = compiler::deserialize_ast(serialized_ast);

        graphviz::DotGraph dotGraph;
        dotGraph.create_from_ast_tree(root);
        dotGraph.convert_to_image(argv[2]);
    }
    catch (std::exception& ex)
    {
        std::cerr << "Error during dot image generation " << ex.what() << std::endl;
        return 1;
    }

    




    return 0;
}
