// VeloManager.cpp
#include "VeloManager.h"
#include "Target.h"

void VeloManager::InitVelo(Target* target) {
  // 现在你可以访问到Target的私有变量 init_dx 和 init_dy
  if (target) {
    printf("VeloManager, InitVelo~~~~~~~\n");
    printf("init_dx = %f, init_dy = %f\n", target->init_dx, target->init_dy);
  }
}


void VeloManager::InitVelo() {
  // 现在你可以访问到Target的私有变量 init_dx 和 init_dy
  if (target_) {
    printf("VeloManager, InitVelo~~~~~~~\n");
    printf("init_dx = %f, init_dy = %f\n", target_->init_dx, target_->init_dy);
  }
}

