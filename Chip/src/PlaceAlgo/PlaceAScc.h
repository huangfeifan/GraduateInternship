//
// Created by Huangff on 2022/1/26.
//

#pragma once

#include <QVector>
#include <QStack>
#include <QDebug>
#include <algorithm>

static double InWeight = 0.6;

class PlaceAScc {
    // 计算相对坐标

public:
    PlaceAScc(const QVector<int> oldIndexList, const QVector<QList<int>> allConnectData) {
        qDebug() << "________________________PlaceAScc________________________";
        /// 根据原结点序号 计算出强连通分支内部的connectData
        m_moduleCount = oldIndexList.size();
        for (int i = 0; i < m_moduleCount; ++i) {
            m_sccIndexHash.insert(oldIndexList[i], i);//  <oldIndex,newIndex>
        }
        /// 初始化所有变量
        preHandleData();
        /// 从原连接数据构造新的新连接数据 同时更新新结点度数
        for (int i = 0; i < m_moduleCount; ++i) {
            int oldIndex = oldIndexList[i];
            int newIndex = m_sccIndexHash.value(oldIndex);
            for (int j = 0; j < allConnectData[oldIndex].size(); ++j) {
                int oldEndIndex = allConnectData[oldIndex][j];
                if (m_sccIndexHash.contains(oldEndIndex)) {
                    int newEndIndex = m_sccIndexHash.value(oldEndIndex);
                    m_connectData[newIndex].push_back(newEndIndex);
                    // 更新出度
                    m_degree[newEndIndex].setY(m_degree[newEndIndex].y() + 1);
                    // 更新入度
                    m_degree[newIndex].setX(m_degree[newIndex].x() + 1);
                }
            }
        }
        qDebug() << m_connectData << "  NewConnectData";
        qDebug() << m_degree << "  NewDegree";
        /// 权重的计算
        for (int i = 0; i < m_moduleCount; ++i) {
            /// 出度越大越靠左 入度越大越靠右
            m_weight[i] = (InWeight + 1) * m_degree[i].x() - InWeight * m_degree[i].y();
        }
        qDebug() << m_weight << "  Weight";
        /// 如何摆放呢





        // 简单调整相对位置
        simpleAdjust(m_relativePosition);
        qDebug() << "________________________PlaceAScc________________________End";
    };

    QVector<QPoint> getSccRelativePosition() {
        return m_relativePosition;
    };

private:

    int findMaxDegreeIndex() {
        return -1;
    };

    void simpleAdjust(QVector<QPoint> &relativePosition) {
        // 调整相对位置 相对位置从(0,0)开始
        int minRow = relativePosition.size();
        int minColumn = relativePosition.size();
        for (int i = 0; i < relativePosition.size(); ++i) {
            minRow = relativePosition[i].y() > minRow ? minRow : relativePosition[i].y();
            minColumn = relativePosition[i].x() > minColumn ? minColumn : relativePosition[i].x();
        }
        for (int i = 0; i < relativePosition.size(); ++i) {
            relativePosition[i].setX(relativePosition[i].x() - minColumn);
            relativePosition[i].setY(relativePosition[i].y() - minRow);
        }
    }


    void preHandleData() {
        // 数据的预处理
        m_isPlaced = QVector<bool>(m_moduleCount);
        m_degree = QVector<QPoint>(m_moduleCount);
        m_relativePosition = QVector<QPoint>(m_moduleCount);
        m_isPositionOccupy = QVector<QHash<int, int>>(m_moduleCount);
        m_weight = QVector<double>(m_moduleCount);
        m_connectData = QVector<QList<int>>(m_moduleCount);
    };

private:
    QHash<int, int> m_sccIndexHash;// 根据原index找到现在的index
    int m_moduleCount = 0;
    QVector<bool> m_isPlaced;// newIndex 是否被摆放
    QVector<QPoint> m_degree;// 出入度
    QVector<QPoint> m_relativePosition;// 待计算的相对坐标
    QVector<QHash<int, int>> m_isPositionOccupy;// 位置是否被占据
    QVector<double> m_weight;// 权重 出度和入度各占50%
    QVector<QList<int>> m_connectData;// 强连通分支内的连接关系

    // todo delete ???
    QVector<int> m_indexParent;// 摆放的index的父节点的

};
