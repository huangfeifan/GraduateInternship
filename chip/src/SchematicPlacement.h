//
// Created by Huangff on 2021/12/15.
//

#pragma once

#include <QVector>
#include <QStack>
#include <QDebug>
#include <algorithm>

#include "Data.h"

// 递归版本 Trajan_algorithm
class SchematicPlacement {
    // Time Complexity O(N+E) Space Complexity O(N+E) 邻接表存储数据
    // link https://www.bilibili.com/video/BV19J411J7AZ?p=5 邻接矩阵
    // link https://segmentfault.com/a/1190000039149539  邻接表
public :

    // 递归版本
    SchematicPlacement() {

        qDebug() << "SchematicPlacement----------------------------------------------------";

        // 初始化邻接表
        initConnection();

        // init
        for (int i = 0; i < m_numVertices; ++i) {
            m_dfn.push_back(-1);
            m_low.push_back(-1);
            m_inStack.push_back(false);
        }

        //qDebug() << m_connection.size();
        getSCC();

        //qDebug() << m_sccs.size() << "  scc_size";
        for (int i = 0; i < m_sccs.size(); ++i) {
            qDebug() << m_sccs[i];
        }

        qDebug() << "SchematicPlacement----------------------------------------------------";

    }

    QList<QList<int>> m_sccs;// strongly connected components 保存强连通分量

private:
    int m_numVertices = 0;// 图顶点个数
    int m_count = 0;// 搜索的次序编号(时间戳)
    QVector<int> m_dfn; // 结点i搜索的次序编号(时间戳)  表示真实访问次序
    QVector<int> m_low; // 结点i或i的子结点能够追溯到的最早时间戳 通过邻接表以及子结点的dfn来实现更新
    QVector<bool> m_inStack; // 结点是否在栈内
    QStack<int> m_stack; // 用于回退结点


    QList<QList<int>> m_connection;//邻接表

    void initConnection() {

        m_connection = graphData;

        // number 模块数量
        m_numVertices = m_connection.size();

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

struct ModuleSize {
    int width = 0;
    int height = 0;
};

struct IndexDegree {
    int index;
    int degree;
};


class Placement {

public :
    // place scc
    int m_moduleCountScc = 0;
    QList<bool> m_isPlacedScc;// 模块是否被摆放
    QList<QHash<int, int>> m_isOccupyScc;// 位置被占据
    QList<QList<int>> m_connectionScc;
    QList<ModuleSize> m_moduleDegreeScc;// 记录模块的度数  width 出度 height 入度
    QList<QPoint> m_relativePositionScc;// 改进的行列定位法坐标 相对坐标的计算

    void placeIndexAndChild(int index, int row, int column, QList<int> orderList);

public:

    int m_minWidth = 20;
    int m_minHeight = 20;
    int width_gap = 20;
    int height_gap = 20;
    bool m_upDown = false;


    QList<QPoint> getRelativePosition() {
        return m_relativePosition;
    };

    QList<QPoint> getRealPosition() {
        return m_realPosition;
    };

    QList<ModuleSize> getModuleSize() {
        return m_moduleSize;
    };

    QList<ModuleSize> getModuleDegree() {
        return m_moduleDegree;
    };

    Placement();

private:

    void computeModuleSize();

    void computeModuleRealPosition();

    QList<int> computeTraversalOrder();

    QList<int> computeTraversalOrder1();

    void breadthPlacement();

    void placeChildStack(QList<int> childList, int row, int column);

    void simplePlace();

    void deepPlacement();

    void adjustRelativePosition();

    void placeChildRecursion(int index, int row, int column);

    void placeParent(int index, int row, int column);

    void init();// 初始化所有数据
    void initScc();// 初始化所有scc

    void sortConnectionData();

    void sortIndexList(QList<IndexDegree> &list);

    void computeSccInfo();

private:
    QList<QList<int>> m_sccs;// strong connected component

    QList<QList<int>> m_parent;//

    QList<QPoint> m_realPosition;// 每个模块实际的坐标
    QList<ModuleSize> m_moduleSize;// 记录模块的大小


    int m_moduleCount = 0;
    QList<bool> m_isPlaced;// 模块是否被摆放
    QList<QHash<int, int>> m_isOccupy;// 位置被占据
    QList<QList<int>> m_connection;
    QList<ModuleSize> m_moduleDegree;// 记录模块的度数  width 出度 height 入度
    QList<QPoint> m_relativePosition;// 改进的行列定位法坐标 相对坐标的计算




};


// 非递归版本 Tarjan_algorithm
class SCC {

public:
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

// 有向无环图的拓扑排序  用Tarjan算法遍历一次图
class TopologySort {

public:
    TopologySort(QList<QList<int>> graphData) {

        m_connection = graphData;

        qDebug() << "TopologySort----------------------------------------------------";

        // 检查是否有环 Tarjan_algorithm 需要参数  m_connection
        initConnect();

        computeResult();

        detectVerticesNum();

        qDebug() << "TopologySort----------------------------------------------------";

        //initParent();
    }

    QList<QStack<int>> result;// 拓扑排序结果

private:
    QList<QList<int>> m_connection;//有向无环图的邻接表


    QList<int> m_inDegree;//入度
    //QList<int> m_outDegree;//出度

    QStack<int> stack;
    QStack<int> qStack;

    bool isDAG = true;//是有向无环图 DAG :=  Directed Acyclic Graph


    // 初始化connect数据
    void initConnect() {
        // 有向无环图 DAG directed acyclic graph
        for (int i = 0; i < m_connection.size(); ++i) {
            m_inDegree.push_back(0);
        }
        // 初始化入度
        for (int i = 0; i < m_connection.size(); ++i) {
            for (int j = 0; j < m_connection[i].size(); ++j) {
                m_inDegree[m_connection[i][j]]++;
            }
        }
    }

    void computeResult() {
        // 有向图的入度为0的
        for (int i = 0; i < m_connection.size(); ++i) {
            QList<int> temp;
            if (m_inDegree[i] == 0) {
                qStack.push_back(i);
            }
        }
        //qDebug() << qStack;
        result.push_back(qStack);

        //qDebug() << "while----";
        while (!qStack.isEmpty()) {
            int index = qStack.pop();
            //qDebug() << qStack;
            for (int i = 0; i < m_connection[index].size(); ++i) {
                int temp = m_connection[index][i];
                m_inDegree[temp]--;
                if (m_inDegree[temp] == 0) {
                    stack.push_back(temp);
                }
            }

            // 更新qStack
            if (qStack.isEmpty()) {
                qStack = stack;
                //qDebug() << qStack << "-------------";
                if (!qStack.isEmpty()) {
                    result.push_back(qStack);
                }
                stack.clear();
            }
        }
        //qDebug() << "while----end";
        qDebug() << "****    Topology sort result:\n" << result;
    }

    void detectVerticesNum() {
        // 拓扑排序的顶点数
        int sortVerticesNum = 0;
        for (int i = 0; i < result.size(); ++i) {
            sortVerticesNum += result[i].size();
        }

        // 拓扑排序顶点数不等于有向图的顶点数  说明有向图有环
        if (sortVerticesNum != m_connection.size()) {
            isDAG = false;
            qDebug() << " has circuit";
        } else {
            qDebug() << " has not circuit";
        }
    }

    // 根据邻接表计算每个顶点的父节点
    void initParent() {
        QList<QList<int>> parent;//

        for (int i = 0; i < m_connection.size(); ++i) {
            QList<int> temp;
            parent.push_back(temp);
        }

        for (int i = 0; i < m_connection.size(); ++i) {
            for (int j = 0; j < m_connection[i].size(); ++j) {
                int end = m_connection[i][j];
                parent[end].push_back(i);
            }
        }

        qDebug() << parent << " IndexParent";
    }


};