#pragma once

// Simple integer queue (circular buffer).
class IntQueue {
 public:
  IntQueue();
  ~IntQueue();

  void Reset(int capacity);
  bool Empty() const;
  void Push(int v);
  int Pop();

 private:
  int* items_{nullptr};
  int cap_{0};
  int head_{0};
  int tail_{0};
};

