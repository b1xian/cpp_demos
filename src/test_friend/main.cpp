#include "iostream"
#include "memory"

// main.cpp
#include "Target.h"

int main() {
  std::shared_ptr<Target> target_ptr;
  target_ptr.reset(new Target);

  target_ptr->DoUpdate();
  return 0;
}
