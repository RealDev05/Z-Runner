#pragma once

#include <vector>
#include <algorithm>
#include <iostream>
#include "PathFindingNode.h"

using namespace std;
class PathFinder
{
private:
	int screenWidth;
	int screenHeight;
	vector<vector<PathFindingNode>> grid;

	float heuristic(const PathFindingNode& a, const PathFindingNode& b);
	bool canMoveToPosition(int x, int y, int enemyWidth, int enemyHeight);
public:
	PathFinder(int screenWidth,int screenHeight);

	vector<PathFindingNode*> findPath(PathFindingNode& startNode, PathFindingNode& targetNode, int enemyWidth, int enemyHeight, int jumpHeight, int jumpWidth, int fallDistance);

	void setGrid(vector<vector<PathFindingNode>>& grid);
};

