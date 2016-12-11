
#ifndef LD37_PATHFINDING_HPP
#define LD37_PATHFINDING_HPP

#include <unordered_map>
#include <cstdint>
#include <functional>

struct ShortestPathData
{
    ShortestPathData();
    ShortestPathData(int32_t x, int32_t y, uint32_t priority);

    int32_t x;
    int32_t y;
    uint32_t priority;

    bool operator < (const ShortestPathData& other) const;
};

uint32_t heuristic(int32_t x0, int32_t y0, int32_t x1, int32_t y1);

std::unordered_map<uint64_t, uint64_t> shortestPath(
    int32_t startX, int32_t startY,
    int32_t goalX, int32_t goalY,
    std::function<bool(int32_t, int32_t)> isObstacle
);

#endif

