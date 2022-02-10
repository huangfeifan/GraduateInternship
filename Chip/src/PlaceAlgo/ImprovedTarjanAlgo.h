//
// Created by Huangff on 2022/1/26.
//

#pragma once

// 非递归版本 Tarjan_algorithm  todo modify
class ImprovedTarjanAlgo {

public:
    // 非递归版本
    ImprovedTarjanAlgo() {
        initData();

        //Run strong connect starting from each vertex
        for (int i = 0; i < numVertices; ++i) {
            if (index[i] < 0) {
                strongConnect(i);
            }
        }
        // Compact sccAdjList
        compactSccAdjList();
    }

private:

    int numVertices = 0; //adjList.length;  有向图顶点个数
    QList<int> index; //
    QList<int> lowValue; //能够抵达该顶点的最早时间戳
    QList<bool> active; // 记录顶点是否被访问
    QList<int> child;
    QList<int> scc;
    QList<QList<int>> sccLinks;

    int count = 0;
    QList<int> components;
    QList<QList<int>> sccAdjList;

    void initData() {
        //Initialize tables
        for (int i = 0; i < numVertices; ++i) {
            index.push_back(-1);
            lowValue.push_back(0);
            active.push_back(false);
            child.push_back(0);
            scc.push_back(-1);

            QList<int> temp;
            sccLinks.push_back(temp);
        }
    }

    void strongConnect(int v) {

    }

    void compactSccAdjList() {
        QList<int> newE;
        for (int i = 0; i < sccAdjList.size(); i++) {
            QList<int> e = sccAdjList[i];
            if (e.size() == 0)
                continue;
            //e.sort(function(a, b)
            //{ return a - b; })
            //newE = [e[0]];
            for (int j = 1; j < e.size(); j++) {
                //if (e[j] != = e[j - 1]) {
                //      newE.push(e[j])
                //}
            }
            sccAdjList[i] = newE;
        }
    }
};