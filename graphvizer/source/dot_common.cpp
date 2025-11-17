#include "dot_common.hpp"
#include "dot_graph.hpp"

namespace compiler::graphviz
{

#include <cstdlib>

Color generate_random_color()
{
    return Color{
        static_cast<uint8_t>(rand() % 256),
        static_cast<uint8_t>(rand() % 256),
        static_cast<uint8_t>(rand() % 256)
    };
}

} // namespace compiler::graphviz
