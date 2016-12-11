
#include "PathFinding.hpp"

#include <queue>
#include <cmath>

ShortestPathData::ShortestPathData() :
x(0),
y(0),
priority(0)
{}

ShortestPathData::ShortestPathData(
    int32_t x,
    int32_t y,
    uint32_t priority) :
x(x),
y(y),
priority(priority)
{}

bool ShortestPathData::operator < (const ShortestPathData& other) const
{
    return priority < other.priority;
}

uint32_t heuristic(int32_t x0, int32_t y0, int32_t x1, int32_t y1)
{
    return std::abs(x0 - x1) + std::abs(y0 - y1);
}

std::unordered_map<uint64_t, uint64_t> shortestPath(
    int32_t startX, int32_t startY,
    int32_t goalX, int32_t goalY,
    std::function<bool(int32_t, int32_t)> isObstacle
)
{
    std::unordered_map<uint64_t, uint64_t> cameFrom;
    std::unordered_map<uint64_t, uint32_t> costSoFar;
    std::priority_queue<ShortestPathData> frontier;

    uint64_t currentPos;
    ((int32_t*) &currentPos)[0] = startX;
    ((int32_t*) &currentPos)[1] = startY;
    frontier.push(ShortestPathData(startX, startY, 0));
    cameFrom.insert(std::make_pair(
        currentPos,
        currentPos
    ));
    costSoFar.insert(std::make_pair(
        currentPos,
        0
    ));

    while(!frontier.empty())
    {
        ShortestPathData current = frontier.top();
        frontier.pop();
        ((int32_t*) &currentPos)[0] = current.x;
        ((int32_t*) &currentPos)[1] = current.y;

        if(current.x == goalX && current.y == goalY)
        {
            break;
        }

        ShortestPathData nextData;
        uint64_t nextPos;
        uint32_t newCost = costSoFar.at(currentPos) + 1;
        for(unsigned int i = 0; i < 4; ++i)
        {
            switch(i)
            {
            case 0: // left
                nextData.x = current.x - 1;
                nextData.y = current.y;
                ((int32_t*) &nextPos)[0] = nextData.x;
                ((int32_t*) &nextPos)[1] = nextData.y;
                break;
            case 1: // right
                nextData.x = current.x + 1;
                nextData.y = current.y;
                ((int32_t*) &nextPos)[0] = nextData.x;
                ((int32_t*) &nextPos)[1] = nextData.y;
                break;
            case 2: // up
                nextData.x = current.x;
                nextData.y = current.y - 1;
                ((int32_t*) &nextPos)[0] = nextData.x;
                ((int32_t*) &nextPos)[1] = nextData.y;
                break;
            case 3: // down
                nextData.x = current.x;
                nextData.y = current.y + 1;
                ((int32_t*) &nextPos)[0] = nextData.x;
                ((int32_t*) &nextPos)[1] = nextData.y;
                break;
            }

            if(!isObstacle(nextData.x, nextData.y) &&
                (costSoFar.find(nextPos) == costSoFar.end() ||
                newCost < costSoFar.at(nextPos)))
            {
                costSoFar.insert(std::make_pair(nextPos, newCost));
                nextData.priority = -newCost - heuristic(nextData.x, nextData.y, goalX, goalY);
                frontier.push(nextData);
                cameFrom.insert(std::make_pair(nextPos, currentPos));
            }
        }
    }

    return cameFrom;
}

std::unordered_map<uint64_t, uint64_t> invertPathMap(
    const std::unordered_map<uint64_t, uint64_t>& map,
    int32_t goalX, int32_t goalY
)
{
    uint64_t goal;
    ((int32_t*) &goal)[0] = goalX;
    ((int32_t*) &goal)[1] = goalY;
    return invertPathMap(map, goal);
}

std::unordered_map<uint64_t, uint64_t> invertPathMap(
    const std::unordered_map<uint64_t, uint64_t>& map,
    uint64_t goal
)
{
    std::unordered_map<uint64_t, uint64_t> newMap;
    uint64_t current = goal;
    uint64_t next = map.at(goal);
    newMap.insert(std::make_pair(current, current));
    do {
        newMap.insert(std::make_pair(next, current));
        current = next;
        next = map.at(current);
    } while (current != next);

    return newMap;
}

