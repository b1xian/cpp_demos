//
// Created by sheldon on 2022/3/18.
//
#include <iostream>
#include <vector>
# include  <algorithm>

struct Obj {
    int x;
    //double y;
    float z;

    //void clear() {
    //    x = 0;
    //    y = 0.;
    //}
};


int main() {
    Obj obj_arr[10];
    //printf("objr size:%d\n", sizeof(Obj));
    //printf("obj0 size:%d\n", sizeof(obj_arr[0]));
    //printf("obj_arr size:%d\n", sizeof(obj_arr) / sizeof(Obj));



    std::vector<std::vector<int>> lanes;
    lanes.push_back({1,2});
    lanes.push_back({1,2});
    lanes.push_back({1,2});
    lanes.push_back({1,2,3});
    lanes.push_back({1,2,3,4});

    std::vector<int> types;
    types.push_back(1);
    types.push_back(3);
    types.push_back(0);
    types.push_back(1);
    types.push_back(2);

    auto it_lane = lanes.begin();
    auto it_type = types.begin();
    for (;it_lane != lanes.end(), it_type != types.end();) {
        int lane_size = it_lane->size();
        printf("~~~~~~~~~~~lane \n");
        for (int j = 0; j < lane_size; j++) {
            printf("%d ", it_lane->at(j));
        }
        printf("\n");
        if (lane_size <= 2) {
            printf("erase lane\n");
            it_lane = lanes.erase(it_lane);
            it_type = types.erase(it_type);
        } else {
            printf("remain lane\n");
            it_lane++, it_type++;
        }
    }
    for (int i = 0; i < lanes.size(); ++i) {
        std::cout << "lane size:" << lanes[i].size() << std::endl;
    }



    std::vector<std::pair<int, int>> starting_point;
    starting_point.emplace_back(1, 2);
    starting_point.emplace_back(1, 2);
    starting_point.emplace_back(1, 3);
    starting_point.emplace_back(1, 4);
    starting_point.emplace_back(1, 4);
    std::vector<std::pair<int, int>> starting_point_unique;
    for (int k = 0; k < starting_point.size(); k++) {
        auto it = std::find(starting_point_unique.begin(), starting_point_unique.end(), starting_point[k]);
        if (it == starting_point_unique.end()) {
            starting_point_unique.push_back(starting_point[k]);
        }
    }
    for(auto it = starting_point_unique.begin(); it != starting_point_unique.end(); it++) {
        printf("%d %d \n", it->first, it->second);
    }


    std::vector<int> a = {1, 2, 4, 3, 6, 5};
    std::sort(a.begin(), a.end(), [](int a1, int a2){
        return a1 > a2;
    });
    for (int i = 0; i < a.size(); ++i) {
        printf("%d ", a[i]);
    }
    printf("\n");
    printf("f:%d b:%d\n", a.front(), a.back());
    return 0;
}
