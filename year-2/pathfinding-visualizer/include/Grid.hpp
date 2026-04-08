#pragma once

#include "Types.hpp"

#include <vector>

class Grid {
 public:
  Grid(int cols, int rows);

  int Cols() const { return cols_; }
  int Rows() const { return rows_; }
  int Count() const { return cols_ * rows_; }

  int Index(int x, int y) const { return y * cols_ + x; }
  bool InBounds(int x, int y) const { return x >= 0 && x < cols_ && y >= 0 && y < rows_; }

  CellType Get(int idx) const { return cells_[idx]; }
  void Set(int idx, CellType t) { cells_[idx] = t; }

  void ClearVisitedAndPath();
  void ClearAll();

  int Start() const { return start_idx_; }
  int End() const { return end_idx_; }
  void SetStart(int idx);
  void SetEnd(int idx);

  bool IsWall(int idx) const { return cells_[idx] == CellType::Wall; }

  std::vector<std::vector<int>> BuildAdjacencyList() const;

 private:
  int cols_{};
  int rows_{};
  std::vector<CellType> cells_;
  int start_idx_{0};
  int end_idx_{0};
};

