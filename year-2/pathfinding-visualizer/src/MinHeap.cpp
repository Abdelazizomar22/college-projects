#include "MinHeap.hpp"

#include <cstdlib>

MinHeap::MinHeap() = default;
MinHeap::~MinHeap() { std::free(items_); }

void MinHeap::Reset(int capacity) {
  std::free(items_);
  items_ = (HeapItem*)std::malloc(sizeof(HeapItem) * (size_t)capacity);
  cap_ = capacity;
  size_ = 0;
}

bool MinHeap::Empty() const { return size_ == 0; }

void MinHeap::Push(int key, int value) {
  if (size_ >= cap_) return;  // best-effort
  items_[size_] = HeapItem{key, value};
  SiftUp(size_);
  size_++;
}

HeapItem MinHeap::Pop() {
  HeapItem out = items_[0];
  size_--;
  if (size_ > 0) {
    items_[0] = items_[size_];
    SiftDown(0);
  }
  return out;
}

void MinHeap::SiftUp(int i) {
  while (i > 0) {
    int p = (i - 1) / 2;
    if (items_[p].key <= items_[i].key) break;
    HeapItem tmp = items_[p];
    items_[p] = items_[i];
    items_[i] = tmp;
    i = p;
  }
}

void MinHeap::SiftDown(int i) {
  while (true) {
    int l = 2 * i + 1;
    int r = 2 * i + 2;
    int m = i;
    if (l < size_ && items_[l].key < items_[m].key) m = l;
    if (r < size_ && items_[r].key < items_[m].key) m = r;
    if (m == i) break;
    HeapItem tmp = items_[m];
    items_[m] = items_[i];
    items_[i] = tmp;
    i = m;
  }
}

