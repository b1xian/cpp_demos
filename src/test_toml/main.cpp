#include <toml.hpp>

#include <iostream>

int main()
{
    auto data = toml::parse("/home/sheldon/work_space/code/trunk/metrics_config/vision/7875_20230314/f60-7875-20230314.toml");

    double pitch = toml::find<float>(data, "camera_rotate", "pitch");
    std::cout << pitch << std::endl;
    if (data.at("camera_rotate").contains("qx")) {
        double qx = toml::find<float>(data, "camera_rotate", "qx");
        double qy = toml::find<float>(data, "camera_rotate", "qy");
        double qz = toml::find<float>(data, "camera_rotate", "qz");
        double qw = toml::find<float>(data, "camera_rotate", "qw");
        std::cout << qx << std::endl;
        std::cout << qy << std::endl;
        std::cout << qz << std::endl;
        std::cout << qw << std::endl;
    }

    // ```toml
    // title = "an example toml file"
    // nums  = [3, 1, 4, 1, 5]
    // ```
    //auto data = toml::parse("/home/sheldon/work_space/code/dev_code/深入应用CPP11/src/test_toml/config/sample.toml");
    //
    //// find a value with the specified type from a table
    //std::string title = toml::find<std::string>(data, "titl2e");
    //std::cout << title << std::endl;
    //
    //// convert the whole array into any container automatically
    //std::vector<int> nums = toml::find<std::vector<int>>(data, "nums");
    //for (int i = 0; i < nums.size(); ++i) {
    //    std::cout << nums[i] << std::endl;
    //}
    //
    //// access with STL-like manner
    //if(!data.contains("foo"))
    //{
    //    data["foo"] = "bar";
    //}
    //std::cout << data["foo"] << std::endl;
    //
    //// pass a fallback
    ////std::string name = toml::find_or<std::string>(data, "name", "not found");
    ////std::cout << name << std::endl;
    //
    ////// width-dependent formatting
    ////std::cout << std::setw(80) << data << std::endl;
    //;
    //const auto& fruit = toml::find(data, "fruit");
    //const auto  name  = toml::find<std::string>(fruit, "name");
    //std::cout << name << std::endl;
    //
    //const auto& physical = toml::find(fruit, "physical");
    //const auto  color    = toml::find<std::string>(physical, "color");
    //std::cout << color << std::endl;
    //const auto  shape    = toml::find<std::string>(physical, "shape");
    //std::cout << shape << std::endl;
    return 0;
}
