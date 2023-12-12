#ifndef VISION_PERCEPTION_BUILD_CONFIG_H
#define VISION_PERCEPTION_BUILD_CONFIG_H

#define IS_COMPRESSED_IMAGE_TYPE true
#define PROJECT_NAME_STR "vision_perception_front"
#define COORD_PARAM_RELATIVE_PATH_STR "/configs/CoordConfig.yml"
#define CAMERA_PARAM_RELATIVE_PATH_STR "/configs/camera-config"
#define CAMERA_YAML_RELATIVE_PATH_STR "/configs/CameraConfig.yml"
#define DISTANCE_YAML_RELATIVE_PATH_STR "/configs/DistanceEstimator.yml"
#define TRUNK_LANE_DBC_FILE_PATH_STR "/configs/trunk-CAN3.dbc"
#define TRUNK_IMU_DBC_FILE_PATH_STR "/configs/trunk-305.dbc"

#define TRUNK_BAD_CASE_RELATIVE_PATH "/../../../logs/vision_perception_front/bad_case/"
#define TRUNK_BAD_CASE_BUFFER_SIZE 10


#define LOG_LEVEL trace

#endif //VISION_PERCEPTION_BUILD_CONFIG_H
