//
// Created by bigaoxian on 2021/10/25.
//

#include <deque>
#include <numeric>
#include <gtest/gtest.h>


int searchInsert(std::deque<int>& nums, int target) {
    int right = nums.size() - 1;
    int left = 0;
    while(left <= right) {
        int mid = left + (right-left) / 2;
        //原本是int mid = (left + right) / 2，上一写法更好;
        if(nums[mid] == target) {
            return mid;
        } else if(nums[mid] < target) {
            left = mid +1 ;
        } else {
            right = mid -1;
        }
    }
    return left;
}


int searchReverseInsert(std::deque<int>& nums, int target) {
    int left = 0;
    int right = nums.size() - 1;
    while(left <= right) {
        //int mid = left + (right - left) / 2;
        //原本是int mid = (left + right) / 2，上一写法更好;
        int mid = (left + right) / 2;
        if(nums[mid] == target) {
            //printf("0: left:%f mid:%f right:%f",
            //       nums[left], nums[mid], nums[right]);
            return mid;
        } else if(nums[mid] < target) {
            //printf("1: left:%f mid:%f right:%f",
            //       nums[left], nums[mid], nums[right]);
            right = mid -1;
        } else {
            //printf("2: left:%f mid:%f right:%f",
            //       nums[left], nums[mid], nums[right]);
            left = mid +1 ;
        }
    }
    return left;
}


int main () {
    std::deque<int> queue;
    //queue.push_back(0);
    //queue.push_back(2);
    //queue.push_back(4);
    queue.push_back(6);
    queue.push_back(10);
    printf("f:%d b:%d\n", queue.front(), queue.back());
    printf("[");
    for (int i = 0; i < queue.size(); ++i) {
        printf("%d ", queue[i]);
    }
    printf("]\n");
    int target = 11;
    int insert_idx = searchInsert(queue, target);
    printf("target %d, left:%d\n", target, insert_idx);

    for (int i = 0; i < insert_idx; ++i) {
        queue.pop_front();
        //if (!queue.empty()) {
        //}
    }
    int total_size = queue.size();
    printf("size %d\n", total_size);
    printf("[");
    for (int i = 0; i < queue.size(); ++i) {
        printf("%d ", queue[i]);
    }
    printf("]\n");


    //target = 0;
    //insert_idx = searchReverseInsert(queue, target);
    //printf("target %f, insert_idx:%d\n", target, insert_idx);
    //target = 8;
    //insert_idx = searchReverseInsert(queue, target);
    //printf("target %f, insert_idx:%d\n", target, insert_idx);

    //std::vector<int> vec1 = {0, 1, 2, 3, 10, 1, 2, 3, 100, 1, 2, 3, 1000, 1, 2, 3};
    //std::vector<int> vec2 = {7, 8, 9, 10};
    //
    //// 创建两个迭代器，分别指向vec1和vec2的开头
    //auto it1 = vec1.begin();
    //auto it2 = vec2.begin();
    //
    //for (;it1 != vec1.end(), it2 != vec2.end();) {
    //    printf("v1:%d v2:%d\n", *it1, *it2);
    //    std::advance(it1, 4);  // it1前进2步
    //    std::advance(it2, 1);  // it2前进3步
    //}
    //
    //
    //
    //std::vector<int> result;
    //set_intersection(vec1.begin(),vec1.end(),
    //                 vec2.begin(),vec2.end(),
    //                 inserter(result, result.begin()));
    //for (int r : result) {
    //    std::cout << r << std::endl;
    //}

    return 0;
}


