#include "frontend/include/tokens/tokens.hpp"

using namespace compiler;

int main ()
{
    LessEqualToken tok("<=");

    std::cout << tok.get_string_token() << std::endl;
}
