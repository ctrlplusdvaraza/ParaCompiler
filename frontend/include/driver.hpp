#pragma once

#include <string>

#include "ast.hpp"
#include "parser.hpp"

#define YY_DECL yy::parser::symbol_type yylex(Driver&)
yy::parser::symbol_type yylex(Driver& driver);


class Driver
{
  public:
    compiler::AstRootPtr ast_root;

  public:
    Driver() = default;

    compiler::AstRootPtr get_ast_root() { return std::move(ast_root); }

    int parse_file(const std::string& file_path)
    {
        if(input_file_initialize(file_path) != 0)
        {
            return 1;
        }

        yy::parser parser(*this);
        int parsing_result = parser();

        input_file_close();
        
        return parsing_result;
    }

    int input_file_initialize(const std::string& file_path);
    void input_file_close();
};

