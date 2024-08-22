// Target.cpp
#include "Target.h"

Target::Target() {
//  veloManager_.reset(new VeloManager);
  veloManager_.reset(new VeloManager(this));
  init_dx = 1.0;
  init_dy = 2.0;
}

void Target::DoUpdate() {
  InitVelo();
}

void Target::InitVelo() {
  printf("Target, InitVelo~~~~~~~\n");
//  veloManager_->InitVelo();
  veloManager_->InitVelo(this);
}

