#ifndef TRUNK_VISION_PERCEPTION_RTCONFIG_H
#define TRUNK_VISION_PERCEPTION_RTCONFIG_H

#include <memory>
#include <string>
#include <unordered_map>

#include "yaml-cpp/yaml.h"  // for yaml

namespace trunk {
namespace perception {

class VisionConfig {

public:
    static std::shared_ptr<VisionConfig> GetInstance(const std::string& config_path);
    YAML::Node operator[](const std::string& node_name);
private:
    VisionConfig() {}

    void LoadConfigFromFile(const std::string& config_path);

    std::unordered_map<std::string, YAML::Node> yml_node_map_;
    YAML::Node root_node_;
    static std::shared_ptr<VisionConfig> instance_;
};

} // namespace perception
} // namespace trunk


#endif // TRUNK_VISION_PERCEPTION_RTCONFIG_H
