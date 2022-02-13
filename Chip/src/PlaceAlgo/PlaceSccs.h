//
// Created by Huangff on 2022/2/13.
//

#pragma once

#include <QDebug>

class PlaceSccs {

public:
    /// 所有强连通分量的摆放
    PlaceSccs(const QVector<int> &oldIndexList, const QList<QPoint> &connectData) {
        qDebug() << "________________________PlaceSccs________________________";

        /// 初始化所有变量
        preHandleData(oldIndexList, connectData);

        /// 计算相对位置
        computePos();

        /// 简单调整相对位置
        PlaceSccs::simpleAdjust(m_relativePos);

        qDebug() << m_relativePos << " RelativePos";
        qDebug() << "________________________PlaceSccs________________________";
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

    QVector<QPoint> getSccRelativePos() {
        return m_relativePos;
    };

    QHash<int, int> getSccIndexHash() {
        return m_sccIndexHash;
    }

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

        // 是否增加排序呢  Todo Add
        for (int i = 0; i < tSort.size(); ++i) {
            for (int j = 0; j < tSort[i].size(); ++j) {
                placeIndexAndChild(tSort[i][j], tSort.size() - 1, i, orderList);
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
            qDebug() << index << " index " << row << " row " << column << " column";
        }

        /// placeChild
        QList<int> childList;
        for (int i = 0; i < m_connectData[index].size(); ++i) {
            int child = m_connectData[index][i];
            if (!m_isPlaced[child] && (orderList[index] + 1) == orderList[child]) {
                childList.push_back(child);
            }
        }

        for (int i = 0; i < childList.size(); ++i) {
            int child = childList[i];
            //row = row - childList.size()/2;
            while (m_isPosOccupied[column + 1].contains(row)) {
                row++;
            }
            m_isPosOccupied[column + 1].insert(row, row);
            m_relativePos[child].setX(column + 1);// column
            m_relativePos[child].setY(row);// row
            m_isPlaced[child] = true;
            qDebug() << child << " child " << row << " row " << column + 1 << " column";
        }
    }

    void preHandleData(const QVector<int> &oldIndexList, const QList<QPoint> &connectData) {
        /// 模块个数
        m_moduleCount = oldIndexList.size();
        for (int i = 0; i < m_moduleCount; ++i) {
            m_sccIndexHash.insert(oldIndexList[i], i);//  <oldIndex,newIndex>
        }

        /// 初始化数组
        m_isPlaced = QVector<bool>(m_moduleCount);
        m_degree = QVector<QPoint>(m_moduleCount);
        m_relativePos = QVector<QPoint>(m_moduleCount);
        m_isPosOccupied = QVector<QHash<int, int>>(m_moduleCount);
        m_weight = QVector<double>(m_moduleCount);

        /// 计算connect数据
        //  根据原结点序号 计算出强连通分支内部的connectData
        m_connectData = QVector<QList<int>>(m_moduleCount);
        for (size_t i = 0; i < connectData.size(); ++i) {
            int start = connectData[i].x();
            int end = connectData[i].y();
            int newStart = m_sccIndexHash.value(start);
            int newEnd = m_sccIndexHash.value(end);
            m_connectData[newStart].push_back(newEnd);
        }
        //qDebug() << connectData << " Old---ConnectInfo";
        //qDebug() << m_connectData << " ConnectData";
    }

private:

    int m_moduleCount = 0;
    QHash<int, int> m_sccIndexHash;// 根据原index找到现在的index

    QVector<bool> m_isPlaced;// newIndex 是否被摆放
    QVector<QPoint> m_degree;// 出入度
    QVector<QPoint> m_relativePos;// 待计算的相对坐标
    QVector<QHash<int, int>> m_isPosOccupied;// 位置是否被占据
    QVector<double> m_weight;// 权重 出度和入度各占50%
    QVector<QList<int>> m_connectData;// 强连通分支间的连接关系

};