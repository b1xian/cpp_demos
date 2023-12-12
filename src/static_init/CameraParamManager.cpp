#include "CameraParamManager.h"

#include <vector>

#include <iostream>


namespace trunk {
namespace perception {

CameraParamsMap CameraParamManager::camera_params_map_;

int CameraParamManager::InitCameraParams() {
    if (camera_params_map_.size() > 0) {
        std::cout << "CameraParamManager hase been inited!";
        return 0;
    }
    std::cout << "all camera params loaded." <<std::endl;
    return 0;
}

int CameraParamManager::GetCameraParams(const std::string &camera_name) {
    return camera_params_map_[camera_name];
}

std::vector<std::string>& CameraParamManager::GetCameraNames() {
    std::vector<std::string> camera_names;
    for (auto pair : camera_params_map_) {
        camera_names.push_back(pair.first);
    }
    return camera_names;
}


// static init
#define INIT_RESULT_VAR(file, line) Init_result_##file_##line
#define INIT_CAMERA_PARAM_MANAGER() \
         static int INIT_RESULT_VAR(__FILE__, __LINE__) = CameraParamManager::InitCameraParams();
        INIT_CAMERA_PARAM_MANAGER()
#undef INIT_CAMERA_PARAM_MANAGER
#undef INIT_RESULT_VAR

}  // namespace perception
}  // namespace trunk