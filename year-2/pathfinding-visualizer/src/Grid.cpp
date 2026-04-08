#include "Grid.hpp"

Grid::Grid(int cols, int rows) : cols_(cols), rows_(rows) {
  cells_.assign((size_t)(cols_ * rows_), CellType::Empty);
  start_idx_ = 0;
  end_idx_ = cols_ * rows_ - 1;
  cells_[start_idx_] = CellType::Start;
  cells_[end_idx_] = CellType::End;
}

void Grid::SetStart(int idx) {
  if (idx < 0 || idx >= Count()) return;
  if (idx == end_idx_) return;
  if (cells_[idx] == CellType::Wall) return;
  if (start_idx_ >= 0 && start_idx_ < Count()) {
    cells_[start_idx_] = CellType::Empty;
  }
  start_idx_ = idx;
  cells_[start_idx_] = CellType::Start;
}

void Grid::SetEnd(int idx) {
  if (idx < 0 || idx >= Count()) return;
  if (idx == start_idx_) return;
  if (cells_[idx] == CellType::Wall) return;
  if (end_idx_ >= 0 && end_idx_ < Count()) {
    cells_[end_idx_] = CellType::Empty;
  }
  end_idx_ = idx;
  cells_[end_idx_] = CellType::End;
}

void Grid::ClearVisitedAndPath() {
  for (int i = 0; i < Count(); i++) {
    if (cells_[i] == CellType::Visited || cells_[i] == CellType::Path) {
      cells_[i] = CellType::Empty;
    }
  }
  cells_[start_idx_] = CellType::Start;
  cells_[end_idx_] = CellType::End;
}

void Grid::ClearAll() {
  cells_.assign((size_t)Count(), CellType::Empty);
  start_idx_ = 0;
  end_idx_ = Count() - 1;
  cells_[start_idx_] = CellType::Start;
  cells_[end_idx_] = CellType::End;
}

std::vector<std::vector<int>> Grid::BuildAdjacencyList() const {
  std::vector<std::vector<int>> adj((size_t)Count());
  for (int y = 0; y < rows_; y++) {
    for (int x = 0; x < cols_; x++) {
      int u = Index(x, y);
      if (IsWall(u)) continue;

      const int dx[4] = {1, -1, 0, 0};
      const int dy[4] = {0, 0, 1, -1};
      for (int k = 0; k < 4; k++) {
        int nx = x + dx[k];
        int ny = y + dy[k];
        if (!InBounds(nx, ny)) continue;
        int v = Index(nx, ny);
        if (IsWall(v)) continue;
        adj[(size_t)u].push_back(v);
      }
    }
  }
  return adj;
}

