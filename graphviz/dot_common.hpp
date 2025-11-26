#pragma once
#include <string>
#include <cxxabi.h>

namespace compiler::graphvizer
{

class Color;
Color generate_random_color();

std::string demangle(const char* mangled);
std::string strip_namespace(const std::string& s);

} // namespace compiler::graphvizer