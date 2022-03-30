//
// Created by Huangff on 2022/3/8.
//

#pragma once

#include "PlaceAlgo/MyStruct.h"

static QList<Bar> amout = {
        {1, "a"},
        {1, "b"},
        {1, "a"},
        {1, "c"},
        {2, "c"},
        {2, "b"},
        {2, "a"},
};


static bool compareBarData(const Bar &bar, const Bar &bar2);

void qSortUse();


void qSortUse() {
    /// qt algorithm Sort的使用
    // int 的比较
    QList<int> testSort = {2, 1, 5, 3};
    qDebug() << testSort;
    std::sort(testSort.begin(), testSort.end());
    qDebug() << testSort;

    // QString的比较
    QList<QString> list = {"aa", "ba", "bb", "ba", "ab"};
    std::sort(list.begin(), list.end());
    qDebug() << list;

    // 结构体元素的比较
    QList<Bar> list1 = amout;
    for (int i = 0; i < list1.size(); ++i) {
        qDebug() << list1[i].barLevel << " " << list1[i].diameter;
    }

    qDebug() << "-----";
    qSort(list1.begin(), list1.end(), compareBarData);
    for (int i = 0; i < list1.size(); ++i) {
        qDebug() << list1[i].barLevel << " " << list1[i].diameter;
    }
}

static bool compareBarData(const Bar &bar, const Bar &bar2) {
    // 根据多个维度来排序 在指标Ⅰ相同的情况下，根据指标Ⅱ进行排序

    if (bar.barLevel < bar2.barLevel) {
        // 从小到大排序
        return true;
    } else if (bar.barLevel > bar2.barLevel) {
        return false;
    } else {
        QString strDiameter = bar.diameter;
        QString diameter = bar2.diameter;
        if (strDiameter > diameter) {
            return true;
        } else {
            return false;
        }
    }
}
