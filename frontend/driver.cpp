#include <cstring>
#include <iostream>
#include <string>

#include "driver.hpp"
#include "parser.hpp"

extern FILE* yyin;

void Driver::parse_file(const std::string& file_path)
{
    input_file_initialize(file_path);

    yy::parser parser(*this);
    if (parser())
    {
        throw DriverException("Error during parsing");
    }

    input_file_close();
}

void Driver::input_file_initialize(const std::string& file_path)
{
    if (file_path.empty())
    {
        throw DriverException("No file provided\n");
    }

    yyin = fopen(file_path.c_str(), "r");
    if (!yyin)
    {
        throw DriverException("Cannot open " + file_path + "\n");
    }

    location.initialize(&file_path);
}

void Driver::input_file_close() { fclose(yyin); }
