#pragma once

#include <stdexcept>
#include <string>

#include "ast.hpp"
#include "location.hh"
#include "nametable.hpp"

namespace compiler
{

class Driver
{
  public:
    AstRootPtr ast_root;
    yy::location location;
    frontend::NamesEnviroment names_environment;

    Driver() = default;

    AstRootPtr get_ast_root() { return std::move(ast_root); }

    void parse_file(const std::string& file_path);

  private:
    void input_file_initialize(const std::string& file_path);
    void input_file_close();
};

class DriverException : public std::runtime_error
{
  public:
    using std::runtime_error::runtime_error;
};

} // namespace compiler
