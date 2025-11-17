#pragma once

#include <memory>
#include <vector>
#include <unordered_map>
#include <cassert>
#include <cstdint>
#include <random>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <iostream>

#include "ast.hpp"
#include "dot_io.hpp"
#include "dot_common.hpp"

namespace graphviz 
{

class DotNode;

using DotNodePtr = std::unique_ptr<DotNode>;

enum class GraphRankdir {
    RANKDIR_TB, // Top to Bottom
    RANKDIR_BT, // Bottom to Top
    RANKDIR_LR, // Left to Right
    RANKDIR_RL, // Right to Left
};

enum class NodeShape {
    // Basic geometric shapes
    BOX,
    ELLIPSE,
    CIRCLE,
    TRIANGLE,
    DIAMOND,
    STAR,
    
    // Polygon-based shapes
    POLYGON,
    TRAPEZIUM,
    PARALLELOGRAM,
    HEXAGON,
    OCTAGON,
    
    // Arrow-like shapes
    POINT,
    EGG,
    HOUSE,
    PENTAGON,
    
    // Special shapes
    NOTE,
    TAB,
    FOLDER,
    BOX3D,
    COMPONENT,
    CYLINDER,
    
    // Record-based shapes
    RECORD,
    MRECORD,
};

enum class NodeStyle {
    // Line styles
    SOLID,
    DASHED,
    DOTTED,
    BOLD,
    
    // Fill patterns
    FILLED,
    STRIPED,
    WEDGED,
    RADIAL,
    
    // Border/corner styles
    ROUNDED,
    DIAGONALS,
    
    // Special styles
    INVISIBLE,
    
    // Line endings
    TAPERED
};

struct Color {
    uint8_t r, g, b;
};

struct DotNodeAttributes {
    NodeShape shape;
    Color     color;
    Color     fillColor;
    NodeStyle style;
};

struct DotGraphAttributes {
    GraphRankdir rankdir;
    NodeShape    defaultNodeShape;
    Color        defaultNodeColor;
    NodeStyle    defaultNodeStyle;

    size_t       defaultEdgePenWidth; 
    Color        defaultEdgeColor;
};

struct DotEdgeAttributes {
    Color  color;
    size_t penwidth;
};

struct DotNode {
    DotNodeAttributes attributes;
    std::size_t       childrenCnt;
    std::string       label;
    uintptr_t         identifier;
};

struct DotEdge {
    uintptr_t   startNodeId;
    std::size_t startChildId;
    uintptr_t   endNodeId;

    DotEdgeAttributes attributes_;
};


class DotGraph {
    static constexpr DotGraphAttributes DEFAULT_DOT_GRAPH_ATTRIBUTES = 
    {
        .rankdir             = GraphRankdir::RANKDIR_TB,
        .defaultNodeShape    = NodeShape::MRECORD,
        .defaultNodeColor    = Color{0, 0, 0},
        .defaultNodeStyle    = NodeStyle::FILLED,

        .defaultEdgePenWidth = 2,
        .defaultEdgeColor    = Color{0, 255, 0}
    };

    DotGraphAttributes attributes_ = DEFAULT_DOT_GRAPH_ATTRIBUTES;
    std::vector<DotNode> nodes_{};
    std::vector<DotEdge> edges_{};

  public:
    // set attributes
    void create_from_ast_tree(const compiler::AstRootPtr& root) {
        if (!root) return;

        std::unordered_map<const char*, Color> nodeColorTable{};
        create_from_ast_tree_recursive(root.get(), nodeColorTable);
    }

    void convert_to_image(const char *imagePath) {
        std::ofstream dotFile(imagePath, std::ios::binary);
        if (!dotFile) {
            std::cerr << "failed to create dotFile ifstream\n";
            return;
        }

        dotFile << "digraph G {\n";
        dotFile << "    rankdir=" << attributes_.rankdir << ";\n";

        for (const auto &node : nodes_) dotFile << "    " << node << "\n";
        for (const auto &edge : nodes_) dotFile << "    " << edge << "\n";
        
        dotFile << "}\n";

        std::string imageGenCommand = "dot " + std::string(imagePath) + " -o " + std::string(imagePath) + "\n";
        int rc = std::system(imageGenCommand.c_str());

        if (rc != 0) throw std::runtime_error("dot image generation failed");
    }

  private:

    DotNode create_dot_node(const compiler::AstNode* astNode, 
                            std::unordered_map<const char*, Color> &nodeColorTable) {
        assert(astNode);

        const char* astNodeTypeName = typeid(*astNode).name();                          
        if (nodeColorTable.find(astNodeTypeName) == nodeColorTable.end())
            nodeColorTable[astNodeTypeName] = generate_random_color();

        DotNode dotNode{};
        dotNode.attributes.shape     = attributes_.defaultNodeShape;
        dotNode.attributes.color     = attributes_.defaultNodeColor;
        dotNode.attributes.fillColor = nodeColorTable[astNodeTypeName];
        dotNode.attributes.style     = attributes_.defaultNodeStyle;
        
        dotNode.childrenCnt          = astNode->children.size();
        dotNode.label                = astNode->get_string_lexeme();
        dotNode.identifier           = reinterpret_cast<uintptr_t>(astNode);

        return dotNode;
    }

    DotEdge create_dot_edge(const compiler::AstNode *startNode, const std::size_t startChildId,
                            const compiler::AstNode *endNode) {
        DotEdge edge{};
        edge.attributes_.color    = attributes_.defaultEdgeColor;
        edge.attributes_.penwidth = attributes_.defaultEdgePenWidth;
        edge.startNodeId          = reinterpret_cast<uintptr_t>(startNode);
        edge.startChildId         = startChildId;
        edge.endNodeId            = reinterpret_cast<uintptr_t>(endNode);

        return edge;
    }

    void create_from_ast_tree_recursive(const compiler::AstNode* root, 
                                        std::unordered_map<const char*, Color> &nodeColorTable) {
        assert(root);
    
        nodes_.push_back(create_dot_node(root, nodeColorTable));

        for (std::size_t childId = 0; childId < root->children.size(); childId++) {
            const auto& child = root->children[childId];
            edges_.push_back(create_dot_edge(root, childId, child.get()));

            create_from_ast_tree_recursive(child.get(), nodeColorTable);
        }
    }
};

} // namespace graphviz
