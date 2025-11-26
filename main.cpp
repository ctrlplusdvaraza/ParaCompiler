#include <getopt.h>
#include <iostream>
#include <string>
#include <vector>

#include "frontend.hpp"
#include "simulator.hpp"

struct ProgramOptions
{
    bool graphviz = false;
    bool ast = false;
    std::string ast_filepath;
    std::string graphviz_filepath;
    std::string source_filepath;
};

bool parse_command_line(int argc, char** argv, ProgramOptions& options);

int main(int argc, char** argv)
{
    ProgramOptions options;
    if (!parse_command_line(argc, argv, options))
    {
        return 1;
    }

    compiler::AstRootPtr ast_root = create_ast_from_source(options.source_filepath);
    compiler::SimulatorState state;
    compiler::simulate_ast(state, ast_root);

    if (options.opt_graphviz)
    {
        compiler::graphvizer::DotGraph dot_graph;
        dot_graph.create_from_ast_tree(ast_root);
        dot_graph.convert_to_image(options.graphviz_filepath);
    }

    if (options.opt_ast)
    {
        ast_protobuf::SerializedAstRoot serialized = compiler::serialize_ast(root);
        compiler::write_ast_to_file(serialized, options.ast_filepath);
    }

    return 0;
}

void print_usage(const char* program)
{
    std::cerr << "Usage: " << program << " [--graphviz[=output_file]] [--ast[=output_file]] <source_file>\n";
}

void initialize_ast_path_by_default(ProgramOptions& options)
{
    options.ast_filepath = make_ast_filepath(options.source_filepath);
}

void initialize_graphviz_path_by_default(ProgramOptions& options)
{
    options.graphviz_filepath = make_graphviz_filepath(options.source_filepath);
}

bool parse_options(int argc, char** argv, ProgramOptions& options)
{
    option long_opts[] = {
        {"graphviz", optional_argument, nullptr, 'g'},
        {"ast",      optional_argument, nullptr, 'a'},
        {0, 0, 0, 0}
    };

    int option;
    while ((option = getopt_long(argc, argv, "g::a::", long_opts, nullptr)) != -1)
    {
        switch (option)
        {
            case 'g':
                options.graphviz = true;
                if (optarg != nullptr)
                {
                    options.graphviz_filepath = optarg;
                }
                break;
            case 'a':
                options.ast = true;
                if (optarg != nullptr)
                {
                    options.ast_filepath = optarg;
                }
                break;
            default:
                print_usage(argv[0]);
                return false;
        }
    }

    return true;
}

bool parse_arguments(int argc, char** argv, ProgramOptions& options)
{
    std::vector<std::string> arguments;
    for (int i = optind; i < argc; ++i)
    {
        arguments.emplace_back(argv[i]);
    }

    if (arguments.empty())
    {
        std::cerr << "No source file provided\n";
        print_usage(argv[0]);
        return false;
    }

    if (arguments.size() > 1)
    {
        std::cerr << "Too many arguments\n";
        print_usage(argv[0]);
        return false;
    }

    options.source_filepath = arguments.front();

    if (options.ast && options.ast_filepath.empty())
    {
        initialize_ast_path_by_default(options);
    }

    if (options.graphviz && options.graphviz_filepath.empty())
    {
        initialize_graphviz_path_by_default(options);
    }

    return true;
}

bool parse_command_line(int argc, char** argv, ProgramOptions& options)
{
    if (!parse_options(argc, argv, options))
    {
        return false;
    }

    return parse_arguments(argc, argv, options);
}
