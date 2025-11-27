#include "driver.hpp"
#include "frontend.hpp"
#include "serialization.hpp"

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cerr << "No file provided!" << std::endl;
        return 1;
    }

    try 
    {
        std::string source_filepath = argv[argc - 1];
        std::string ast_filepath = compiler::make_ast_filepath(source_filepath);

        compiler::AstRootPtr root = compiler::create_ast_from_source(source_filepath);

        ast_protobuf::SerializedAstRoot serialized = compiler::serialize_ast(root);
        compiler::write_ast_to_file(serialized, ast_filepath);
    }
    catch (const compiler::DriverException& ex)
    {
        std::cerr << ex.what() << std::endl;
        return 1;
    }
    catch (const compiler::ProtobufException& ex)
    {
        std::cerr << ex.what() << std::endl;
        return 1;
    }
    
    return 0;
}
