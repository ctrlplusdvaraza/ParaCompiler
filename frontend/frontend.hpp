#pragma once

#include <filesystem>
#include <string>

#include "driver.hpp"

inline std::string make_ast_filepath(const std::string& source_filepath)
{
    std::filesystem::path source_filepath_obj(source_filepath);
    std::filesystem::path ast_filepath_obj = source_filepath_obj.replace_extension(".ast");
    std::string ast_filepath = ast_filepath_obj.string();

    return ast_filepath;
}

inline std::string get_filename_from_path(const std::string& filepath)
{
    std::filesystem::path filepath_obj(filepath);
    std::string filename = filepath_obj.filename().string();

    return filename;
}

inline compiler::AstRootPtr create_ast_from_source(const std::string& source_filepath)
{
    std::string source_filename = get_filename_from_path(source_filepath);

    Driver driver;
    driver.parse_file(source_filename);
    compiler::AstRootPtr root = driver.get_ast_root();
    root->set_file_name(source_filename);

    return root;
}
