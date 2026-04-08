#pragma once

// Simple integer stack.
class IntStack {
 public:
  IntStack();
  ~IntStack();

  void Reset(int capacity);
  bool Empty() const;
  void Push(int v);
  int Pop();

 private:
  int* items_{nullptr};
  int cap_{0};
  int size_{0};
};

