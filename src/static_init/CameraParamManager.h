#ifndef TRUNK_VISION_CAMERA_PARAM_MANAGER_H
#define TRUNK_VISION_CAMERA_PARAM_MANAGER_H

#include <string>
#include <vector>
#include <unordered_map>



namespace trunk {
    namespace perception {

        using namespace trunk::perception;
        using CameraParamsMap = std::unordered_map<std::string, int>;

        class CameraParamManager {
        public:
            static int InitCameraParams();
            static int GetCameraParams(const std::string& camera_name);
            static std::vector<std::string>& GetCameraNames();
        private:
            static CameraParamsMap camera_params_map_;
        };

    }  // namespace perception
}  // namespace trunk
#endif //TRUNK_VISION_CAMERA_PARAM_MANAGER_H
