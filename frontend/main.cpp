#include <filesystem>
#include <string>

#include "ast.pb.h"
#include "driver.hpp"
#include "serialization.hpp"

int main(int argc, char* argv[])
{
    if(argc != 2) 
    {
        return 1;
    }

    std::string input_filepath = argv[argc - 1];

    std::filesystem::path input_filepath_obj(input_filepath);
    std::filesystem::path output_filepath_obj = input_filepath_obj.replace_extension(".ast");

    std::string input_filename = input_filepath_obj.filename().string();
    std::string output_filepath = output_filepath_obj.string();

    Driver driver;

    int parsing_result = driver.parse_file(input_filepath);
    if(!parsing_result)
    {
        auto root = driver.get_ast_root();

        root->set_file_name(input_filename);

        ast_protobuf::SerializedAstRoot serialized = compiler::serialize_ast(root);
        
        compiler::write_ast_to_file(serialized, output_filepath);
    }

    return parsing_result;
}
