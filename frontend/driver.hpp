#pragma once

#include <memory>
#include <string>

#include "ast.hpp"
#include "location.hh"

class Driver
{
public:
    compiler::AstRootPtr ast_root;
    yy::location location;

    Driver() = default;

    compiler::AstRootPtr get_ast_root() { return std::move(ast_root); }

    int parse_file(const std::string& file_path);

private:
    int input_file_initialize(const std::string& file_path);
    void input_file_close();
};
