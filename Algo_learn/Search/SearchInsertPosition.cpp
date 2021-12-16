//
// Created by Huangff on 2021/7/9.
//
#include "vector"
using namespace  std;

class Solution {
public:
    int searchInsert(vector<int>& nums, int target) {
        //简单的二分查找
        //小知识点： java数组的最大长度为int的最大值
        int left = 0;
        int right = nums.size() - 1;
        while (left < right) {
            int mid = (left + right) / 2;
            if (target == nums.at(mid)) {
                return mid;
            } else if (target < nums.at(mid)) {
                right = mid - 1;
            } else {
                left = mid + 1;
            }
        }
        //此时left = right
        return target <= nums.at(left) ? left : left + 1;
    }
};