#include <algorithm>
#include <iostream>
#include <vector>
#include <gtest/gtest.h>

typedef struct Bbox {
    int x1, y1, x2, y2;
    float score;
};

float Iou (Bbox& box1, Bbox& box2) {
    float area1 = (box1.x2 - box1.x1) * (box1.y2 - box1.y1);
    float area2 = (box2.x2 - box2.x1) * (box2.y2 - box2.y1);

    int max_x1 = std::max(box1.x1, box2.x1);
    int min_x2 = std::min(box1.x2, box2.x2);
    int max_y1 = std::max(box1.y1, box2.y1);
    int min_y2 = std::min(box1.y2, box2.y2);

    if (max_x1 > min_x2 && max_y1 > min_y2) {
        return  0.f;
    }

    float inter_area = std::max(0, min_x2 - max_x1) * std::max(0, min_y2 - max_y1);
    float union_area = area1 + area2 - inter_area;

    return inter_area / union_area;

}

std::vector<Bbox> Nms(std::vector<Bbox>& boxes, float thresh) {
    // 按照score从大到小排序
    std::sort(boxes.begin(), boxes.end(), [](Bbox& box1, Bbox& box2) -> bool{
        return box1.score > box2.score;
    });
    std::vector<Bbox> res;
    while (!boxes.empty()) {
        res.push_back(boxes[0]);
        size_t index = 1;
        while (index < boxes.size()) {
            if (Iou(boxes[0], boxes[index]) > thresh) {
                boxes.erase(boxes.begin() + index); // 重叠框，删除
            } else {
                index += 1;
            }
        }
        boxes.erase(boxes.begin()); // 添加过的，删除
    }
    return res;
}

int main(int argc,char **argv) {
    testing::InitGoogleTest(&argc, argv);
    std::cout << "Hello, iou!" << std::endl;



    return RUN_ALL_TESTS();
}