#ifndef TRUNK_VISION_PERCEPTION_ERROR_CODE_H
#define TRUNK_VISION_PERCEPTION_ERROR_CODE_H

#include <unordered_map>

#define MARGIN 100

namespace trunk {
namespace perception {

enum ErrorCode {
    /**
     * note：添加ErrorCode后，务必在下方的err_code_desc_map_中添加其对应的文字说明信息。
     *
     */
    NORMAL = 0u,

    // init state, start from 10001
    INIT_CODE = 10000u, // don't use

    INIT_CAMERA_CODE = INIT_CODE + 1, // don't use
    INIT_CAMERA_DRIVER_FAILED = INIT_CAMERA_CODE * MARGIN + 1, // 10001001
    // =========================end========================= //

    // percept input state, start from 20001
    INPUT_CODE = 20000u, // don't use

    // camera data recv
    CAMERA_RECV_CODE = INPUT_CODE + 1, // don't use
    CAMERA_DELAY_TOO_LONG = CAMERA_RECV_CODE * MARGIN + 1, // 20001001
    CAMERA_FPS_ABNORMAL = CAMERA_RECV_CODE * MARGIN + 2, // 20001002

    // pre data match
    PRE_DATA_MATCH_CODE = INPUT_CODE + 2, // don't use
    RADAR_MATCH_FAILED = PRE_DATA_MATCH_CODE * MARGIN + 1, // 20002001
    ODOMETRY_MATCH_FAILED = PRE_DATA_MATCH_CODE * MARGIN + 2, // 20002002
    // =========================end========================= //


    // percept state, start from 30001
    PERCEPT_CODE = 30000u,
    OBJECT_DETECT_CODE = PERCEPT_CODE + 1, // don't use
    // ...
    LANE_DETECT_CODE = PERCEPT_CODE + 2, // don't use
    // ...
    // =========================end========================= //


    // percept output state, start from 40001
    OUTPUT_CODE = 40000u, // don't use
    // =========================end========================= //
};

const static std::unordered_map<ErrorCode, std::string> err_code_desc_map_ {
        {NORMAL, "正常"},

        {INIT_CAMERA_DRIVER_FAILED, "摄像头初始化错误"},

        {CAMERA_DELAY_TOO_LONG, "图像输入延迟过高"},
        {CAMERA_FPS_ABNORMAL, "图像fps异常"},

        {RADAR_MATCH_FAILED, "毫米波匹配失败"},
        {ODOMETRY_MATCH_FAILED, "里程计匹配失败"},
};



}  // namespace perception
}  // namespace trunk
#endif //TRUNK_VISION_PERCEPTION_ERROR_CODE_H
