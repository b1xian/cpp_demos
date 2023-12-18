#include <iostream>
#include <vector>
#include <algorithm>
#include <gtest/gtest.h>

using namespace std;

// 快速选择法
int partition(vector<int>& nums, int left, int right) {
    int pivot = nums[left]; // 左1为基准值
    int l = left + 1, r = right; // 双指针
    while (l <= r) {
        if (nums[l] < pivot) {
            l++; // 左侧小于基准值，跳过，左指针++
        } else if (nums[r] > pivot) {
            r--; // 右侧大于基准值，跳过，右指针--
        } else {
            // 交换左右，左右指针继续移动
            swap(nums[l++], nums[r--]);
        }
    }
    // 将基准元素归位
    swap(nums[left], nums[r]);
    // 返回基准元素所在的位置
    return r;
}

int partition1(std::vector<int>& nums, int left, int right) {
    int pivot = nums[left];
    int l = left + 1, r = right;
    while(l <= r) {
        if (nums[l] < pivot) {
            l++;
        } else if (nums[r] > pivot) {
            r--;
        } else {
            std::swap(nums[l], nums[r]);
            l++, r--;
        }
    }
    std::swap(nums[left], nums[r]);
    return r;
}
void QuickSort1(std::vector<int>& nums, int left, int right) {
    if (left >= right) {
        return;
    }
    int pivot = partition1(nums, left, right);
    QuickSort1(nums, left, pivot - 1);
    QuickSort1(nums, pivot + 1, right);
}


int quickSelect(std::vector<int>& a, int l, int r, int index) {
    int q = partition(a, l, r);
    if (q == index) {
        return a[q];
    } else if (q < index) {
        return quickSelect(a, q + 1, r, index);
    } else {
        return quickSelect(a, l, q - 1, index);
    }
}

void quickSort(std::vector<int>& nums, int left, int right) {
    if (left >= right) {
        return ;
    }
    // 分两堆
    int pivot = partition(nums, left, right);
    // 继续分左侧
    quickSort(nums, left, pivot - 1);
    // 继续分右侧
    quickSort(nums, pivot + 1, right);
}

int findKthLargest(std::vector<int>& nums, int k) {
    // 寻找第k大的元素，那么只要我们part后，
    // part的划分位置的索引等于length - k即可，此时右侧的part有k-1个元素，
    // 那么part划分的位置就是第k大的元素(因为右侧part的元素都大于它)
    return quickSelect(nums, 0, nums.size() - 1, nums.size() - k);
}

TEST(testCase, test_auto_1){
    std::vector<int> nums = {3, 2, 1, 2, 5, 2, 4};
    int k = 4;

    // 使用快速选择法
    int result_quickSelect = findKthLargest(nums, k);
    std::cout << "Quick Select: " << result_quickSelect << std::endl;

    EXPECT_EQ(result_quickSelect, 2);

    // 排序
    QuickSort1(nums, 0, nums.size() - 1);
    for (int num : nums) {
        std::cout << num << " ";
    }
    std::cout << std::endl;
}

