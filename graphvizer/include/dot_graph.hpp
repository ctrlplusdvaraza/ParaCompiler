#pragma once

#include <cassert>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <memory>
#include <random>
#include <sstream>
#include <unordered_map>
#include <vector>

#include "ast.hpp"
#include "dot_common.hpp"
#include "dot_io.hpp"

namespace compiler::graphviz
{

class DotNode;

using DotNodePtr = std::unique_ptr<DotNode>;

enum class GraphRankdir
{
    RANKDIR_TB,
    RANKDIR_BT,
    RANKDIR_LR,
    RANKDIR_RL,
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
    std::size_t children_cnt;
    std::string label;
    uintptr_t identifier;
};

struct DotEdge
{
    uintptr_t start_node_id;
    std::size_t start_child_id;
    uintptr_t end_node_id;

    DotEdgeAttributes attributes_;
};

class DotGraph
{
    static constexpr DotGraphAttributes DEFAULT_DOT_GRAPH_ATTRIBUTES = {
        .rankdir = GraphRankdir::RANKDIR_TB,
        .default_node_shape = NodeShape::MRECORD,
        .default_node_color = Color{0, 0, 0},
        .default_node_style = NodeStyle::FILLED,
        .default_edge_pen_width = 2,
        .default_edge_color = Color{0, 255, 0}};

    DotGraphAttributes attributes_ = DEFAULT_DOT_GRAPH_ATTRIBUTES;
    std::vector<DotNode> nodes_{};
    std::vector<DotEdge> edges_{};

  public:
    void create_from_ast_tree(const compiler::AstRootPtr& root)
    {
        if (!root)
            return;

        std::unordered_map<const char*, Color> node_color_table{};
        create_from_ast_tree_recursive(root.get(), node_color_table);
    }

    void convert_to_image(const char* image_path)
    {
        std::ofstream dot_file(image_path, std::ios::binary);
        if (!dot_file)
        {
            std::cerr << "failed to create dot_file ifstream\n";
            return;
        }

        dot_file << "digraph G {\n";
        dot_file << "    rankdir=" << attributes_.rankdir << ";\n";

        for (const auto& node : nodes_)
            dot_file << "    " << node << "\n";
        for (const auto& edge : edges_)
            dot_file << "    " << edge << "\n";

        dot_file << "}\n";

        std::string image_gen_command =
            "dot ./" + std::string(image_path) + " -Tpng -o ./" + std::string(image_path) + "\n";
        int rc = std::system(image_gen_command.c_str());
        if (rc != 0)
            throw std::runtime_error("dot image generation failed");
    }

  private:
    DotNode create_dot_node(const compiler::AstNode* ast_node,
                            std::unordered_map<const char*, Color>& node_color_table)
    {
        assert(ast_node);

        const char* ast_node_type_name = typeid(*ast_node).name();
        if (node_color_table.find(ast_node_type_name) == node_color_table.end())
            node_color_table[ast_node_type_name] = generate_random_color();

        DotNode dot_node{};
        dot_node.attributes.shape = attributes_.default_node_shape;
        dot_node.attributes.color = attributes_.default_node_color;
        dot_node.attributes.fill_color = node_color_table[ast_node_type_name];
        dot_node.attributes.style = attributes_.default_node_style;

        dot_node.children_cnt = ast_node->children.size();
        dot_node.label = ast_node->get_string_lexeme();
        dot_node.identifier = reinterpret_cast<uintptr_t>(ast_node);

        return dot_node;
    }

    DotEdge create_dot_edge(const compiler::AstNode* start_node, const std::size_t start_child_id,
                            const compiler::AstNode* end_node)
    {
        DotEdge edge{};
        edge.attributes_.color = attributes_.default_edge_color;
        edge.attributes_.penwidth = attributes_.default_edge_pen_width;
        edge.start_node_id = reinterpret_cast<uintptr_t>(start_node);
        edge.start_child_id = start_child_id;
        edge.end_node_id = reinterpret_cast<uintptr_t>(end_node);

        return edge;
    }

    void create_from_ast_tree_recursive(const compiler::AstNode* root,
                                        std::unordered_map<const char*, Color>& node_color_table)
    {
        assert(root);

        nodes_.push_back(create_dot_node(root, node_color_table));

        for (std::size_t child_id = 0; child_id < root->children.size(); child_id++)
        {
            const auto& child = root->children[child_id];
            edges_.push_back(create_dot_edge(root, child_id, child.get()));

            create_from_ast_tree_recursive(child.get(), node_color_table);
        }
    }
};

} // namespace compiler::graphviz
