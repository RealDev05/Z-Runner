#pragma once
class PathFindingNode
{
public :
    int x, y;
    bool walkable;
    float gCost, hCost;
    PathFindingNode* parent;

    PathFindingNode(int x, int y, bool walkable);

    float getFCost() const;

    bool operator==(const PathFindingNode& other) const;
};

