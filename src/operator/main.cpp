#include <iostream>
#include "VisionConfig.h"

int main() {
    std::string root_yml_path = "/home/sheldon/work_space/code/dev_code/深入应用CPP11/src/operator/config/VisionPerceptionOnline.yml";
    std::shared_ptr<trunk::perception::VisionConfig> visionConfig = trunk::perception::VisionConfig::GetInstance(root_yml_path);


    std::string s1{"das-231312.yml"};
    std::size_t current = s1.find('-');

    printf("~~~~~~~~~~~current:%d\n", current);
    printf("~~~~~~~~~~~%s\n", s1.substr(0, current).c_str());
    printf("~~~~~~~~~~~\n");
}