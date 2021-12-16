//
// Created by Huangff on 2021/7/9.
//
bool isBadVersion(int number){
    return false;
}
/*
 *
You are a product manager and currently leading a team to develop a new product.
Unfortunately, the latest version of your product fails the quality check.
Since each version is developed based on the previous version, all the versions after a bad version are also bad.
Suppose you have n versions [1, 2, ..., n] and you want to find out the first bad one, which causes all the following ones to be bad.
You are given an API bool isBadVersion(version) which returns whether version is bad.
Implement a function to find the first bad version. You should minimize the number of calls to the API.
 * */
class Solution {
public:
    int firstBadVersion(int n) {
        int min=0;
        int middle = min+(n-min)/2;
        //二分法搜索
        while(min<=n){
            if(isBadVersion(middle)){
                //true
                n=middle-1;
            }else {
                min=middle+1;
            }
            middle=min+(n-min)/2;
        }
        return middle;
    }
};