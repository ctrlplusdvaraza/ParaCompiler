#pragma once

#include <string>

#include "ast.hpp"
#include "parser.hpp"

#define YY_DECL yy::parser::symbol_type yylex(Driver& driver)

YY_DECL;

class Driver
{
  public:
    Driver() = default;

    compiler::AstRootPtr ast_root;

    compiler::AstRootPtr get_ast_root() { return std::move(ast_root); }

    const std::string& get_file() const { return file_; }
    yy::location& get_location() { return location_; }

    int parse(const std::string& file_path)
    {
        file_ = file_path;
        location_.initialize(&get_file());

        scan_begin();
        yy::parser parser(*this);

        int res = parser();

        scan_end();
        return res;
    }

    void scan_begin();
    void scan_end();

  private:
    std::string file_;
    yy::location location_;
};
