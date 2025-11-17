#include <iostream>

#include "dot_graph.hpp"
#include "dot_io.hpp"

namespace compiler::graphviz
{

std::ostream& operator<<(std::ostream& stream, const GraphRankdir rankdir)
{
    switch (rankdir)
    {
        case (GraphRankdir::RANKDIR_TB):
            stream << "TB";
            break;
        case (GraphRankdir::RANKDIR_BT):
            stream << "BT";
            break;
        case (GraphRankdir::RANKDIR_LR):
            stream << "LR";
            break;
        case (GraphRankdir::RANKDIR_RL):
            stream << "RL";
            break;
        default:
            stream << "unknown_GraphRankdir";
            break;
    }

    return stream;
}

std::ostream& operator<<(std::ostream& stream, NodeShape shape)
{
    switch (shape)
    {
        case NodeShape::BOX:
            stream << "box";
            break;
        case NodeShape::ELLIPSE:
            stream << "ellipse";
            break;
        case NodeShape::CIRCLE:
            stream << "circle";
            break;
        case NodeShape::TRIANGLE:
            stream << "triangle";
            break;
        case NodeShape::DIAMOND:
            stream << "diamond";
            break;
        case NodeShape::STAR:
            stream << "star";
            break;

        case NodeShape::POLYGON:
            stream << "polygon";
            break;
        case NodeShape::TRAPEZIUM:
            stream << "trapezium";
            break;
        case NodeShape::PARALLELOGRAM:
            stream << "parallelogram";
            break;
        case NodeShape::HEXAGON:
            stream << "hexagon";
            break;
        case NodeShape::OCTAGON:
            stream << "octagon";
            break;

        case NodeShape::POINT:
            stream << "point";
            break;
        case NodeShape::EGG:
            stream << "egg";
            break;
        case NodeShape::HOUSE:
            stream << "house";
            break;
        case NodeShape::PENTAGON:
            stream << "pentagon";
            break;

        case NodeShape::NOTE:
            stream << "note";
            break;
        case NodeShape::TAB:
            stream << "tab";
            break;
        case NodeShape::FOLDER:
            stream << "folder";
            break;
        case NodeShape::BOX3D:
            stream << "box3d";
            break;
        case NodeShape::COMPONENT:
            stream << "component";
            break;
        case NodeShape::CYLINDER:
            stream << "cylinder";
            break;

        case NodeShape::RECORD:
            stream << "record";
            break;
        case NodeShape::MRECORD:
            stream << "Mrecord";
            break;

        default:
            stream << "unknown_NodeShape";
            break;
    }
    return stream;
}

std::ostream& operator<<(std::ostream& stream, NodeStyle style)
{
    switch (style)
    {
        case NodeStyle::SOLID:
            stream << "solid";
            break;
        case NodeStyle::DASHED:
            stream << "dashed";
            break;
        case NodeStyle::DOTTED:
            stream << "dotted";
            break;
        case NodeStyle::BOLD:
            stream << "bold";
            break;

        case NodeStyle::FILLED:
            stream << "filled";
            break;
        case NodeStyle::STRIPED:
            stream << "striped";
            break;
        case NodeStyle::WEDGED:
            stream << "wedged";
            break;
        case NodeStyle::RADIAL:
            stream << "radial";
            break;

        case NodeStyle::ROUNDED:
            stream << "rounded";
            break;
        case NodeStyle::DIAGONALS:
            stream << "diagonals";
            break;

        case NodeStyle::INVISIBLE:
            stream << "invis";
            break;

        case NodeStyle::TAPERED:
            stream << "tapered";
            break;

        default:
            stream << "unknown_NodeStyle";
            break;
    }
    return stream;
}

std::ostream& operator<<(std::ostream& stream, const Color color)
{
    char buf[8] = {};
    std::snprintf(buf, sizeof(buf), "%02X%02X%02X", color.r, color.g, color.b);
    return stream << buf;
}

std::ostream& operator<<(std::ostream& stream, const DotNode& node)
{
    stream << "NODE_" << node.identifier << "[";

    stream << "shape=\"" << node.attributes.shape << "\",";
    stream << "color=\"#" << node.attributes.color << "\",";
    stream << "style=\"" << node.attributes.style << "\",";
    stream << "fillcolor=\"#" << node.attributes.fill_color << "\",";

    stream << "label=\"{";
    stream << "\'" << node.label << "\' | {";
    for (std::size_t childId = 0; childId < node.children_cnt; childId++)
    {
        stream << "<C" << childId << "> (C" << childId << ")";
        if (childId + 1 != node.children_cnt) { stream << " | "; }
    }

    stream << "}}\"]";

    return stream;
}

std::ostream& operator<<(std::ostream& stream, const DotEdge& edge)
{
    stream << "NODE_" << edge.start_node_id;
    if (edge.start_child_id != std::numeric_limits<std::size_t>::max())
    {
        stream << ":<C" << edge.start_child_id << ">";
    }

    stream << " -> ";

    stream << "NODE_" << edge.end_node_id;

    stream << " [";
    stream << "color=\"#" << edge.attributes_.color << "\"";
    stream << ",penwidth=" << edge.attributes_.penwidth;
    stream << ",label=\"\"";
    stream << "]";

    return stream;
}

} // namespace compiler::graphviz
