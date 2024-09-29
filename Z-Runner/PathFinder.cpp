#include "PathFinder.h"
#include <algorithm> // for min_element, remove
#include <cmath> // for abs

float PathFinder::heuristic(const PathFindingNode& a, const PathFindingNode& b)
{
    return std::abs(a.x - b.x) + std::abs(a.y - b.y); // Manhattan distance
}

bool PathFinder::canMoveToPosition(int x, int y, int enemyWidth, int enemyHeight)
{
    for (int i = 0; i < enemyWidth; ++i) {
        for (int j = 0; j < enemyHeight; ++j) {
            int checkX = x + i;
            int checkY = y + j;
            if (checkX < 0 || checkX >= screenHeight || checkY < 0 || checkY >= screenHeight || !grid[checkX][checkY].walkable) {
                return false;
            }
        }
    }
    return true;
}

PathFinder::PathFinder(int screenWidth, int screenHeight)
{
    this->screenWidth = screenWidth;
    this->screenHeight = screenHeight; // Fixed assignment for screenHeight
}

std::vector<PathFindingNode*> PathFinder::findPath(PathFindingNode& startNode, PathFindingNode& targetNode, int enemyWidth, int enemyHeight, int jumpHeight, int jumpWidth, int fallDistance)
{
    std::vector<PathFindingNode*> openSet;
    std::vector<PathFindingNode*> closedSet;

    openSet.push_back(&startNode);

    while (!openSet.empty()) {
        auto currentNode = *std::min_element(openSet.begin(), openSet.end(), [](PathFindingNode* a, PathFindingNode* b) { return a->getFCost() < b->getFCost(); });

        if (*currentNode == targetNode) {
            std::vector<PathFindingNode*> path;
            while (currentNode != &startNode) {
                path.push_back(currentNode);
                currentNode = currentNode->parent;
            }
            std::reverse(path.begin(), path.end());
            return path;
        }

        openSet.erase(std::remove(openSet.begin(), openSet.end(), currentNode), openSet.end());
        closedSet.push_back(currentNode);

        std::vector<PathFindingNode*> neighbors;

        // Check left and right movements
        if (currentNode->x > 0 && canMoveToPosition(currentNode->x - 1, currentNode->y, enemyWidth, enemyHeight)) neighbors.push_back(&grid[currentNode->x - 1][currentNode->y]); // Left
        if (currentNode->x < screenWidth - 1 && canMoveToPosition(currentNode->x + 1, currentNode->y, enemyWidth, enemyHeight)) neighbors.push_back(&grid[currentNode->x + 1][currentNode->y]); // Right

        // Check for jumps within the max jump height and width
        for (int dx = -jumpWidth; dx <= jumpWidth; dx++) {
            for (int dy = 1; dy <= jumpHeight; dy++) {
                int nx = currentNode->x + dx;
                int ny = currentNode->y - dy; // Adjust y-axis for SFML coordinate system (decrement)
                if (nx >= 0 && nx < screenWidth && ny >= 0 && ny < screenHeight && canMoveToPosition(nx, ny, enemyWidth, enemyHeight)) {
                    neighbors.push_back(&grid[nx][ny]); // Jump
                }
            }
        }

        // Check for falls within the max fall distance
        for (int dy = 1; dy <= fallDistance; dy++) {
            int ny = currentNode->y + dy;
            if (ny < screenHeight && canMoveToPosition(currentNode->x, ny, enemyWidth, enemyHeight)) {
                neighbors.push_back(&grid[currentNode->x][ny]); // Fall
            }
        }

        for (auto neighbor : neighbors) {
            if (!neighbor->walkable || std::find(closedSet.begin(), closedSet.end(), neighbor) != closedSet.end()) continue;

            float newGCost = currentNode->gCost + heuristic(*currentNode, *neighbor);
            if (newGCost < neighbor->gCost || std::find(openSet.begin(), openSet.end(), neighbor) == openSet.end()) {
                neighbor->gCost = newGCost;
                neighbor->hCost = heuristic(*neighbor, targetNode);
                neighbor->parent = currentNode;

                if (std::find(openSet.begin(), openSet.end(), neighbor) == openSet.end()) {
                    openSet.push_back(neighbor);
                }
            }
        }
    }

    return std::vector<PathFindingNode*>(); // Return an empty path if no path is found
}

void PathFinder::setGrid(std::vector<std::vector<PathFindingNode>>& grid)
{
    this->grid = grid;
}
