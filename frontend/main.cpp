#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "ast.pb.h"
#include "ast.hpp"
#include "driver.hpp"

#include "serialization.hpp"

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
    }

    return parsing_result;
}
