#include "ast.hpp"
#include "serialization.hpp"
#include <string>
#include <unordered_map>

namespace compiler
{
class SimulatorState
{
  public:
    using ValueType = int64_t;

    SimulatorState() = default;

    void update_var(const std::string name, ValueType val)
    {
    }

  private:
    std::unordered_map<std::string, ValueType> state_vals_{};
};

void traverse_ast(const SimulatorState& state, const AstNodePtr& node)
{
    std::cout << node->get_string_lexeme() << std::endl;

    if (node->is_node_type<InputNode>())
    {

    }

    for (const auto& cur : node->children)
    {
        traverse_ast(state, cur);
    }
}

void traverse_ast_root(const SimulatorState& state, const AstRootPtr& root)
{
    for (const auto& child : root->children)
    {
        traverse_ast(state, child);
    }
}

} // namespace compiler

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        std::cerr << "No serialized AST provided!" << std::endl;
        return 1;
    }

    try
    {
        auto serialized_ast = compiler::read_ast_from_file(argv[1]);
        auto root = compiler::deserialize_ast(serialized_ast);
    }
    catch (std::exception& ex)
    {
        std::cerr << "Error during AST deserialization: " << ex.what() << std::endl;
        return 1;
    }

    return 0;
}
