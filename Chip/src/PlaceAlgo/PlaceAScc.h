﻿//
// Created by Huangff on 2022/2/13.
//

#pragma once

#include "PlaceSccs.h"


class PlaceAScc {
    /// 一个强连通分量的摆放  计算一个强连通分量内部结点的相对坐标

public:
    PlaceAScc(const QVector<QList<int>> &connectData) {

        qDebug() << "\n****************************PlaceAScc****************************";

        /// 初始化所有变量
        preHandleData(connectData);

        /// 计算相对位置
        computePos();

        // 简单调整相对位置
        PlaceSccs::simpleAdjust(m_relativePos);
        qDebug() << m_relativePos << " after Simple adjust:  AScc_relativePos";

        qDebug() << "****************************PlaceAScc****************************End\n";
    };

    QVector<QPoint> getRelativePos() {
        return m_relativePos;
    };

private:

    void computePos() {// Todo Modify     <==>    Placement::placeAScc()
        QVector<int> indexParentIndex(m_moduleCount);// 父节点所在的行数

        // 找出度数最大的
        int degree = m_connectData[0].size();// 结点0的度数
        int maxIndex = 0;// 假设结点0度数最大
        for (int i = 1; i < m_connectData.size(); ++i) {
            if (m_connectData[i].size() > degree) {
                // 较大度数出现 更新最大度数的结点
                maxIndex = i;
                degree = m_connectData[i].size();
            }
        }

        // 思想： 先摆放度数最大的点 然后摆放其所有子节点 然后摆放所有子节点的子节点
        int row = m_connectData.size();    // 摆放权重(度数)最大的
        int column = 0;
        while (m_isPosOccupied[column].key(row)) {
            row++;
        }
        m_isPosOccupied[column].insert(row, row);// 存储index占据的位置
        m_relativePos[maxIndex].setX(column++);// 列
        m_relativePos[maxIndex].setY(row);// 行
        m_isPlaced[maxIndex] = true;// 修改状态

        qDebug() << m_relativePos << "   relativePosition";

        QStack<int> currentStack, nextStack;
        // 修改最大度数结点的子节点的父节点行号 indexParentIndex 用于描述父节点所在行数
        for (int i = 0; i < m_connectData[maxIndex].size(); ++i) {
            int child = m_connectData[maxIndex][i];// 新moduleIndex
            qDebug() << " child Index " << child;
            indexParentIndex[child] = maxIndex;
            // child没有被摆放 则将其添加到stack中 便于后续摆放
            if (!m_isPlaced[child]) {
                currentStack.push(child);
            }
        }
        qDebug() << currentStack << "   CurrentStack";

        qDebug() << row << " FRow--";
        while (!currentStack.isEmpty()) {
            // 摆放topIndex
            int topIndex = currentStack.pop();
            int topParentIndex = indexParentIndex[topIndex];
            qDebug() << topIndex << "   " << topParentIndex << " " << m_connectData[topParentIndex].size();
            row = m_relativePos[topParentIndex].y() - m_connectData[topParentIndex].size() * 0.382;

            while (m_isPosOccupied[column].contains(row)) {
                row++;
            }
            qDebug() << row << " RRRow";
            m_isPosOccupied[column].insert(row, row);
            m_relativePos[topIndex].setX(column);// 列
            m_relativePos[topIndex].setY(row);// 行
            m_isPlaced[topIndex] = true;

            qDebug() << topIndex << " topIndex";
            for (int i = 0; i < m_connectData[topIndex].size(); ++i) {
                // update nextStack;
                int child = m_connectData[topIndex][i];
                // 将未摆放的子节点入栈
                if (!m_isPlaced[child]) {
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
        qDebug() << m_relativePos << " before Simple adjust:  AScc_relativePos";
    }

    int findMaxDegreeIndex() {
        return -1;
    };

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

        // 从原连接数据构造新的新连接数据 同时更新新结点度数
        /// 计算出入度和权重 Todo add
        for (int i = 0; i < m_connectData.size(); ++i) {
            // 更新出度
            m_degree[i].setX(m_connectData[i].size());
            for (int j = 0; j < m_connectData[i].size(); ++j) {
                // 起点i 终点end
                int end = m_connectData[i][j];
                // x() 出度   y()入度
                int in = m_degree[end].y();
                // 更新入度
                m_degree[end].setY(in + 1);
            }
        }

        qDebug() << m_connectData << "  NewConnectData";
        qDebug() << m_degree << "  NewDegree";
        // 权重的计算  Todo consider 权重的聚类
        for (int i = 0; i < m_moduleCount; ++i) {
            // 出度越大越靠左 入度越大越靠右
            m_weight[i] = (m_degree[i].x() - m_degree[i].y()) * IN_OUT_WEIGHT;
        }
        qDebug() << m_weight << "  Weight";
    };

private:
    int m_moduleCount = 0;
    QVector<bool> m_isPlaced;// newIndex 是否被摆放
    QVector<QPoint> m_degree;// 出入度
    QVector<QPoint> m_relativePos;// 待计算的相对坐标
    QVector<QHash<int, int>> m_isPosOccupied;// 位置是否被占据
    QVector<double> m_weight;// 权重 出度和入度各占50%
    QVector<QList<int>> m_connectData;// 强连通分支内的连接关系

    // todo delete ???
    QVector<int> m_indexParent;// 摆放的index的父节点的

    const double IN_OUT_WEIGHT = 1.0;
};