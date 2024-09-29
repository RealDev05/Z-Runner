#include "PathFindingNode.h"

PathFindingNode::PathFindingNode(int x, int y, bool walkable) : x(x), y(y), walkable(walkable), gCost(0), hCost(0), parent(nullptr) {}


float PathFindingNode::getFCost() const
{
    return gCost + hCost;
}

bool PathFindingNode::operator==(const PathFindingNode& other) const
{
	return x == other.x && y == other.y;
}
