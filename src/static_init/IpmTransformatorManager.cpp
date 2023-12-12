//
// Created by sheldon on 2022/2/18.
//

#include "IpmTransformatorManager.h"

#include <iostream>

IpmTransformatorMap IpmTransformatorManager::imp_transformator_map;

int IpmTransformatorManager::InitIpmTransformator() {
    std::cout << "init ipm" <<std::endl;
}

int IpmTransformatorManager::GetIpmTransformator(const std::string& camera_name) {
    return imp_transformator_map[camera_name];
}


// static init
#define INIT_RESULT_VAR(file, line) Init_result_##file_##line
#define INIT_IPM_MANAGER() \
         static int INIT_RESULT_VAR(__FILE__, __LINE__) = IpmTransformatorManager::InitIpmTransformator();
INIT_IPM_MANAGER()
#undef INIT_IPM_MANAGER
#undef INIT_RESULT_VAR