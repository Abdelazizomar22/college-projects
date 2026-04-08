#pragma once

// Simple min-heap priority queue (integer key + integer value).
struct HeapItem {
  int key;
  int value;
};

class MinHeap {
 public:
  MinHeap();
  ~MinHeap();

  void Reset(int capacity);
  bool Empty() const;
  void Push(int key, int value);
  HeapItem Pop();

 private:
  HeapItem* items_{nullptr};
  int cap_{0};
  int size_{0};

  void SiftUp(int i);
  void SiftDown(int i);
};

