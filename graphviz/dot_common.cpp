#include <cstdlib>

#include "dot_common.hpp"
#include "dot_graph.hpp"

namespace compiler::graphvizer
{

Color generate_random_color()
{
    return Color{static_cast<uint8_t>(56 + rand() % 200), static_cast<uint8_t>(56 + rand() % 200),
                 static_cast<uint8_t>(56 + rand() % 200)};
}

std::string demangle(const char* mangled)
{
    assert(mangled);

    int status = 0;
    char* dem = abi::__cxa_demangle(mangled, nullptr, nullptr, &status);
    if (status != 0)
    {
        std::cerr << "demagle failed. mangled : '" << mangled << "'\n";
        free(dem);
        return {};
    }

    std::string out = (status == 0 && dem) ? dem : mangled;
    free(dem);
    return out;
}

std::string strip_namespace(const std::string& s)
{
    size_t pos = s.rfind("::");
    return (pos == std::string::npos) ? s : s.substr(pos + 2);
}

} // namespace compiler::graphvizer
