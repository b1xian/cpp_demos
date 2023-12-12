#include "VisionConfig.h"

#include <iostream>

namespace trunk {
namespace perception {

std::shared_ptr<VisionConfig> VisionConfig::instance_ = nullptr;

// TODO 如果可以像ros一样在获取package路径，则config_path可以使用相对路径
std::shared_ptr<VisionConfig> VisionConfig::GetInstance(const std::string &config_path) {
    if (!instance_) {
        instance_.reset(new VisionConfig());
        instance_->LoadConfigFromFile(config_path);
        return instance_;
    }
}

YAML::Node VisionConfig::operator[](const std::string& node_name) {
    return yml_node_map_[node_name];
}

void VisionConfig::LoadConfigFromFile(const std::string& config_path) {
    // TODO 路径存在判断 工具函数
    if (config_path.empty()) {
        std::cout << "vision config yml file path invalid:" << config_path << std::endl;
        throw std::runtime_error("file not exists!");
    }
    root_node_ = YAML::LoadFile(config_path);
    std::string cfg_root_path = config_path.substr(0, config_path.find_last_of('/') + 1);
    for (auto node : root_node_) {
        std::string sub_node_name = node.first.as<std::string>();
        std::string sub_yml_path = node.second.as<std::string>();

        if (sub_yml_path.size() &&
            sub_yml_path.at(0) != '/') {  // 相对路径 则进行路径追加, 否则直接使用
            sub_yml_path = cfg_root_path + sub_yml_path;
        }
        
        if (sub_yml_path.empty()) {
            std::cout << "vision config yml file path invalid:" << config_path << std::endl;
            throw std::runtime_error("file not exists!");
        }
        YAML::Node sub_node = YAML::LoadFile(sub_yml_path);
        yml_node_map_[sub_node_name] = sub_node;
        std::cout <<  "load yml node:" << sub_node_name << " from " << sub_yml_path << std::endl;
    }

}

} // namespace perception
} // namespace trunk