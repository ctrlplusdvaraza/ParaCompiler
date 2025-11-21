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

    std::string file_path = argv[argc - 1];

    Driver driver;

    int parsing_result = driver.parse_file(file_path);
    if(!parsing_result)
    {
        auto root = driver.get_ast_root();

        std::filesystem::path file_path_obj(file_path);
        root->set_file_name(file_path_obj.filename().string());

        ast_protobuf::SerializedAstRoot serialized = serialize_ast(root);
        
        write_ast_to_file(serialized, "amogus_real.ast");
    }

    return parsing_result;
}
