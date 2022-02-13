//
// Created by Huangff on 2022/2/11.
//

#pragma once

#include "GetGraphSccs.h"
#include "Placer.h"
#include "PlaceSccs.h"
#include "PlaceAScc.h"

#include <QList>
#include <QVector>
#include <QPoint>
#include <QDebug>
#include <QStack>
#include <algorithm>

struct ASccInfo {
    /// 单个强连通分支内部结点的所有相关信息

    QPoint size;// 一个强连通分支包含结点所占据的大小
    QVector<QPoint> absolutePos;// 强连通分支内部结点的绝对位置
    QVector<QPoint> relativePos;// 强连通分支内部结点的相对位置

    QHash<int, int> sccIndexHash;// 根据原index找到现在的index   <key--int--before,value--int--after--now>
};


class PlaceSccs;

class PlaceAScc;

class SchematicPlacement {

public:
    SchematicPlacement(QVector<QList<int>> graphData, QVector<QPoint> moduleSize);

    ~SchematicPlacement();

public:

private:
    void computeScc() {
        GetGraphSccs tarjanAlgo(m_moduleConnectData);
        m_sccs = tarjanAlgo.getGraphAllScc();
    }

    void computeSccConnectData() {
        QVector<int> moduleSccIndex = QVector<int>(m_moduleConnectData.size());// 模块所在的sccIndex
        m_sccsConnectData = QVector<QList<QPoint>>(m_sccs.size() + 1);
        for (size_t i = 0; i < m_sccs.size(); ++i) {
            for (size_t j = 0; j < m_sccs[i].size(); ++j) {
                moduleSccIndex[m_sccs[i][j]] = i;
            }
        }

        for (size_t i = 0; i < m_moduleConnectData.size(); ++i) {
            for (size_t j = 0; j < m_moduleConnectData[i].size(); ++j) {
                int end = m_moduleConnectData[i][j];// endModuleIndex
                int sccIndex = moduleSccIndex[i];// 起点i所在的sccIndex
                int endSccIndex = moduleSccIndex[end];
                if (sccIndex == endSccIndex) {
                    /// 起点i 和 终点 m_moduleConnectData[i][j] 在同一个scc中 则考虑将该connect添加到m_sccConnectData中
                    //qDebug() << moduleSccIndex[i] << "-----";
                    m_sccsConnectData[sccIndex + 1].push_back(QPoint(i, end));
                    //m_sccsConnectData[]
                } else {
                    m_sccsConnectData[0].push_back(QPoint(m_sccs[sccIndex][0], m_sccs[endSccIndex][0]));
                }
            }
        }

        //qDebug() << m_sccsConnectData<<"***************************";

    }

    void preHandleData() {
        /// 数据的预处理
        //      1.计算强连通分支
        //      2.计算connectData

        computeScc();
        computeSccConnectData();

    }

    void computeSccsRelativePosition() {
        /// 计算强连通分支间的相对位置
        ///  Step1.连接数据
        ///  Step2.拓扑排序
        ///  Step3.计算相对位置 存放在m_sccsInfo[0].absolutePos中

        // connect数据
        //qDebug() << m_sccsConnectData[0];
        QVector<int> oldIndexList = QVector<int>(m_sccs.size());
        for (size_t i = 0; i < m_sccs.size(); ++i) {
            oldIndexList[i] = m_sccs[i][0];
        }

        PlaceSccs placeSccs(oldIndexList, m_sccsConnectData[0]);
        m_sccsInfo[0].sccIndexHash = placeSccs.getSccIndexHash();
        m_sccsInfo[0].relativePos = placeSccs.getSccRelativePos();

    }

    void computeSccsAbsolutePosition() {
        /// 计算强连通分支间的绝对位置
    }

    void computeASccPosition() {
        /// 计算强连通分支内部的相对位置和绝对位置
    }

    void computePortPos() {

    }

    void computePosition() {

        m_modulePos = QVector<QPoint>(m_moduleConnectData.size());

        /// 计算模块的绝对位置
        ///     Step1.计算强连通分支间的相对位置
        ///     Step2.计算每个强连通分支内部的相对位置和绝对位置
        ///     Step3.计算强连通分支间的绝对位置
        ///     Step4.计算单独port的绝对位置

        computeSccsRelativePosition();
        computeASccPosition();
        computeSccsAbsolutePosition();

        // todo add
        computePortPos();

    }

private:


    /// 输入数据
    QVector<QList<int>> m_moduleConnectData;// 模块间的连接数据 不包括单独的port与模块的连接数据
    QVector<QList<int>> m_portConnectData;// 模块间的连接数据 不包括单独的port与模块的连接数据

    QVector<QPoint> m_size;// 所有模块的大小

    int m_grid = 10;// 网格大小

    /// 中间数据
    QList<QList<int>> m_sccs;// strong connected component    /// 所有强连通分量
    QVector<QList<QPoint>> m_sccsConnectData;// 强连通分支间的connect 强连通分支内部的connect
    QVector<ASccInfo> m_sccsInfo;// 强连通分支间的info 强连通分支内部的info

    /// 输出结果
    QVector<QPoint> m_modulePos;
    QVector<QPoint> m_portPos;

};
