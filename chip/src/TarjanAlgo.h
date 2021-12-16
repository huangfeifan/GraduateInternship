﻿//
// Created by Huangff on 2021/12/15.
//

#pragma once

#include <QVector>
#include <QStack>
#include <QDebug>


// 递归版本 Trajan_algorithm
class TarjanAlgo {
    // Time Complexity O(N+E) Space Complexity O(N+E) 邻接表存储数据
    // link https://www.bilibili.com/video/BV19J411J7AZ?p=5 邻接矩阵
    // link https://segmentfault.com/a/1190000039149539  邻接表
public :

    // 递归版本
    TarjanAlgo(int number) {

        // number 模块数量
        m_numVertices = number;

        // 初始化邻接表
        initConnection();

        // 数据匹配不上
        if (number != m_connection.size()) {
            qDebug() << "data error \n vertices_num != m_connect_size";
            return;
        }

        // init
        for (int i = 0; i < m_numVertices; ++i) {
            m_dfn.push_back(-1);
            m_low.push_back(-1);
            m_inStack.push_back(false);
        }

        //qDebug() << m_connection.size();
        getSCC();

        qDebug() << m_sccs.size() << "  scc_size";
        for (int i = 0; i < m_sccs.size(); ++i) {
            qDebug() << m_sccs[i];
        }
    }

    int m_numVertices = 0;// 图顶点个数
    int m_count = 0;// 搜索的次序编号(时间戳)
    QVector<int> m_dfn; // 结点i搜索的次序编号(时间戳)  表示真实访问次序
    QVector<int> m_low; // 结点i或i的子结点能够追溯到的最早时间戳 通过邻接表以及子结点的dfn来实现更新
    QVector<bool> m_inStack; // 结点是否在栈内
    QStack<int> m_stack; // 用于回退结点

    QList<QList<int>> m_sccs;// strongly connected components 保存强连通分量
    QList<QList<int>> m_connection;//邻接表



    void initConnection() {
        // test data link https://www.bilibili.com/video/BV19J411J7AZ?p=5
/*        m_connection = {
                {1, 5},//0
                {2},//1
                {3, 4},//2
                {1},//3
                {},//4
                {6},//5
                {0}//6
        };*/

/*        m_connection = {
                {4},//0
                {0, 2},//1
                {1, 3},//2
                {2},//3
                {1},//4
                {4, 6},//5
                {5, 2},//6
                {7, 6, 3}//7
        };*/

        // test data link https://segmentfault.com/a/1190000039149539
/*        m_connection = {
                {1, 2},//0
                {3},//1
                {3, 4},//2
                {0, 5},//3
                {5},//4
                {},//5

        };*/

        // 有向无环图 DAG directed acyclic graph
        m_connection = {
                {1, 5},//0
                {},//1
                {3, 4},//2
                {1},//3
                {},//4
                {6},//5
                {}//6
        };
    }

    bool isConnected(int start, int end) {

        for (int i = 0; i < m_connection[start].size(); ++i) {
            if (m_connection[start][i]) {
                return true;
            }
        }
        return false;
    }


    void getSCC() {
        for (int i = 0; i < m_numVertices; ++i) {
            if (m_dfn[i] == -1) {
                DFS(i);
            }
        }
    }


    void DFS(int start) {

        m_dfn[start] = m_low[start] = ++m_count;
        m_stack.push(start);
        m_inStack[start] = true;

        // 邻接矩阵 https://www.bilibili.com/video/BV19J411J7AZ?p=5
/*        for (int i = 0; i < m_numVertices; ++i) {
            if (isConnected(start, i)) {
                if (m_dfn[i] == 0) {
                    DFS(i);
                    m_low[start] = m_low[start] > m_low[i] ? m_low[i] : m_low[start];
                } else if (m_stack.contains(i)) {
                    m_low[start] = m_low[start] > m_low[i] ? m_low[i] : m_low[start];
                }
            }
        }*/

        // 邻接表 https://segmentfault.com/a/1190000039149539
        for (int i = 0; i < m_connection[start].size(); ++i) {
            // 从邻接表选一个start可达的结点end
            int end = m_connection[start][i];

            // end结点没有被访问
            if (m_dfn[end] == -1) {
                // 递归调用
                DFS(end);
                // 更新能够到达的上层结点
                m_low[start] = m_low[start] > m_low[end] ? m_low[end] : m_low[start];
            } else if (m_inStack[end]) {// 在栈中且被访问过
                // 因为start能够抵达end 如果end的时间戳更小 则更新start的low
                m_low[start] = m_low[start] > m_dfn[end] ? m_dfn[end] : m_low[start];
            }

        }


        if (m_dfn[start] == m_low[start]) {
            //
            QList<int> strongComponent;// 一个强连通分支

            int temp = -1;
            while (start != temp) {
                temp = m_stack.pop();
                strongComponent.push_back(temp);
                m_inStack[temp] = false;
            }
            m_sccs.push_back(strongComponent);
        }

    }

};

// 非递归版本 Tarjan_algorithm
class SCC {

    // 非递归版本
    SCC() {

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


// 有向无环图的拓扑排序  用Tarjan算法遍历一次图 若
class TopologySort {

    TopologySort() {

        // 检查是否有环 Tarjan_algorithm 需要参数  m_connection


        initConnect();



        // 初始化度数
        for (int i = 0; i < m_connection.size(); ++i) {
            m_outDegree.push_back(m_connection[i].size());
            m_inDegree.push_back(0);
            inStack.push_back(false);
        }
        // 初始化入度
        for (int i = 0; i < m_connection.size(); ++i) {
            for (int j = 0; j < m_connection[i].size(); ++j) {
                m_inDegree[m_connection[i][j]]++;
            }
        }

        // test
        //qDebug() << inStack << "  inStack";
        //qDebug() << m_inDegree << "  inDegree";

        // 有向图的入度为0的

        for (int i = 0; i < m_connection.size(); ++i) {
            QList<int> temp;
            if (m_inDegree[i] == 0) {
                inStack[i] = true;//
                for (int j = 0; j < m_connection[i].size(); ++j) {
                    m_inDegree[j]--;
                }
                temp.push_back(i);
            }
            qDebug() << temp;
        }
        //result.push_back(temp);

        int num = 0;
        while (num < m_connection.size()) {
            for (int i = 0; i < m_connection.size(); ++i) {
                if (!inStack[i]) {

                }
            }
        }

    }

private:
    QList<QList<int>> m_connection;//有向无环图的邻接表
    QList<bool> inStack;// 结点i是否遍历过
    QList<QList<int>> result;// 排序的结果
    QStack<int> stack;

    QList<int> m_outDegree;//出度
    QList<int> m_inDegree;//入度

    void initDegree() {
        for (int i = 0; i < m_connection.size(); ++i) {
            m_outDegree.push_back(m_connection[i].size());
        }
    }

    void initConnect() {
        // 有向无环图 DAG directed acyclic graph
        m_connection = {
                {1, 5},//0
                {},//1
                {3, 4},//2
                {1},//3
                {},//4
                {6},//5
                {}//6
        };
    }


};