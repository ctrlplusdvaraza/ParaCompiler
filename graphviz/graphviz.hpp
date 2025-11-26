#pragma once

#include <cxxabi.h>
#include <filesystem>
#include <iomanip>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include <ast.hpp>

namespace compiler::graphviz
{

enum class GraphRankdir
{
    TB,
    BT,
    LR,
    RL,
};

enum class NodeShape
{
    BOX,
    ELLIPSE,
    CIRCLE,
    TRIANGLE,
    DIAMOND,
    STAR,
    POLYGON,
    TRAPEZIUM,
    PARALLELOGRAM,
    HEXAGON,
    OCTAGON,
    POINT,
    EGG,
    HOUSE,
    PENTAGON,
    NOTE,
    TAB,
    FOLDER,
    BOX3D,
    COMPONENT,
    CYLINDER,
    RECORD,
    MRECORD,
};

enum class NodeStyle
{
    SOLID,
    DASHED,
    DOTTED,
    BOLD,
    FILLED,
    STRIPED,
    WEDGED,
    RADIAL,
    ROUNDED,
    DIAGONALS,
    INVISIBLE,
    TAPERED
};

struct Color
{
    uint8_t r, g, b;
};

using ColorTable = std::unordered_map<std::string, Color>;

struct DotNodeAttributes
{
    NodeShape shape;
    Color color;
    Color fill_color;
    NodeStyle style;
};

struct DotGraphAttributes
{
    GraphRankdir rankdir;
    NodeShape default_node_shape;
    Color default_node_color;
    NodeStyle default_node_style;
    size_t default_edge_pen_width;
    Color default_edge_color;
};

struct DotEdgeAttributes
{
    Color color;
    size_t penwidth;
};

struct DotNode
{
    DotNodeAttributes attributes;
    std::string type_label;
    std::string lexeme_label;
    uintptr_t identifier;
};

struct DotEdge
{
    uintptr_t start_node_id;
    uintptr_t end_node_id;

    DotEdgeAttributes attributes_;
};

inline auto escape_string(const std::string& str) { return std::quoted(str, ' '); }

inline void to_lower(std::string str)
{
    std::transform(str.begin(), str.end(), str.begin(),
                   [](unsigned char c) { return std::tolower(c); });
}

inline uint8_t get_random_byte() { return 56 + std::rand() % 200; }

inline Color generate_random_color()
{
    return Color{get_random_byte(), get_random_byte(), get_random_byte()};
}

template <auto V>
constexpr auto get_enum_name_static()
{
#if defined(__clang) || defined(__GNUC__)
    std::string_view name = __PRETTY_FUNCTION__;
    auto start = name.find_last_of("::") + 1;
    auto end = name.find_last_of(']');
    return name.substr(start, end - start);
#elif
#error get_enum_name: compiler does not support the enum trick
#endif
}

inline const constexpr std::size_t MAX_ENUM_CNT = 255;

template <typename E, std::size_t Min = 0, std::size_t Max = MAX_ENUM_CNT>
constexpr auto get_enum_name_dynamic(E val)
{
    std::string_view result = "Unknown enum member";
    using sequence = std::make_index_sequence<Max - Min + 1>;
    auto search = [&]<std::size_t... Is>(std::integer_sequence<std::size_t, Is...>) {
        (((static_cast<int>(val) == (Min + Is))
              ? (result = get_enum_name_static<static_cast<E>(Min + Is)>())
              : ""),
         ...);
    };

    search(sequence{});
    return result;
}

template <typename T>
std::ostream& enum_to_stream(std::ostream& stream, T val)
{
    std::string name(get_enum_name_dynamic(val));
    to_lower(name);
    stream << name;
    return stream;
}

inline std::string demangle(const std::string& mangled)
{
    int status = 0;
    auto dem = abi::__cxa_demangle(mangled.c_str(), nullptr, nullptr, &status);

    if (status != 0)
    {
        std::cerr << "demangling failed for name '" << mangled << "'" << std::endl;
        std::free(dem);
        return {};
    }

    std::string out = (status == 0 && dem) ? dem : mangled;
    std::free(dem);
    return out;
}

inline std::string strip_namespace(const std::string& s)
{
    size_t pos = s.rfind("::");
    return (pos == std::string::npos) ? s : s.substr(pos + 2);
}

inline std::string make_graphviz_filepath(const std::string& source_filepath)
{
    std::filesystem::path source_filepath_obj(source_filepath);
    std::filesystem::path ast_filepath_obj = source_filepath_obj.replace_extension(".svg");
    std::string ast_filepath = ast_filepath_obj.string();

    return ast_filepath;
}

class DotGraph
{
  public:
    static const constexpr DotGraphAttributes DEFAULT_DOT_GRAPH_ATTRIBUTES = {
        .rankdir = GraphRankdir::TB,
        .default_node_shape = NodeShape::MRECORD,
        .default_node_color = Color{0, 0, 0},
        .default_node_style = NodeStyle::FILLED,
        .default_edge_pen_width = 2,
        .default_edge_color = Color{0, 0, 0}};

    static const constexpr std::string TEMP_DOT_FILEPATH = "temp.dot";

    void create_from_ast_tree(const compiler::AstRootPtr& root);
    void convert_to_image(const std::string& image_path);

  private:
    DotNode create_dot_node(const compiler::AstNode* ast_node, ColorTable& node_color_table);
    DotEdge create_dot_edge(const compiler::AstNode* start_node, const compiler::AstNode* end_node);
    void create_from_ast_tree_recursive(const compiler::AstNode* root,
                                        ColorTable& node_color_table);

    DotGraphAttributes attributes_ = DEFAULT_DOT_GRAPH_ATTRIBUTES;
    std::vector<DotNode> nodes_{};
    std::vector<DotEdge> edges_{};
};


} // namespace compiler::graphviz
