#ifndef TRUNK_VISION_PERCEPTION_RECORD_TYPE_H
#define TRUNK_VISION_PERCEPTION_RECORD_TYPE_H

#include <string>
#include <unordered_map>

namespace trunk {
namespace perception {

/**
 * 	每帧的算法pipline重要状态：
 * 	初始化状态
 *
	输入状态：
        毫米波雷达/odometry的匹配情况
        图像数据延迟时间
	感知状态：
		各模块的处理时长
		detect过程中重要状态：
		tracking过程中重要状态：
	输出状态：
        车道线、障碍物、cross point，车身姿态，vp点，报文发送情况
 */
// 记录感知过程中的重要状态
enum PerceptRecordType {
    /**
     * note：添加PerceptRecordType后，务必在下方的record_type_desc_map中添加其对应的文字说明信息。
     *
     */
    CAMERA_DRIVER,
    RADAR_MATCH,
    ODOMETRY_MATCH,
};

const static std::unordered_map<PerceptRecordType, std::string> record_type_desc_map {
        {CAMERA_DRIVER, "图像获取"},
        {RADAR_MATCH, "毫米波雷达匹配"},
        {ODOMETRY_MATCH, "里程计匹配"},
};


}  // namespace perception
}  // namespace trunk
#endif //TRUNK_VISION_PERCEPTION_RECORD_TYPE_H
