#pragma once

#include <cstring>
#include <filesystem>
#include <string>
#include <memory>

#include "ast.hpp"
#include "parser.hpp"

#define YY_DECL yy::parser::symbol_type yylex(Driver&)

yy::parser::symbol_type yylex(Driver& driver);
extern FILE* yyin;

class Driver
{
  public:
    std::unique_ptr<TranslationUnitNode> ast_root;

  public:
    Driver() = default;

    compiler::AstRootPtr get_ast_root() { return std::move(ast_root); }

    int parse_file(const std::string& file_path)
    {
        if (input_file_initialize(file_path) != 0)
        {
            return 1;
        }

        yy::parser parser(*this);
        int parsing_result = parser();

        std::filesystem::path file_path_obj(file_path);
        ast_root->set_file_name(file_path_obj.filename().string());

        input_file_close();

        return parsing_result;
    }

  private:
    int input_file_initialize(const std::string& file_path)
    {
        if (file_path.empty())
        {
            std::cerr << "No file provided" << std::endl;
            return 1;
        }

        yyin = fopen(file_path.c_str(), "r");

        if (yyin == NULL)
        {
            std::cerr << "Cannot open " << file_path << ": " << strerror(errno) << std::endl;
            return 1;
        }

        return 0;
    }

    void input_file_close()
    {
        fclose(yyin);
    }
};
