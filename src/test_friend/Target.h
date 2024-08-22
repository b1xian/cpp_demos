//
// Created by bigaoxian on 8/16/24.
//

#ifndef CPP11_DEMO_TARGET_H
#define CPP11_DEMO_TARGET_H

// Target.h
#include <memory>

#include "VeloManager.h"

class Target {
 public:
  friend VeloManager;

  Target();

  void DoUpdate();
  void InitVelo();

 private:
  std::unique_ptr<VeloManager> veloManager_;

  double init_dx;
  double init_dy;
};



#endif //CPP11_DEMO_TARGET_H
