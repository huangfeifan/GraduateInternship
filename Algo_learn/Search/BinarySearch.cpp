//
// Created by Huangff on 2021/7/9.
// https://leetcode-cn.com/problems/binary-search/solution/binary-search-er-fen-fa-sou-suo-by-momo-04gl8/
//
#include <vector>
using namespace  std;
class Solution {
public:
    int search(vector<int>& nums, int target) {
        int length = nums.size();
        int startIndex = 0, endIndex = length - 1;
        int middle = (startIndex + endIndex) / 2;
        while (startIndex <=endIndex) {
            //不等于 那么及时更新startIndex endIndex
            if (nums.at(middle) < target) {
                //target 可能在右半部分
                startIndex = middle + 1;
                middle = (startIndex + endIndex) / 2;
            } else if (nums.at(middle) > target) {
                //target 可能在左半部分
                endIndex = middle - 1;
                middle = (startIndex + endIndex) / 2;
            } else if (nums.at(middle) == target) {
                //判断target在不在middle位置
                return middle;
            }
        }
        return -1;
    }
};

