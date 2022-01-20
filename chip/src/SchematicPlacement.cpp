﻿//
// Created by Huangff on 2022/1/20.
//

#include "SchematicPlacement.h"
#include "AfterPlacement.h"

Placement::Placement() {
    m_upDown = true;
    qDebug() << "Placement----------------------------------------------------";
    SchematicPlacement tarjanAlgo;
    m_sccs = tarjanAlgo.m_sccs;
    m_connection = graphData;

    //simplePlace();

    //初始化scc数据
    initScc();


    qDebug() << "Placement----------------------------------------------------\n";

}

void Placement::computeModuleSize() {
    // 模块大小可能是确定的  所以也就不需要计算
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
     *  如何计算列与列的通道数     粗略计算  todo modify
     *  如何计算行与行的通道数     粗略计算
     */

    // 计算行列宽度和间距 最终得到模块的最终位置
/*    for (int i = 0; i < m_moduleCount; ++i) {
        m_moduleSize
    }*/

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

void Placement::breadthPlacement() {
    /**
     * 从每个强连通分支取一个权重(出度/子结点个数)最大的结点 得到一个遍历序列
     *  对遍历序列进行遍历
     *  从强连通分支的权重最大结点出发 对当前结点的所有子节点进行摆放 然后摆放子节点的子节点(对于有向无环图也能得到非常不错的布局结果)
     *
     *
     *  首先计算大概的摆放顺序 其次根据大致的摆放顺序摆放Module
     */


    // 出度排序 根据出度遍历 step1  todo modify

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


    //return orderList;


    // 遍历顺序
    QList<int> traverseOrderList = orderList;
    //qDebug() << traverseOrderList<<"      ***************           ";

    for (int i = 0; i < traverseOrderList.size(); ++i) {

        int index = traverseOrderList[i];// index
        if (m_isPlaced[index]) {
            continue;
        }

        //qDebug() << index << "  index_for";

        // 摆放index
        int row = m_moduleCount;
        //qDebug() << row << "-------row--while";

        while (m_isOccupy[m_moduleCount].key(row)) {
            // 第row行 第verticesNum列 被占据
            row++;
        }

        //qDebug() << row << "--row";
        //qDebug() << "   tttt ";

        m_isOccupy[m_moduleCount].insert(row, row);// 存储index所占据位置

        m_relativePosition[index].setX(m_moduleCount);// column 列
        m_relativePosition[index].setY(row);  // row 行
        m_isPlaced[index] = true;// 修改摆放状态

        QList<int> childList;
        for (int j = 0; j < m_connection[index].size(); ++j) {
            int temp = m_connection[index][j];
            if (!m_isPlaced[temp]) {
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
        if (m_isPlaced[index]) {
            continue;
        }
        while (m_isOccupy[indexColumn].key(indexRow)) {
            // 第indexRow行 第indexColumn列 被占据
            indexRow++;
        }

        m_isOccupy[indexColumn].insert(indexRow, indexRow);// 存储index所占据位置  第indexColumn列的indexRow行被占据
        m_relativePosition[index].setX(indexColumn);// indexColumn 列
        m_relativePosition[index].setY(indexRow);  // indexRow 行
        m_isPlaced[index] = true;// 修改摆放状态

        //indexRow++;
    }


}

void Placement::deepPlacement() {
    // 遍历顺序
    QList<int> traverseOrderList = computeTraversalOrder();


    for (int i = 0; i < traverseOrderList.size(); ++i) {

        int index = traverseOrderList[i];// index
        if (m_isPlaced[index]) {
            continue;
        }

        //qDebug() << index << "  index_for";

        // 摆放index
        int row = m_moduleCount;
        //qDebug() << row << "-------row--while";

        while (m_isOccupy[m_moduleCount].key(row)) {
            // 第row行 第verticesNum列 被占据
            row--;
        }
        //qDebug() << row << "--row";
        if (!m_isOccupy[m_moduleCount].key(row)) {

            //qDebug() << "   tttt ";

            m_isOccupy[m_moduleCount].insert(row, row);// 存储index所占据位置

            m_relativePosition[index].setX(m_moduleCount);// column 列
            m_relativePosition[index].setY(row);  // row 行
            m_isPlaced[index] = true;// 修改摆放状态

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

        if (m_isPlaced[first]) {
            continue;
        }
        //qDebug() << " parent_index " << index << " child_index " << first;

        int firstRow = row;
        //firstRow = m_upDown ? row + 1 : row ;// todo modify  设置偏移量 多少合适呢
        //m_upDown = !m_upDown;
        while (m_isOccupy[column + 1].contains(firstRow)) {
            firstRow++;
        }
        m_isOccupy[column + 1].insert(firstRow, firstRow);

        m_relativePosition[first].setX(column + 1);  // column 列
        m_relativePosition[first].setY(firstRow); // row 行
        m_isPlaced[first] = true;

        qDebug() << first << " index(column/row) " << column + 1 << " " << firstRow;

        // 摆放first的child
        placeChildRecursion(first, firstRow, column + 1);
    }
}

void Placement::placeParent(int index, int row, int column) {
    // 结点index 放在第column列 第row行
    // 父节点first放在子结点左侧 避免冲突
    for (int i = 0; i < m_parent[index].size(); ++i) {
        int first = m_parent[index][i];
        if (!m_isPlaced[first]) {
            // index的父节点first没有访问
            int firstRow = row - 1;
            while (m_isOccupy[column - 1].contains(firstRow)) {
                firstRow--;
            }
            m_isOccupy[column - 1].insert(firstRow, firstRow);// 存储index所占据位置
            m_isPlaced[first] = true;

            placeParent(first, firstRow, column - 1);
            //placedModuleNum++;
        }
    }
}

static bool degreeCompare(const IndexDegree &index1, const IndexDegree &index2) {
    // 从大到小进行排序
    return index1.degree > index2.degree;
}

void Placement::init() {
    // FakeData
    //m_connection = graphData;

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

        m_parent.push_back(QList<int>());

        m_isPlaced.push_back(false);

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
            m_parent[end].push_back(i);
        }

        m_isOccupy.push_back(QHash<int, int>());
    }

    for (int i = m_connection.size(); i < m_connection.size() * 2; ++i) {
        m_isOccupy.push_back(QHash<int, int>());
    }



    // 对connection进行排序
    sortConnectionData();


    // 打印出入度
/*        for (int i = 0; i < m_moduleCount; ++i) {
            qDebug() << "Module__" << i << " out_degree " << m_moduleDegree[i].width << " , in_degree "
                     << m_moduleDegree[i].height;
        }*/


/*      qDebug() << m_isOccupy << "  m_isOccupy";
        qDebug() << m_isPlaced << "  m_isPlaced";
        qDebug() << m_relativePosition << "  m_relativePosition";
        qDebug() << m_parent << " m_parent";*/

}

void Placement::sortConnectionData() {
    qDebug() << m_connection << "----ConnectionData";
    for (int i = 0; i < m_connection.size(); ++i) {
        if (m_connection[i].size() < 2) {
            continue;
        }
        //qDebug() << m_connection[i] << "  -----       " << i;
        QList<IndexDegree> list;
        for (int j = 0; j < m_connection[i].size(); ++j) {
            IndexDegree indexDegree;
            int index = m_connection[i][j];
            indexDegree.index = index;
            indexDegree.degree = m_moduleDegree[index].width;
            list.push_back(indexDegree);
        }
        //qDebug() << list.size() << " size";
        qSort(list.begin(), list.end(), degreeCompare);

        for (int j = 0; j < m_connection[i].size(); ++j) {
            m_connection[i][j] = list[j].index;
        }
    }
    qDebug() << m_connection << "After Sort----ConnectionData";
}

void Placement::sortIndexList(QList<IndexDegree> &list) {

    qSort(list.begin(), list.end(), degreeCompare);
}

void Placement::computeSccInfo() {
    SchematicPlacement tarjanAlgo;
    m_sccs = tarjanAlgo.m_sccs;
    if (m_sccs.size() == m_moduleCount) {
        // 有向无环图
    } else {
        // 有向有环图


    }


}

void Placement::initScc() {
    // sccData
    QHash<int, int> hash;// 用于生成m_connectionScc
    QList<QList<int>> temp = m_connection;
    QList<int> indexList;
    QHash<int, int> indexHash;
    QList<int> orderList;// index拓扑排序的编号

    // connect 存在hash中
    for (int i = 0; i < m_sccs.size(); ++i) {
        if (m_sccs[i].size() > 1) {
            for (int j = 0; j < m_sccs[i].size(); ++j) {
                hash.insert(m_sccs[i][j], m_sccs[i][0]);
            }
        }
        indexList.push_back(m_sccs[i][0]);
        // 通过index 可以找到在indexList的下表
        indexHash.insert(m_sccs[i][0], i);
        QList<int> list;
        m_connectionScc.push_back(list);
        orderList.push_back(0);
    }
    //qDebug() << m_connectionScc << "     m_connectionScc  " << m_connectionScc.size();
    // 通过hash 构造出m_connectionScc
    for (int i = 0; i < m_connection.size(); ++i) {
        for (int j = 0; j < m_connection[i].size(); ++j) {
            int startIndex = i;
            int endIndex = m_connection[i][j];
            if (hash.contains(startIndex)) {
                startIndex = hash.value(startIndex);
            }
            startIndex = indexHash.value(startIndex);
            if (hash.contains(endIndex)) {
                endIndex = hash.value(endIndex);
            }
            endIndex = indexHash.value(endIndex);
            //qDebug() << "startIndex  " << startIndex << " endIndex " << endIndex;
            if (startIndex != endIndex) {
                m_connectionScc[startIndex].push_back(endIndex);
            }
        }
    }
    qDebug() << hash << "     Hash";
    qDebug() << indexHash << "     indexHash";
    qDebug() << indexList << "     indexList";
    qDebug() << m_connectionScc << "     m_connectionScc";
    qDebug() << "   init connectionScc   ";
    // todo simplify m_connectionScc


    //m_connectionScc = graphData;

    // 模块个数
    m_moduleCountScc = m_connectionScc.size();

    // 初始化链表
    for (int i = 0; i < m_moduleCountScc; ++i) {
        m_moduleDegreeScc.push_back(ModuleSize());
        //m_moduleSize.push_back(ModuleSize());
    }

    for (int i = 0; i < m_moduleCount; ++i) {
        // width 出度
        //m_moduleDegree[i].width = m_connection[i].size();
    }


    for (int i = 0; i < m_connectionScc.size(); ++i) {

        //m_parent.push_back(QList<int>());

        m_isPlacedScc.push_back(false);

        m_relativePositionScc.push_back(QPoint());
        //m_realPosition.push_back(QPoint());

        // height 入度
        /*for (int j = 0; j < m_connectionScc[i].size(); ++j) {
            int end = m_connectionScc[i][j];
            // 入度
            m_moduleDegree[end].height++;
        }*/
    }

    for (int i = 0; i < m_connectionScc.size(); ++i) {
        /*for (int j = 0; j < m_connectionScc[i].size(); ++j) {
            int end = m_connectionScc[i][j];
            m_parent[end].push_back(i);
        }*/

        m_isOccupyScc.push_back(QHash<int, int>());
    }

    for (int i = m_connectionScc.size(); i < m_connectionScc.size() * 2; ++i) {
        m_isOccupyScc.push_back(QHash<int, int>());
    }

    // 摆放---------------------------------------------------------------------------------
    //      Step1.计算拓扑排序序列
    //      Step2.根据拓扑序列摆放Module
    //            根据父子关系摆放
    TopologySort topologySort(m_connectionScc);
    QList<QStack<int>> tSort = topologySort.result;
    //qDebug() << m_connectionScc << "    m_connectionScc";
    qDebug() << tSort << "    topologySort";
    for (int i = 0; i < tSort.size(); ++i) {
        for (int j = 0; j < tSort[i].size(); ++j) {
            orderList[tSort[i][j]] = i;
        }
    }
    qDebug() << orderList << "   orderList";
    // 是否增加排序呢  todo consider
    for (int i = 0; i < tSort.size(); ++i) {
        //qDebug() << tSort[i][0]<< " -----------------------------*************";
        int column = tSort.size() - 1 + i;

        for (int j = 0; j < tSort[i].size(); ++j) {
            placeIndexAndChild(tSort[i][j], tSort.size() - 1, column, orderList);
        }
    }


    // 调整相对位置 相对位置从(0,0)开始
    int minRow = m_relativePositionScc.size();
    int minColumn = m_relativePositionScc.size();
    for (int i = 0; i < m_relativePositionScc.size(); ++i) {

        minRow = m_relativePositionScc[i].y() > minRow ? minRow : m_relativePositionScc[i].y();
        minColumn = m_relativePositionScc[i].x() > minColumn ? minColumn : m_relativePositionScc[i].x();

    }

    for (int i = 0; i < m_relativePositionScc.size(); ++i) {
        m_relativePositionScc[i].setX(m_relativePositionScc[i].x() - minColumn);
        m_relativePositionScc[i].setY(m_relativePositionScc[i].y() - minRow);
    }



/*    AfterPlacement placement(m_relativePositionScc, m_connectionScc);
    placement.show();*/

    // 测试初始化是否出错
/*    qDebug() << m_connectionScc << "  m_connectionScc";
    qDebug() << m_isOccupyScc << "  m_isOccupyScc";
    qDebug() << m_isPlacedScc << "  m_isPlacedScc";
    qDebug() << m_relativePositionScc << "  m_relativePositionScc";*/

    // 对connection进行排序
    //sortConnectionData();
}

void Placement::simplePlace() {
    // 初始化所有数据
    init();

    // 计算模块的相对位置
    //breadthPlacement();// breadth Placement 参照图的广度遍历
    deepPlacement();// deep Placement   参照图的深度遍历

    // 对计算出来的相对位置 进行微调  todo modify
    adjustRelativePosition();

    qDebug() << m_relativePosition << "  position_after_adjust";
    qDebug() << m_isOccupy << "  m_isOccupy";
    qDebug() << m_isPlaced << "  m_isPlaced";
    qDebug() << "----------------finish_placement----------------";
    //qDebug() << "----------------compute_Module_Size----------------";

    // 计算模块的大小
    computeModuleSize();


    //qDebug() << "----------------compute_Module_Size----------------";
    // 计算模块的绝对位置
    computeModuleRealPosition();
}

void Placement::placeIndexAndChild(int index, int row, int column, QList<int> orderList) {
    qDebug() << "placeIndexAndChild-----------------------------------";

    // placeIndex
    if (!m_isPlacedScc[index]) {
        while (m_isOccupyScc[column].contains(row)) {
            row++;
        }
        m_isOccupyScc[column].insert(row, row);
        m_relativePositionScc[index].setX(column);// column
        m_relativePositionScc[index].setY(row);// row
        m_isPlacedScc[index] = true;
        qDebug() << index << " index " << row << " row " << column << " column";
    }


    // placeChild
    QList<int> childList;
    for (int i = 0; i < m_connectionScc[index].size(); ++i) {
        int child = m_connectionScc[index][i];
        if (!m_isPlacedScc[child] && (orderList[index] + 1) == orderList[child]) {
            childList.push_back(child);
        }
    }

    for (int i = 0; i < childList.size(); ++i) {
        int child = childList[i];

        //row = row - childList.size()/2;
        while (m_isOccupyScc[column + 1].contains(row)) {
            row++;
        }
        m_isOccupyScc[column + 1].insert(row, row);
        m_relativePositionScc[child].setX(column + 1);// column
        m_relativePositionScc[child].setY(row);// row
        m_isPlacedScc[child] = true;
        qDebug() << child << " child " << row << " row " << column + 1 << " column";
    }
    qDebug() << m_isPlacedScc << " isPlacedScc";

}
