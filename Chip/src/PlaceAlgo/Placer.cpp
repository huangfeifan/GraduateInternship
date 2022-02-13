//
// Created by Huangff on 2022/1/20.
//

#include "Placer.h"
#include "ComputePosition.h"
#include "SchematicPlacement.h"

static bool degreeCompare(const IndexDegree &index1, const IndexDegree &index2) {
    // 从大到小进行排序
    return index1.degree > index2.degree;
}

void SortListByDegree(QList<int> &list, QList<int> degreeList) {
    // 根据degree对index进行排序
    QList<IndexDegree> indexList;
    for (int i = 0; i < list.size(); ++i) {
        IndexDegree indexDegree;
        indexDegree.index = list[i];
        indexDegree.degree = degreeList[i];
        indexList.push_back(indexDegree);
    }

    qSort(indexList.begin(), indexList.end(), degreeCompare);

    for (int i = 0; i < list.size(); ++i) {
        list[i] = indexList[i].index;
    }
};

Placement::Placement() {
    m_upDown = true;
    qDebug() << "Placement----------------------------------------------------";
    GetGraphSccs tarjanAlgo(graphData);
    m_sccs = tarjanAlgo.getGraphAllScc();
    m_connection = graphData;

    simplePlace();/// 深度摆放

    // 初始化scc数据
    initScc();

    // 单独摆放一个强连通分支
    placeAScc();

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

void Placement::init() {
    // FakeData
    //m_connection = graphData;

    // 模块个数
    m_moduleCount = m_connection.size();

    // 初始化链表
    for (int i = 0; i < m_moduleCount; ++i) {
        m_moduleDegree.push_back(ModuleSize());
        m_moduleSize.push_back(ModuleSize());

        // 存储最原始的Info
        QString str("Module_" + QString::number(i));
        m_nameList.push_back(str);
    }
    qDebug() << m_nameList.size() << " nameListSize ";

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
        qDebug() << m_relativePos << "  m_relativePos";
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

        //SortListByDegree();

    }

    qDebug() << m_connection << "After Sort----ConnectionData";
}

void Placement::sortIndexList(QList<IndexDegree> &list) {

    qSort(list.begin(), list.end(), degreeCompare);
}

void Placement::initScc() {
    qDebug() << "InitScc---------------------------------";

    // sccData
    QHash<int, int> hash;// 用于生成m_connectionScc
    QVector<QList<int>> temp = m_connection;
    QList<int> indexList; // todo modify QList to QVector
    QHash<int, int> indexHash;
    QList<int> orderList;// index拓扑排序的编号 // todo modify QList to QVector


    for (int i = 0; i < m_sccs.size(); ++i) {
        // 存储强连通分支的Info
        m_nameListScc.push_back(QString());
    }
    qDebug() << m_nameListScc.size() << " nameListSccSize";

    int sccCount = 0;// 有几个强连通分支需要重新计算connectData
    QList<int> sccSize;

    // connect 存在hash中
    for (int i = 0; i < m_sccs.size(); ++i) {
        QString str("Module");
        if (m_sccs[i].size() > 1) {
            sccCount++;
            sccIndex = i;// 第i个强连通分支
            sccSize.push_back(m_sccs[i].size());
            for (int j = 0; j < m_sccs[i].size(); ++j) {
                hash.insert(m_sccs[i][j], m_sccs[i][0]);
                str = str + "_" + QString::number(m_sccs[i][j]);
                // todo modify
                sccList.push_back(QString::number(m_sccs[i][j]));
                sccIndexHash.insert(m_sccs[i][j], j);

                m_moduleSccIndex.insert(m_sccs[i][j], i);
            }
        } else {
            str = str + "_" + QString::number(m_sccs[i][0]);
            m_moduleSccIndex.insert(m_sccs[i][0], i);

        }
        m_nameListScc[i] = str;
        indexList.push_back(m_sccs[i][0]);
        // 通过index 可以找到在indexList的下表
        indexHash.insert(m_sccs[i][0], i);
        QList<int> list;
        m_connectionScc.push_back(list);
        orderList.push_back(0);
    }

    qDebug() << sccCount << " Scc_count";
    qDebug() << sccList << " sccList";
    qDebug() << sccIndexHash << " sccIndexHash";
    qDebug() << m_moduleSccIndex << " m_moduleSccIndex";
    //qDebug() << sccSize << " sccSize";  // todo modify 先只计算一个强连通分支内部的摆放
    QVector<QVector<QList<int>>> allSccConnect(sccCount);//
    //qDebug() << allSccConnect.size() << " allSccConnect";

    //qDebug() << m_connectionScc << "     m_connectionScc  " << m_connectionScc.size();
    // 通过hash 构造出m_connectionScc
    for (int i = 0; i < m_connection.size(); ++i) {
        for (int j = 0; j < m_connection[i].size(); ++j) {
            int startIndex = i;
            if (hash.contains(startIndex)) {
                startIndex = hash.value(startIndex);
            }
            startIndex = indexHash.value(startIndex);
            int endIndex = m_connection[i][j];
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
    qDebug() << m_nameListScc << "     m_nameListScc";
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
    GetTopologySort topologySort(m_connectionScc);
    QList<QStack<int>> tSort = topologySort.m_result;
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
    PlaceSccs::simpleAdjust(m_relativePositionScc);


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
    //adjustRelativePosition();
    PlaceSccs::simpleAdjust(m_relativePosition);

    qDebug() << m_relativePosition << "  position_after_adjust";
    qDebug() << m_isOccupy << "  m_isOccupy";
    qDebug() << m_isPlaced << "  m_isPlaced";
    qDebug() << "----------------finish_placement----------------";
    //qDebug() << "----------------compute_Module_Size----------------";


    // 计算模块的大小  假定模块大小给定
    //computeModuleSize();


    //qDebug() << "----------------compute_Module_Size----------------";
    /// 计算模块的绝对位置  Todo add function


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

void Placement::placeAScc() {
    qDebug() << "---------------------------------------PlaceAScc";
    int sccNumber = sccList.size();// 当前强连通分支结点个数
    sccPlaced = QVector<bool>(sccNumber);
    isOccupy = QVector<QHash<int, int>>(sccNumber);
    moduleDegree = QVector<ModuleSize>(sccNumber);
    relativePosition = QVector<QPoint>(sccNumber);
    sccConnect = QVector<QList<int>>(sccNumber);

    QVector<int> indexParentIndex(sccNumber);// 父节点所在的行数

    // 强连通分支内部的下标 <oldIndex--key,newIndex--value>
    qDebug() << sccIndexHash << "  SccIndexHash";
    // 计算connection 摒弃原序号 启用新序号0-sccNumber
    for (int i = 0; i < sccNumber; ++i) {
        int start = sccList[i].toInt();
        qDebug() << start << "   startIndex----";
        for (int j = 0; j < m_connection[start].size(); ++j) {
            int end = m_connection[start][j];
            int newEnd = sccIndexHash.value(end);
            qDebug() << end << "       end ";
            qDebug() << newEnd << "       newEnd ";
            if (m_moduleSccIndex.value(end) == sccIndex) {
                sccConnect[i].push_back(newEnd);
            }
        }
    }
    qDebug() << sccConnect << " sccConnect";
    // 找出度数最大的
    int degree = sccConnect[0].size();// 结点0的度数
    int maxIndex = 0;// 假设结点0度数最大
    for (int i = 1; i < sccConnect.size(); ++i) {
        if (sccConnect[i].size() > degree) {
            // 较大度数出现 更新最大度数的结点
            maxIndex = i;
            degree = sccConnect[i].size();
        }
    }

    qDebug() << maxIndex << "     maxDegreeIndex";
    qDebug() << sccPlaced << "  sccPlaced";
    qDebug() << isOccupy << "  isOccupy";
    // 思想： 先摆放度数最大的点 然后摆放其所有子节点 然后摆放所有子节点的子节点
    int row = sccConnect.size();    // 摆放权重(度数)最大的
    int column = 0;
    while (isOccupy[column].key(row)) {
        row++;
    }
    isOccupy[column].insert(row, row);// 存储index占据的位置
    relativePosition[maxIndex].setX(column++);// 列
    relativePosition[maxIndex].setY(row);// 行
    sccPlaced[maxIndex] = true;// 修改状态

    qDebug() << relativePosition << "   relativePosition";

    QStack<int> currentStack, nextStack;// stack存放的都是新下标
    // 修改最大度数结点的子节点的父节点行号 indexParentIndex 用于描述父节点所在行数
    for (int i = 0; i < sccConnect[maxIndex].size(); ++i) {
        int child = sccConnect[maxIndex][i];// 新moduleIndex
        qDebug() << " child Index " << child;
        indexParentIndex[child] = maxIndex;
        // child没有被摆放 则将其添加到stack中 便于后续摆放
        if (!sccPlaced[child]) {
            currentStack.push(child);
        }
    }
    qDebug() << currentStack << "   CurrentStack";

    qDebug() << row << " FRow--";
    while (!currentStack.isEmpty()) {
        // 摆放topIndex
        int topIndex = currentStack.pop();
        int topParentIndex = indexParentIndex[topIndex];
        qDebug() << topIndex << "   " << topParentIndex << " " << sccConnect[topParentIndex].size();
        row = relativePosition[topParentIndex].y() - sccConnect[topParentIndex].size() * 0.382;

        while (isOccupy[column].contains(row)) {
            row++;
        }
        qDebug() << row << " RRRow";
        isOccupy[column].insert(row, row);
        relativePosition[topIndex].setX(column);// 列
        relativePosition[topIndex].setY(row);// 行
        sccPlaced[topIndex] = true;

        qDebug() << topIndex << " topIndex";
        for (int i = 0; i < sccConnect[topIndex].size(); ++i) {
            // update nextStack;
            int child = sccConnect[topIndex][i];
            // 将未摆放的子节点入栈
            if (!sccPlaced[child]) {
                qDebug() << child << " childChild";
                nextStack.push(child);
                indexParentIndex[child] = topIndex;
            }
        }
        // 当前列摆放结束  接着摆放下一列
        if (currentStack.isEmpty()) {
            currentStack = nextStack;
            nextStack.clear();
            column++;// 当前列摆放结束 继续摆放下一列
        }
        qDebug() << column << " Column------Current   " << row;
        qDebug() << currentStack << " currentStack";
        qDebug() << nextStack << " nextStack";
    }
    qDebug() << relativePosition << "   relativePosition";
    PlaceSccs::simpleAdjust(relativePosition);

}

