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
    queue.push_back(0);
    queue.push_back(2);
    queue.push_back(4);
    queue.push_back(6);
    queue.push_back(8);
    queue.push_back(10);
    queue.push_back(12);
    printf("f:%d b:%d\n", queue.front(), queue.back());
    printf("[");
    for (int i = 0; i < queue.size(); ++i) {
        printf("i:%d %d\n ", i, queue[i]);
    }
    printf("]\n");
//    int num = 2;
//    int index = queue.size() - num;
//    printf("num:%d search %d, %d\n", num, index, queue[index]);
//    num = 5;
//    index = queue.size() - num;
//    printf("num:%d search %d, %d\n", num, index, queue[index]);
//    num = 6;
//    index = queue.size() - num;
//    printf("num:%d search %d, %d\n", num, index, queue[index]);

//  std::deque<int> frame_queue_;
//  frame_queue_.insert(frame_queue_.end(), queue.begin(), queue.end());
//
//  // 从旧到新排序
//  std::sort(frame_queue_.begin(), frame_queue_.end(),
//            [] (int f1, int f2) {
//              return f1 > f2;
//            });
//  printf("latest:%d\n", frame_queue_.front());
//
//  int frame_expired_limit_ = 6;
//  int lastest_framestamp_ = frame_queue_.front();
//  auto expired_iter = std::find_if(frame_queue_.begin(), frame_queue_.end(), [&] (int v) {
//    return lastest_framestamp_ - v > frame_expired_limit_;
//  });
//  printf("expired:%d\n", *expired_iter);
//  for (auto iter = expired_iter; iter != frame_queue_.end(); iter++) {
//      int delta_time_latest = lastest_framestamp_ - *iter;
//      // 数据延时太久了，丢弃
//      printf("remove expired:%d, %d\n", *iter, delta_time_latest);
//  }
//  frame_queue_.erase(expired_iter, frame_queue_.end());
////  int max_size = 5;
////  if (frame_queue_.size() > 5) {
////    auto iter = frame_queue_.begin() + 5;
////    printf("%d\n", *iter);
////    frame_queue_.erase(iter, frame_queue_.end());
////  }
//
//
//  for (size_t i = 0; i < frame_queue_.size(); i ++) {
//    printf("%d ", frame_queue_[i]);
//  }
//  printf("\n");
//
//  std::sort(frame_queue_.begin(), frame_queue_.end(),
//            [] (int f1, int f2) {
//              return f1 < f2;
//            });
  printf("f:%d b:%d\n", queue.front(), queue.back());
  int target = 11;
  int insert_idx = searchInsert(queue, target);
  printf("target %d, left:%d\n", target, insert_idx);

  for (int i = 0; i < insert_idx; ++i) {
      queue.pop_front();
      //if (!queue.empty()) {
      //}
  }
//    int total_size = queue.size();
//    printf("size %d\n", total_size);
//    printf("[");
//    for (int i = 0; i < queue.size(); ++i) {
//        printf("%d ", queue[i]);
//    }
//    printf("]\n");


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


