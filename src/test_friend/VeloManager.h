//
// Created by bigaoxian on 8/16/24.
//

#ifndef CPP11_DEMO_VELOMANAGER_H
#define CPP11_DEMO_VELOMANAGER_H

// VeloManager.h
#include <iostream>

class Target; // 前向声明

class VeloManager {
 public:
  VeloManager() = default;
  VeloManager(Target* target) {
    target_ = target;
  }

  void InitVelo(Target* target);
  void InitVelo();

 private:
  Target* target_;
};


#endif //CPP11_DEMO_VELOMANAGER_H
