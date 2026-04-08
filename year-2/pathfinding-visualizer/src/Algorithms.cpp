#include "Algorithms.hpp"

#include "MinHeap.hpp"
#include "Queue.hpp"
#include "Stack.hpp"

#include <climits>

static std::vector<int> ReconstructPath(int start, int goal, const std::vector<int>& prev) {
  if (start == goal) return std::vector<int>{start};
  if (goal < 0 || goal >= (int)prev.size()) return {};
  if (prev[(size_t)goal] == -1) return {};

  IntStack st;
  st.Reset((int)prev.size());

  int cur = goal;
  while (cur != -1) {
    st.Push(cur);
    if (cur == start) break;
    cur = prev[(size_t)cur];
  }
  if (cur != start) return {};

  std::vector<int> path;
  while (!st.Empty()) path.push_back(st.Pop());
  return path;
}

PathResult RunBfs(const std::vector<std::vector<int>>& adj, int start, int goal) {
  PathResult res;
  const int n = (int)adj.size();
  if (start < 0 || start >= n || goal < 0 || goal >= n) return res;

  std::vector<int> prev((size_t)n, -1);
  std::vector<char> seen((size_t)n, 0);

  IntQueue q;
  q.Reset(n * 2 + 8);
  q.Push(start);
  seen[(size_t)start] = 1;

  while (!q.Empty()) {
    int u = q.Pop();
    res.visit_order.push_back(u);
    if (u == goal) break;
    for (int v : adj[(size_t)u]) {
      if (seen[(size_t)v]) continue;
      seen[(size_t)v] = 1;
      prev[(size_t)v] = u;
      q.Push(v);
    }
  }

  res.path = ReconstructPath(start, goal, prev);
  res.found = !res.path.empty();
  return res;
}

PathResult RunDijkstra(const std::vector<std::vector<int>>& adj, int start, int goal) {
  PathResult res;
  const int n = (int)adj.size();
  if (start < 0 || start >= n || goal < 0 || goal >= n) return res;

  std::vector<int> dist((size_t)n, INT_MAX / 4);
  std::vector<int> prev((size_t)n, -1);
  std::vector<char> done((size_t)n, 0);

  MinHeap heap;
  heap.Reset(n * 8 + 32);

  dist[(size_t)start] = 0;
  heap.Push(0, start);

  while (!heap.Empty()) {
    HeapItem it = heap.Pop();
    int d = it.key;
    int u = it.value;
    if (u < 0 || u >= n) continue;
    if (done[(size_t)u]) continue;
    if (d != dist[(size_t)u]) continue;

    done[(size_t)u] = 1;
    res.visit_order.push_back(u);
    if (u == goal) break;

    for (int v : adj[(size_t)u]) {
      int nd = d + 1;  // uniform grid cost
      if (nd < dist[(size_t)v]) {
        dist[(size_t)v] = nd;
        prev[(size_t)v] = u;
        heap.Push(nd, v);
      }
    }
  }

  res.path = ReconstructPath(start, goal, prev);
  res.found = !res.path.empty();
  return res;
}

static int manhattan(int cols, int a, int b) {
  int ax = a % cols;
  int ay = a / cols;
  int bx = b % cols;
  int by = b / cols;
  int dx = ax - bx;
  if (dx < 0) dx = -dx;
  int dy = ay - by;
  if (dy < 0) dy = -dy;
  return dx + dy;
}

PathResult RunAStarGrid(const std::vector<std::vector<int>>& adj, int cols, int rows, int start, int goal) {
  (void)rows;
  PathResult res;
  const int n = (int)adj.size();
  if (start < 0 || start >= n || goal < 0 || goal >= n) return res;

  std::vector<int> g((size_t)n, INT_MAX / 4);
  std::vector<int> prev((size_t)n, -1);
  std::vector<char> done((size_t)n, 0);

  MinHeap heap;
  heap.Reset(n * 12 + 32);

  g[(size_t)start] = 0;
  heap.Push(manhattan(cols, start, goal), start);

  while (!heap.Empty()) {
    HeapItem it = heap.Pop();
    int u = it.value;
    if (u < 0 || u >= n) continue;
    if (done[(size_t)u]) continue;
    done[(size_t)u] = 1;
    res.visit_order.push_back(u);
    if (u == goal) break;

    for (int v : adj[(size_t)u]) {
      int ng = g[(size_t)u] + 1;
      if (ng < g[(size_t)v]) {
        g[(size_t)v] = ng;
        prev[(size_t)v] = u;
        int f = ng + manhattan(cols, v, goal);
        heap.Push(f, v);
      }
    }
  }

  res.path = ReconstructPath(start, goal, prev);
  res.found = !res.path.empty();
  return res;
}

