#include "simulator.cpp"

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <serialized_ast_file>" << std::endl;
        return 1;
    }

    try
    {
        auto serialized_ast = compiler::read_ast_from_file(argv[1]);
        auto root = compiler::deserialize_ast(serialized_ast);

        compiler::SimulatorState state;
        compiler::simulate_ast(state, root);
    }
    catch (const std::exception& ex)
    {
        std::cerr << "Error: " << ex.what() << std::endl;
        return 1;
    }

    return 0;
}
