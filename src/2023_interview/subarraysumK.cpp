//
// Created by cuteyuyin on 23-12-18.
//
#include <algorithm>
#include <iostream>
#include <unordered_map>
#include <vector>
#include <gtest/gtest.h>

using namespace std;

/**
 * 使用前缀和的方法可以解决这个问题，因为我们需要找到和为k的连续子数组的个数。通过计算前缀和，我们可以将问题转化为求解两个前缀和之差等于k的情况。

假设数组的前缀和数组为prefixSum，其中prefixSum[i]表示从数组起始位置到第i个位置的元素之和。那么对于任意的两个下标i和j（i < j），
如果prefixSum[j] - prefixSum[i] = k，即从第i个位置到第j个位置的元素之和等于k，那么说明从第i+1个位置到第j个位置的连续子数组的和为k。

通过遍历数组，计算每个位置的前缀和，并使用一个哈希表来存储每个前缀和出现的次数。在遍历的过程中，我们检查是否存在prefixSum[j] - k的前缀和，
如果存在，说明从某个位置到当前位置的连续子数组的和为k，我们将对应的次数累加到结果中。

这样，通过遍历一次数组，我们可以统计出和为k的连续子数组的个数，并且时间复杂度为O(n)，其中n为数组的长度。
 */
int subarraySum(vector<int>& nums, int k) {
    unordered_map<int, int> map;
    int count = 0;
    int pre = 0;
    map.insert({0, 1});
    for (int i = 0; i < nums.size(); ++i) {
        // 当前之和
        pre += nums[i];
        // 是否存在
        int look_for = pre - k;
        if (map.find(look_for) != map.end()){
            count += map.at(look_for);
        }
        // 记录前缀和出现的次数
        map[pre]++;
    }
    return count;
}



TEST(SubarraysumK, subarraysumK) {

}