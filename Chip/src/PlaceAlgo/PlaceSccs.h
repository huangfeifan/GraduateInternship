//
// Created by Huangff on 2022/2/13.
//

#pragma once

#include <QDebug>

class PlaceSccs {

public:
    /// 所有强连通分量的摆放 只处理最基本的数据
    PlaceSccs(const QVector<QList<int>> &connectData) {
        qDebug() << "________________________PlaceSccs________________________";

        /// 初始化所有变量
        preHandleData(connectData);

        /// 计算相对位置
        computePos();

        /// 简单调整相对位置
        adjustPos();

        qDebug() << m_relativePos << " RelativePos";
        qDebug() << "________________________PlaceSccs________________________End";
    }

    void adjustPos() {
        /// 调整相对位置 相对位置从(0,0)开始

        simpleAdjust(m_relativePos);

        // 加入最后几列都只有一个module 则考虑将其拼在一起
        QVector<QList<int>> columnIndexList = QVector<QList<int>>(m_relativePos.size());
        // 初始化数据
        for (int i = 0; i < m_relativePos.size(); ++i) {
            QPoint point = m_relativePos[i];// .x()列 .y()行
            columnIndexList[point.x()].push_back(i);// 存放索引index
        }

        int maxModuleCount = 0;// 按列划分 模块数的最大值
        for (int i = 0; i < columnIndexList.size(); ++i) {
            maxModuleCount = maxModuleCount > columnIndexList[i].size() ? maxModuleCount : columnIndexList[i].size();
        }

        if (maxModuleCount > 1) {
            // 最后两列仅有一个 则将其放在其他列
            int temp = m_relativePos.size() - 1;
            while (columnIndexList[temp].size() == 0) {
                temp--;
            }
            if (columnIndexList[temp].size() == 1) {// 从最后一列往前面列探索
                int index = columnIndexList[temp][0];
                int column = temp - 1;
                // 调整单个模块位置
                int row = m_relativePos[index].y();
                while (m_isPosOccupied[column].contains(row)) {
                    row++;
                }
                m_relativePos[index].setX(column);
                m_relativePos[index].setY(row);
            }
        }

        //qDebug() << columnIndexList << "     ((((((";
        //qDebug() << m_relativePos << "      m_relativePos";
        //qDebug() << maxModuleCount << " )))))";

    }

    void preHandleData(const QVector<QList<int>> &connectData) {
        /// 模块个数
        m_moduleCount = connectData.size();

        /// 初始化数组
        m_isPlaced = QVector<bool>(m_moduleCount);
        m_degree = QVector<QPoint>(m_moduleCount);
        m_relativePos = QVector<QPoint>(m_moduleCount);
        m_isPosOccupied = QVector<QHash<int, int>>(m_moduleCount);
        m_weight = QVector<double>(m_moduleCount);

        /// connect数据
        m_connectData = connectData;

        /// 计算出入度和权重 Todo add
        for (int i = 0; i < m_connectData.size(); ++i) {
            m_degree[i].setX(m_connectData[i].size());
            for (int j = 0; j < m_connectData[i].size(); ++j) {
                // 起点i 终点end
                int end = m_connectData[i][j];
                // x() 出度   y()入度
                int in = m_degree[end].y();
                // 起点i出度加1 终点end入度加1
                m_degree[end].setY(in + 1);
            }
        }
        for (int i = 0; i < m_moduleCount; ++i) {
            // 出度越大越靠左 入度越大越靠右
            m_weight[i] = (m_degree[i].x() - m_degree[i].y());
        }
    }

    static void PlaceSccs::simpleAdjust(QVector<QPoint> &relativePosition) {
        /// 调整相对位置 相对位置从(0,0)开始

        int minRow = relativePosition[0].y();
        int minColumn = relativePosition[0].x();
        for (int i = 0; i < relativePosition.size(); ++i) {
            minRow = relativePosition[i].y() > minRow ? minRow : relativePosition[i].y();
            minColumn = relativePosition[i].x() > minColumn ? minColumn : relativePosition[i].x();
        }
        for (int i = 0; i < relativePosition.size(); ++i) {
            relativePosition[i].setX(relativePosition[i].x() - minColumn);
            relativePosition[i].setY(relativePosition[i].y() - minRow);
        }
    }


    QVector<QPoint> getRelativePos() {
        return m_relativePos;
    };

private:

    void computePos() {
        /// 摆放---------------------------------------------------------------------------------
        //      Step1.计算拓扑排序序列
        //      Step2.根据拓扑序列摆放Module
        //            根据父子关系摆放

        GetTopologySort topologySort(m_connectData);
        QList<QStack<int>> tSort = topologySort.m_result;
        QVector<int> orderList = QVector<int>(m_connectData.size());// index拓扑排序的编号
        for (int i = 0; i < tSort.size(); ++i) {
            for (int j = 0; j < tSort[i].size(); ++j) {
                orderList[tSort[i][j]] = i;
            }
        }

        // 是否增加排序呢  Todo consider Add
        for (int i = 0; i < tSort.size(); ++i) {
            for (int j = 0; j < tSort[i].size(); ++j) {
                //placeIndexAndChild(tSort[i][j], tSort.size() - 1, i, orderList);
                placeIndexAndChild(tSort[i][j], 0, i, orderList);
            }
        }

    }

    void placeIndexAndChild(int index, int row, int column, const QVector<int> &orderList) {
        /// placeIndex
        if (!m_isPlaced[index]) {
            while (m_isPosOccupied[column].contains(row)) {
                row++;
            }
            m_isPosOccupied[column].insert(row, row);
            m_relativePos[index].setX(column);// column
            m_relativePos[index].setY(row);// row
            m_isPlaced[index] = true;
            //qDebug() << index << " index " << row << " row " << column << " column";
        }

        /// placeChild
        QList<int> childList;
        for (int i = 0; i < m_connectData[index].size(); ++i) {
            int child = m_connectData[index][i];
            if (!m_isPlaced[child] && (orderList[index] + 1) == orderList[child]) {
                childList.push_back(child);
            }
        }
        // qlist去重
        QHash<int, int> hash;
        for (int i = 0; i < childList.size(); ++i) {
            if (!hash.contains(childList[i])) {
                hash.insert(childList[i], childList[i]);
            }
        }
        childList.clear();
        QHash<int, int>::const_iterator iter = hash.constBegin();// key--row   value--index
        while (iter != hash.constEnd()) {
            childList.push_back(iter.key());
            ++iter;
        }

        // 以row为基准 尽量在row附近行
        //qDebug() << row << "   *****";
        row = row - childList.size() / 2; // 巧妙???
        //qDebug() << row << "   *****";
        //qDebug() << childList << "   *****";

        for (int i = 0; i < childList.size(); ++i) {
            int child = childList[i];
            while (m_isPosOccupied[column + 1].contains(row)) {
                row++;
            }
            m_isPosOccupied[column + 1].insert(row, row);
            m_relativePos[child].setX(column + 1);// column
            m_relativePos[child].setY(row);// row
            m_isPlaced[child] = true;
            //qDebug() << child << " child " << row << " row " << column + 1 << " column";
        }
    }

private:

    int m_moduleCount = 0;
    QVector<bool> m_isPlaced;// newIndex 是否被摆放
    QVector<QPoint> m_degree;// 出入度
    QVector<QPoint> m_relativePos;// 待计算的相对坐标
    QVector<QHash<int, int>> m_isPosOccupied;// 位置是否被占据
    QVector<double> m_weight;// 权重 出度和入度各占50%
    QVector<QList<int>> m_connectData;// 强连通分支间的连接关系

};
