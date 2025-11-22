#include <string>
#include <cstring>
#include <iostream>

#include "driver.hpp"
#include "parser.hpp"

extern FILE* yyin;

int Driver::parse_file(const std::string& file_path)
{
    if (input_file_initialize(file_path) != 0)
        return 1;

    yy::parser parser(*this);
    int parsing_result = parser();

    input_file_close();
    return parsing_result;
}

int Driver::input_file_initialize(const std::string& file_path)
{
    if (file_path.empty()) {
        std::cerr << "No file provided\n";
        return 1;
    }

    yyin = fopen(file_path.c_str(), "r");
    if (!yyin) {
        std::cerr << "Cannot open " << file_path << ": " << strerror(errno) << "\n";
        return 1;
    }

    location.initialize(&file_path);
    return 0;
}

void Driver::input_file_close()
{
    fclose(yyin);
}
