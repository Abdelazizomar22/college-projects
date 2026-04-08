#include "Stack.hpp"

#include <cstdlib>

IntStack::IntStack() = default;
IntStack::~IntStack() { std::free(items_); }

void IntStack::Reset(int capacity) {
  std::free(items_);
  items_ = (int*)std::malloc(sizeof(int) * (size_t)capacity);
  cap_ = capacity;
  size_ = 0;
}

bool IntStack::Empty() const { return size_ == 0; }

void IntStack::Push(int v) { items_[size_++] = v; }

int IntStack::Pop() { return items_[--size_]; }

