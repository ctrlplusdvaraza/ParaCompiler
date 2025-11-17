#pragma once
#include <iostream>
#include "dot_io.hpp"

namespace graphviz
{

enum class GraphRankdir;
enum class NodeShape;
enum class NodeStyle;
struct Color;
class DotEdge;
class DotNode;

std::ostream &operator<<(std::ostream &stream, const GraphRankdir rankdir);
std::ostream &operator<<(std::ostream &stream, NodeShape shape);
std::ostream &operator<<(std::ostream &stream, NodeStyle style);
std::ostream &operator<<(std::ostream &stream, const Color color);
std::ostream &operator<<(std::ostream &stream, const DotEdge &edge);
std::ostream &operator<<(std::ostream &stream, const DotNode &node);

}
