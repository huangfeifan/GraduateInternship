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

class Placement {

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
    }

    QList<ModuleSize> getModuleSize() {
        return m_moduleSize;
    }

    QList<ModuleSize> getModuleDegree() {
        return m_moduleDegree;
    }

    Placement();

private:

    void computeModuleSize();

    void computeModuleRealPosition();

    QList<int> computeTraversalOrder() ;

    QList<int> computeTraversalOrder1();

    void computeRelativePosition1();

    void placeChildStack(QList<int> childList, int row, int column);

    void computeRelativePosition();

    void adjustRelativePosition();

    void placeChildRecursion(int index, int row, int column);

    void placeParent(int index, int row, int column);

    void init();


private:

    QList<QList<int>> m_connection;
    QList<QList<int>> parent;//

    QList<QList<int>> m_sccs;// strong connected component

    QList<bool> isPlaced;// 模块是否被摆放
    QList<QHash<int, int>> isOccupy;// 位置被占据

    int m_moduleCount = 0;
    QList<QPoint> m_relativePosition;// 改进的行列定位法坐标 相对坐标的计算
    QList<QPoint> m_realPosition;// 每个模块实际的坐标
    QList<ModuleSize> m_moduleSize;// 记录模块的大小
    QList<ModuleSize> m_moduleDegree;// 记录模块的度数  width 出度 height 入度

};

Placement::Placement() {
    m_upDown = true;
    qDebug() << "Placement----------------------------------------------------";
    // 初始化所有数据
    init();

    // 计算模块的相对位置
    computeRelativePosition1();
    //computeRelativePosition();

    // 对计算出来的相对位置 进行微调  todo modify
    adjustRelativePosition();

/*        qDebug() << m_relativePosition << "  position_after_adjust";
        qDebug() << isOccupy << "  isOccupy";
        qDebug() << isPlaced << "  isPlaced";*/
    qDebug() << "----------------finish_placement----------------";
    //qDebug() << "----------------compute_Module_Size----------------";

    // 计算模块的大小
    computeModuleSize();


    //qDebug() << "----------------compute_Module_Size----------------";
    // 计算模块的绝对位置
    computeModuleRealPosition();

    qDebug() << "Placement----------------------------------------------------\n";

}

void Placement::computeModuleSize() {
    // 更新模块的大小
    // 根据模块的出度入度就可以得到模块的宽高 默认只有左右端口 所以宽度默认
    for (int i = 0; i < m_moduleCount; ++i) {
        m_moduleSize[i].width = m_minWidth;
        int height = m_moduleDegree[i].width > m_moduleDegree[i].height ? m_moduleDegree[i].width
                                                                        : m_moduleDegree[i].height;
        m_moduleSize[i].height = m_minHeight + height * height_gap;
    }

    // 打印 Module Size
    for (int i = 0; i < m_moduleCount; ++i) {
        //qDebug() << "Module__" << i << " width " << m_moduleSize[i].width << " , height " << m_moduleSize[i].height;
    }
}

void Placement::computeModuleRealPosition() {
    /**
     * 目的：确定每个元件的布局最终位置
     *  已有各个模块的大小 模块的相对位置
     *
     *  难点：如何根据模块间的连接信息得到模块的最终位置
     *
     *  问题分解
     *      1.类似行列定位的思路 首先确定每一列的宽度 其次确定列与列之间的间距(根据通道的概念 预留出足够的空间) 最后得到每一列的坐标
     *      2.按照列的方式 依然根据通道的概念 预留出足够的空间 得到每一行的坐标
     *      3.对通道数据的存储 辅助线探索判断后续的走线  线探索需要处理的事情  Todo : need consider
     *
     *  如何计算列与列的通道数
     *  如何计算行与行的通道数
     */


}

QList<int> Placement::computeTraversalOrder() {
    // 出度排序 根据出度遍历 todo modify

    QList<int> orderList;
/*        // 强连通分量个数
        int sccSize = m_sccs.size();
        // todo modify---
        if (sccSize > 1) {
            // 多个强连通分量
        } else if (sccSize == 1) {
            // 只有一个强连通分量
        } else if (!sccSize) {
            qDebug() << "error";
        }*/

    for (int i = m_sccs.size() - 1; i >= 0; --i) {
/*            if (m_sccs[i].size() == 0) {
                // 连通分支只有一个结点
                orderList.push_back(m_sccs[i][0]);
            } else {
                // 对强连通分量的结点进行排序 根据连接关系(包括但不限于出入度) todo modify
                QList<int> degree;
                int maxIndex = 0;
                qDebug() << m_sccs[i] << "   scc_i";
                for (int j = 1; j < m_sccs[i].size(); ++j) {
                    int index = m_sccs[i][j];
                    maxIndex = m_connection[index][0];
                    if (m_connection[index].size() > m_connection[maxIndex].size()) {
                        maxIndex = index;
                        degree.push_front(maxIndex);
                    } else {
                        degree.push_back(index);
                    }
                }
                qDebug() << degree;

                for (int j = 0; j < degree.size(); ++j) {
                    orderList.push_back(degree[j]);
                }
            }*/

        // 强连通分支内部不调整次序
        for (int j = 0; j < m_sccs[i].size(); ++j) {
            orderList.push_back(m_sccs[i][j]);
        }

/*            // 根据输出输出关系 调整强连通分支结点的次序
            int index = m_sccs[i][0];
            for (int j = 1; j < m_sccs[i].size(); ++j) {
                int newIndex = m_sccs[i][j];
                if (m_connection[index].size() < m_connection[newIndex].size()) {
                    index = newIndex;
                }
            }
            orderList.push_back(index);*/


    }
    qDebug() << orderList << " orderList_print";

    return orderList;
}

QList<int> Placement::computeTraversalOrder1() {
    // 出度排序 根据出度遍历 todo modify

    QList<int> orderList;
    QList<int> m_sccIndex;// 记录每个module在第几个scc中
    for (int i = 0; i < m_moduleCount; ++i) {
        m_sccIndex.push_back(0);
    }

    for (int i = m_sccs.size() - 1; i >= 0; --i) {
/*            if (m_sccs[i].size() == 0) {
                // 连通分支只有一个结点
                orderList.push_back(m_sccs[i][0]);
            } else {
                // 对强连通分量的结点进行排序 根据连接关系(包括但不限于出入度) todo modify
                QList<int> degree;
                int maxIndex = 0;
                qDebug() << m_sccs[i] << "   scc_i";
                for (int j = 1; j < m_sccs[i].size(); ++j) {
                    int index = m_sccs[i][j];
                    maxIndex = m_connection[index][0];
                    if (m_connection[index].size() > m_connection[maxIndex].size()) {
                        maxIndex = index;
                        degree.push_front(maxIndex);
                    } else {
                        degree.push_back(index);
                    }
                }
                qDebug() << degree;

                for (int j = 0; j < degree.size(); ++j) {
                    orderList.push_back(degree[j]);
                }
            }*/

/*            // 强连通分支内部不调整次序
            for (int j = 0; j < m_sccs[i].size(); ++j) {
                orderList.push_back(m_sccs[i][j]);
            }*/

        // 根据输出输出关系 调整强连通分支结点的次序
        int index = m_sccs[i][0];
        for (int j = 0; j < m_sccs[i].size(); ++j) {
            int newIndex = m_sccs[i][j];
            m_sccIndex[newIndex] = i;// 记录module在第几个scc中
            if (m_connection[index].size() < m_connection[newIndex].size()) {
                index = newIndex;
            }
        }
        orderList.push_back(index);


    }
    qDebug() << orderList << " orderList_print";
    qDebug() << m_sccIndex << " SccList ";


    return orderList;
}

void Placement::computeRelativePosition1() {
    /**
     * 从每个强连通分支取一个权重(出度/子结点个数)最大的结点 得到一个遍历序列
     *  对遍历序列进行遍历
     *  从强连通分支的权重最大结点出发 对当前结点的所有子节点进行摆放 然后摆放子节点的子节点(对于有向无环图也能得到非常不错的布局结果)
     *
     */


    // 遍历顺序
    QList<int> traverseOrderList = computeTraversalOrder1();
    //qDebug() << traverseOrderList<<"      ***************           ";

    for (int i = 0; i < traverseOrderList.size(); ++i) {

        int index = traverseOrderList[i];// index
        if (isPlaced[index]) {
            continue;
        }

        //qDebug() << index << "  index_for";

        // 摆放index
        int row = m_moduleCount;
        //qDebug() << row << "-------row--while";

        while (isOccupy[m_moduleCount].key(row)) {
            // 第row行 第verticesNum列 被占据
            row++;
        }

        //qDebug() << row << "--row";
        //qDebug() << "   tttt ";

        isOccupy[m_moduleCount].insert(row, row);// 存储index所占据位置

        m_relativePosition[index].setX(m_moduleCount);// column 列
        m_relativePosition[index].setY(row);  // row 行
        isPlaced[index] = true;// 修改摆放状态

        QList<int> childList;
        for (int j = 0; j < m_connection[index].size(); ++j) {
            int temp = m_connection[index][j];
            if (!isPlaced[temp]) {
                childList.push_back(temp);
            }
        }
        qDebug() << childList << "               ChildList";
        // 摆放index的child        广度遍历所有子节点
        placeChildStack(childList, row, m_moduleCount);
        //qDebug() << index << " index(column/row) " << m_moduleCount << " " << row;

    }

    qDebug() << m_relativePosition << "  position_before_adjust";
}

void Placement::placeChildStack(QList<int> childList, int row, int column) {

    int indexColumn = column + 1;
    int indexRow = row - childList.size() / 2;
    for (int i = 0; i < childList.size(); ++i) {
        int index = childList[i];
        if (isPlaced[index]) {
            continue;
        }
        while (isOccupy[indexColumn].key(indexRow)) {
            // 第indexRow行 第indexColumn列 被占据
            indexRow++;
        }

        isOccupy[indexColumn].insert(indexRow, indexRow);// 存储index所占据位置  第indexColumn列的indexRow行被占据
        m_relativePosition[index].setX(indexColumn);// indexColumn 列
        m_relativePosition[index].setY(indexRow);  // indexRow 行
        isPlaced[index] = true;// 修改摆放状态

        //indexRow++;
    }


}

void Placement::computeRelativePosition() {
    // 遍历顺序
    QList<int> traverseOrderList = computeTraversalOrder();


    for (int i = 0; i < traverseOrderList.size(); ++i) {

        int index = traverseOrderList[i];// index
        if (isPlaced[index]) {
            continue;
        }

        //qDebug() << index << "  index_for";

        // 摆放index
        int row = m_moduleCount;
        //qDebug() << row << "-------row--while";

        while (isOccupy[m_moduleCount].key(row)) {
            // 第row行 第verticesNum列 被占据
            row--;
        }
        //qDebug() << row << "--row";
        if (!isOccupy[m_moduleCount].key(row)) {

            //qDebug() << "   tttt ";

            isOccupy[m_moduleCount].insert(row, row);// 存储index所占据位置

            m_relativePosition[index].setX(m_moduleCount);// column 列
            m_relativePosition[index].setY(row);  // row 行
            isPlaced[index] = true;// 修改摆放状态

            qDebug() << index << " index(column/row) " << m_moduleCount << " " << row;

            // 递归地摆放index的child    深度遍历所有子节点
            placeChildRecursion(index, row, m_moduleCount);
        }

        // 摆放index的parent
        //placeParent(index, m_moduleCount, m_moduleCount);

    }

    qDebug() << m_relativePosition << "  position_before_adjust";

}

void Placement::adjustRelativePosition() {
    /**
     * 根据图的父子关系得到的布局可能存在父子结点高度差的方差很大
     * 对后期布线可能有比较大的影响
     * 为了解决这个问题
     * 考虑对现有的布局进行一个微调
     * 来增加布局的美观性
     */
    int minRow = m_relativePosition.size();
    int minColumn = m_relativePosition.size();
    for (int i = 0; i < m_relativePosition.size(); ++i) {

        minRow = m_relativePosition[i].y() > minRow ? minRow : m_relativePosition[i].y();
        minColumn = m_relativePosition[i].x() > minColumn ? minColumn : m_relativePosition[i].x();

    }

    for (int i = 0; i < m_relativePosition.size(); ++i) {
        m_relativePosition[i].setX(m_relativePosition[i].x() - minColumn);
        m_relativePosition[i].setY(m_relativePosition[i].y() - minRow);
    }


}

void Placement::placeChildRecursion(int index, int row, int column) {
    // 父结点index 放在第row行 第column列
    // 子结点 first 放在index右侧
    for (int i = 0; i < m_connection[index].size(); ++i) {

        int first = m_connection[index][i];

        if (isPlaced[first]) {
            continue;
        }
        //qDebug() << " parent_index " << index << " child_index " << first;

        int firstRow = row;
        //firstRow = m_upDown ? row + 1 : row ;// todo modify  设置偏移量 多少合适呢
        //m_upDown = !m_upDown;
        while (isOccupy[column + 1].contains(firstRow)) {
            firstRow++;
        }
        isOccupy[column + 1].insert(firstRow, firstRow);

        m_relativePosition[first].setX(column + 1);  // column 列
        m_relativePosition[first].setY(firstRow); // row 行
        isPlaced[first] = true;

        qDebug() << first << " index(column/row) " << column + 1 << " " << firstRow;

        // 摆放first的child
        placeChildRecursion(first, firstRow, column + 1);
    }
}

void Placement::placeParent(int index, int row, int column) {
    // 结点index 放在第column列 第row行
    // 父节点first放在子结点左侧 避免冲突
    for (int i = 0; i < parent[index].size(); ++i) {
        int first = parent[index][i];
        if (!isPlaced[first]) {
            // index的父节点first没有访问
            int firstRow = row - 1;
            while (isOccupy[column - 1].contains(firstRow)) {
                firstRow--;
            }
            isOccupy[column - 1].insert(firstRow, firstRow);// 存储index所占据位置
            isPlaced[first] = true;

            placeParent(first, firstRow, column - 1);
            //placedModuleNum++;
        }
    }
}

void Placement::init() {
    // FakeData
    m_connection = graphData;

    SchematicPlacement tarjanAlgo;
    m_sccs = tarjanAlgo.m_sccs;

    // 模块个数
    m_moduleCount = m_connection.size();

    // 初始化链表
    for (int i = 0; i < m_moduleCount; ++i) {
        m_moduleDegree.push_back(ModuleSize());
        m_moduleSize.push_back(ModuleSize());
    }

    for (int i = 0; i < m_moduleCount; ++i) {
        // width 出度
        m_moduleDegree[i].width = m_connection[i].size();
    }


    for (int i = 0; i < m_connection.size(); ++i) {

        parent.push_back(QList<int>());

        isPlaced.push_back(false);

        m_relativePosition.push_back(QPoint());
        m_realPosition.push_back(QPoint());

        // height 入度
        for (int j = 0; j < m_connection[i].size(); ++j) {
            int end = m_connection[i][j];
            // 入度
            m_moduleDegree[end].height++;
        }
    }

    for (int i = 0; i < m_connection.size(); ++i) {
        for (int j = 0; j < m_connection[i].size(); ++j) {
            int end = m_connection[i][j];
            parent[end].push_back(i);
        }

        isOccupy.push_back(QHash<int, int>());
    }

    for (int i = m_connection.size(); i < m_connection.size() * 2; ++i) {
        isOccupy.push_back(QHash<int, int>());
    }



    // 打印出入度
/*        for (int i = 0; i < m_moduleCount; ++i) {
            qDebug() << "Module__" << i << " out_degree " << m_moduleDegree[i].width << " , in_degree "
                     << m_moduleDegree[i].height;
        }*/


/*        qDebug() << isOccupy << "  isOccupy";
        qDebug() << isPlaced << "  isPlaced";
        qDebug() << m_relativePosition << "  m_relativePosition";
        qDebug() << parent << " parent";*/

}


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
    TopologySort() {

        qDebug() << "TopologySort----------------------------------------------------";

        // 检查是否有环 Tarjan_algorithm 需要参数  m_connection
        initConnect();

        computeResult();

        detectVerticesNum();

        qDebug() << "TopologySort----------------------------------------------------";

        initParent();
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

        m_connection = graphData;

        initDegree();
    }

    void initDegree() {
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

        qDebug() << parent;
    }


};