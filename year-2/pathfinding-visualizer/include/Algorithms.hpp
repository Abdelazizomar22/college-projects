#pragma once

#include <vector>

struct PathResult {
  bool found{false};
  std::vector<int> visit_order;
  std::vector<int> path;
};

PathResult RunBfs(const std::vector<std::vector<int>>& adj, int start, int goal);
PathResult RunDijkstra(const std::vector<std::vector<int>>& adj, int start, int goal);
PathResult RunAStarGrid(const std::vector<std::vector<int>>& adj, int cols, int rows, int start, int goal);

