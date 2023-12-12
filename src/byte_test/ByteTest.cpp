//
// Created by sheldon on 2022/2/8.
//
#include <algorithm>
#include <iostream>
#include <cmath>
#include <stdlib.h>
#include <string>
#include <vector>


int main() {

    char dev_node[32] = "/dev/video1";
    printf("camera_no:%d\n", dev_node[10]);
    int camera_no = dev_node[10] - 0x30;
    printf("0x30:%d\n", 0x30);
    printf("camera_no:%d\n", camera_no);


    std::vector<std::string> camera_nodes_;
    camera_nodes_.push_back("/dev/video5");
    camera_nodes_.push_back("/dev/video4");

    std::sort(camera_nodes_.begin(), camera_nodes_.end(), [](std::string& c1, std::string& c2){
        int c1_index = atoi(c1.substr(c1.size()-1).c_str());
        int c2_index = atoi(c2.substr(c2.size()-1).c_str());
        return c1_index < c2_index;
    });

    uint8_t mask = 0;
    for (int i = 0; i < camera_nodes_.size(); ++i) {
        std::string camera_name = camera_nodes_[i];
        int index = atoi(camera_name.substr(camera_name.size()-1).c_str());
        printf("index:%d\n", index);
        mask += (uint8_t)pow(2, index);
    }
    printf("mask :%d\n", mask);

}