#include "dot_graph.hpp"
#include "dot_common.hpp"

namespace graphviz 
{

Color generate_random_color() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> dis(0, 255);

    return Color
    {
        static_cast<uint8_t>(dis(gen)),
        static_cast<uint8_t>(dis(gen)),
        static_cast<uint8_t>(dis(gen))
    };
}

}
