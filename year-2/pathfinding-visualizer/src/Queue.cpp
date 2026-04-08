#include "Queue.hpp"

#include <cstdlib>

IntQueue::IntQueue() = default;
IntQueue::~IntQueue() { std::free(items_); }

void IntQueue::Reset(int capacity) {
  std::free(items_);
  items_ = (int*)std::malloc(sizeof(int) * (size_t)capacity);
  cap_ = capacity;
  head_ = 0;
  tail_ = 0;
}

bool IntQueue::Empty() const { return head_ == tail_; }

void IntQueue::Push(int v) {
  items_[tail_] = v;
  tail_ = (tail_ + 1) % cap_;
}

int IntQueue::Pop() {
  int v = items_[head_];
  head_ = (head_ + 1) % cap_;
  return v;
}

