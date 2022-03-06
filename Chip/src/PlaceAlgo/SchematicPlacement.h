//
// Created by Huangff on 2022/2/11.
//

#pragma once

#include "GetGraphSccs.h"
#include "Router.h"
#include "Placer.h"
#include "PlaceSccs.h"
#include "PlaceAScc.h"
#include "ComputeAbsolutePos.h"

#include <QList>
#include <QVector>
#include <QPoint>
#include <QDebug>
#include <QStack>
#include <algorithm>

struct ASccInfo {
    /// 单个强连通分支内部结点的所有相关信息

    QPoint sccSize;// 一个强连通分支包含结点所占据的大小
    QVector<QPoint> absolutePos;// 强连通分支内部结点的绝对位置
    QVector<QPoint> relativePos;// 强连通分支内部结点的相对位置
    QHash<int, int> sccIndexHash;// 根据原index找到现在的index   <key--int--before,value--int--after--now>
    QVector<QList<int>> graph;// 连接数据--邻接表
    QVector<QPoint> moduleSize;// 模块的大小
};


class PlaceSccs;

class PlaceAScc;

class SchematicPlacement {

public:
    SchematicPlacement(QVector<QList<int>> graphData, QVector<QPoint> moduleSize);

    ~SchematicPlacement() = default;

    QVector<QPoint> getModulePos() {
        return m_modulePos;
    }

    QVector<QPoint> getPortPos() {
        // 返回不属于module的port的位置信息
        return m_portPos;
    }

private:
    void computeScc() {
        GetGraphSccs tarjanAlgo(m_moduleConnectData);
        m_sccs = tarjanAlgo.getGraphAllScc();
    }

    void initSccsInfo() {
        // 初始化m_sccsConnectData和m_sccsInfo

        m_moduleSccIndex = QVector<int>(m_moduleConnectData.size());// 模块所在的sccIndex
        m_sccsConnectData = QVector<QList<QPoint >>(m_sccs.size() + 1);
        m_sccsInfo = QVector<ASccInfo>(m_sccs.size() + 1);

        QHash<int, int> sccHash;// 强连通分支间的hash
        for (int i = 0; i < m_sccs.size(); ++i) {
            sccHash.insert(m_sccs[i][0], i);// 原index 新index

            QVector<QPoint> moduleSize = QVector<QPoint>(m_sccs[i].size());
            QHash<int, int> sccIndexHash;// 根据原index找在强连通分支的新index
            for (int j = 0; j < m_sccs[i].size(); ++j) {
                // 原index = m_sccs[i][j] 在第i个强连通分支中
                m_moduleSccIndex[m_sccs[i][j]] = i;

                sccIndexHash.insert(m_sccs[i][j], j);// 原来的index 新index
                // 模块大小
                moduleSize[j] = m_size[m_sccs[i][j]];
            }

            m_sccsInfo[i + 1].sccSize = moduleSize[0];// 先初始化一个sccSize 后续会修改
            m_sccsInfo[i + 1].absolutePos = QVector<QPoint>(m_sccs[i].size());
            m_sccsInfo[i + 1].relativePos = QVector<QPoint>(m_sccs[i].size());
            m_sccsInfo[i + 1].sccIndexHash = sccIndexHash;
            // pass
            //qDebug() << sccIndexHash << "---QVector<ASccInfo>";
            m_sccsInfo[i + 1].graph = QVector<QList<int>>(m_sccs[i].size());
            m_sccsInfo[i + 1].moduleSize = moduleSize;
        }

        m_sccsInfo[0].sccSize = QPoint(0, 0);// 先初始化一个sccSize 后续会修改
        m_sccsInfo[0].absolutePos = QVector<QPoint>(m_sccs.size());
        m_sccsInfo[0].relativePos = QVector<QPoint>(m_sccs.size());
        m_sccsInfo[0].sccIndexHash = sccHash;
        //qDebug() << sccHash << "---***";
        m_sccsInfo[0].graph = QVector<QList<int>>(m_sccs.size());
        // 差graph

        // success
        for (int i = 0; i < m_moduleConnectData.size(); ++i) {
            for (int j = 0; j < m_moduleConnectData[i].size(); ++j) {
                int end = m_moduleConnectData[i][j];// endModuleIndex
                int sccIndex = m_moduleSccIndex[i];// 起点i所在的sccIndex
                int endSccIndex = m_moduleSccIndex[end];
                if (sccIndex == endSccIndex) {
                    /// 起点i 和 终点 m_moduleConnectData[i][j] 在同一个scc中 则考虑将该connect添加到m_sccConnectData中
                    //qDebug() << m_moduleSccIndex[i] << "-----";
                    m_sccsConnectData[sccIndex + 1].push_back(QPoint(i, end));
                    //m_sccsConnectData[]
                } else {
                    m_sccsConnectData[0].push_back(QPoint(m_sccs[sccIndex][0], m_sccs[endSccIndex][0]));
                }
            }
        }

        // pass
        // 根据原来的连接关系 计算新索引下强连通分支间、单个强连通分析的连接关系
        for (int i = 0; i < m_sccsConnectData.size(); ++i) {
            for (int j = 0; j < m_sccsConnectData[i].size(); ++j) {
                // 原 起点m_sccsConnectData[i][j].x() 终点m_sccsConnectData[i][j].y()
                int start = m_sccsInfo[i].sccIndexHash.value(m_sccsConnectData[i][j].x());
                int end = m_sccsInfo[i].sccIndexHash.value(m_sccsConnectData[i][j].y());
                //qDebug() << m_sccsInfo[i].sccIndexHash;
                //qDebug() << " before " << m_sccsConnectData[i][j].x() << m_sccsConnectData[i][j].y() << " after "
                //<< start << " " << end;
                m_sccsInfo[i].graph[start].push_back(end);
            }
        }
    }

    void preHandleData() {
        /// 数据的预处理
        //      1.计算强连通分支
        //      2.初始化m_sccsConnectData和m_sccsInfo

        computeScc();
        initSccsInfo();

    }

    void computeSccsRelativePosition() {
        /// 计算强连通分支间的相对位置
        // 存放在m_sccsInfo[0].relativePos中

        // connect数据
        //qDebug() << m_sccsConnectData[0];// 原连接数据
        qDebug() << m_sccsInfo[0].graph;// 新索引下的连接数据

        PlaceSccs placeSccs(m_sccsInfo[0].graph);
        // 保存相对位置数据
        m_sccsInfo[0].relativePos = placeSccs.getRelativePos();

    }

    void computeSccsAbsolutePosition() {
        /// 计算强连通分支间的绝对位置

        qDebug() << "\n\n------------------------------------------------------compute Sccs AbsolutePosition";

        // 每个强连通分支的大小
        QVector<QPoint> moduleSize = QVector<QPoint>(m_sccs.size());

        for (int i = 0; i < m_sccs.size(); ++i) {
            if (m_sccs[i].size() == 1) {
                // 为了计算相对位置 以5为基本大小单位
                moduleSize[i] = QPoint(5, 5);
            } else {
                // 一个最大强连通分支的大小
                moduleSize[i] = m_sccsInfo[i + 1].sccSize;

            }
        }

        // pass
        //qDebug() << moduleSize << " ModuleSize----Sccs";

        QVector<QList<int>> graph;//    m_sccsInfo[0].graph
        ComputeAbsolutePos absolutePos(m_sccsInfo[0].graph, moduleSize,
                                       m_sccsInfo[0].relativePos, m_grid, 0, 0);
        m_sccsInfo[0].absolutePos = absolutePos.getAbsolutePos();
        m_sccsInfo[0].sccSize = absolutePos.getSccBlockSize();

        // pass
        qDebug() << "\n\n  absolutePos  " << m_sccsInfo[0].absolutePos;
        qDebug() << "  sccSize  " << m_sccsInfo[0].sccSize;

        //qDebug() << m_sccsInfo[0].absolutePos.size() << " ";
        qDebug() << "------------------------------------------------------compute Sccs AbsolutePosition End\n\n";
    }

    void computeASccPosition() {
        /// 计算强连通分支内部的相对位置和绝对位置
        // pass
        for (int i = 0; i < m_sccs.size(); ++i) {
            if (m_sccs[i].size() > 1) {
                /// 该强连通分量不止一个结点 则考虑连通分支内的模块的位置的计算

                int index = i + 1;
                // 计算相对位置并保存
                PlaceAScc placeAScc(m_sccsInfo[index].graph);
                m_sccsInfo[index].relativePos = placeAScc.getRelativePos();
                qDebug() << m_sccsInfo[index].relativePos << "     relative Pos";

                int width = 0;
                int height = 0;
                QVector<QPoint> size = m_sccsInfo[index].relativePos;
                for (int j = 0; j < size.size(); ++j) {
                    width = width > size[j].x() ? width : size[j].x();
                    height = height > size[j].y() ? height : size[j].y();
                    size[j].setX(size[j].x() * 5);
                    size[j].setY(size[j].y() * 5);
                }
                //qDebug() << size;

                //qDebug() << width << " width  height" << height;
                // 计算绝对位置并保存
                m_sccsInfo[index].absolutePos = size;
                // 更新单个强连通分支的大小
                m_sccsInfo[index].sccSize = QPoint((width + 1) * 5, (height + 1) * 5);

/*                // 计算绝对位置并保存
                ComputeAbsolutePos absolutePos(m_sccsInfo[index].graph, size,
                                               m_sccsInfo[index].relativePos, m_grid, 0, 0);
                m_sccsInfo[index].absolutePos = absolutePos.getAbsolutePos();
                // 更新单个强连通分支的大小
                m_sccsInfo[index].sccSize = absolutePos.getSccBlockSize();*/

                qDebug() << index << "  absolutePos  " << m_sccsInfo[index].absolutePos;
                qDebug() << index << "  sccSize  " << m_sccsInfo[index].sccSize;
            }
        }
    }

    void computePortPos() {
        // todo add something 用于计算port的位置
    }

    void computeASccInnerPosition() {
        qDebug() << "------------------------------------------------------compute A SccInner Position";

        /// 计算每个强连通分支内部结点的绝对位置
        // 根据强连通分支间 以及 强连通分支内部的绝对位置来计算出每个module的最终位置
        for (int i = 0; i < m_sccs.size(); ++i) {
            for (int j = 0; j < m_sccs[i].size(); ++j) {
                // 模块index
                int index = m_sccs[i][j];// 原index
                int newIndex = m_sccsInfo[i + 1].sccIndexHash.value(index);
                // bug solved
                QPoint absolutePos = m_sccsInfo[i + 1].absolutePos[newIndex];
                // bug solved
                //qDebug() << m_sccsInfo[0].absolutePos.size() << " " << i;
                QPoint sccIPos = m_sccsInfo[0].absolutePos[i];
                m_modulePos[index] = QPoint(sccIPos.x() + absolutePos.x(), sccIPos.y() + absolutePos.y());
            }
        }
        qDebug() << m_modulePos << " ModulePos";
        qDebug() << "------------------------------------------------------compute A SccInner Position End\n\n";

    }

    void computePosition() {

        m_modulePos = QVector<QPoint>(m_moduleConnectData.size());

        /// 计算模块的绝对位置
        ///     Step1.计算强连通分支间的相对位置
        ///     Step2.计算每个强连通分支内部的相对位置和绝对位置
        ///     Step3.计算强连通分支间的绝对位置
        ///     Step4.计算单独port的绝对位置

        // Not Pass bug     Fix     Time:2022.0228
        computeSccsRelativePosition();// 计算强连通分支间的相对位置

        computeASccPosition();// 计算每个强连通分支内部的相对位置

        //printInfo();

        // bug 疑难杂症 Fix Time:2022.0228
        // 计算SccInfo 需要注意
        computeSccsAbsolutePosition();// 计算强连通分支间的绝对位置

        // 计算每个模块的相对位置
        computeASccInnerPosition();

        // 计算每个模块的绝对位置
        computeAllModulePos();

        // 调整模块的位置 使得模块间距小一些
        adjustModulePos();

        // todo add
        computePortPos();// 计算单独port的绝对位置

    }

    void printInfo() {
        qDebug() << "****************************************************";
        qDebug() << m_sccs << "  SCC";
        for (int i = 0; i < m_sccsInfo.size(); ++i) {
            qDebug() << "Index   " << i;
            qDebug() << m_sccsInfo[i].absolutePos << "  absolutePos";
            qDebug() << m_sccsInfo[i].relativePos << "  relativePos";
            qDebug() << m_sccsInfo[i].graph << "  graph";
            qDebug() << m_sccsInfo[i].sccIndexHash << "  sccIndexHash";
            qDebug() << m_sccsInfo[i].sccSize << "  sccSize";
            qDebug() << m_sccsInfo[i].moduleSize << "  moduleSize";
            qDebug() << "****************************************************";
        }
    }


    void adjustModulePos() {
        // 根据俄罗斯方块调整最终位置
    }

    void computeAllModulePos() {
        qDebug() << "compute All Module Pos------------------------------------";
        qDebug() << m_modulePos;
        // 相对位置的计算
        for (int i = 0; i < m_modulePos.size(); ++i) {
            QPoint point = m_modulePos[i];
            m_modulePos[i] = QPoint(point.x() / 5, point.y() / 5);
        }
        qDebug() << m_modulePos;

        ComputeAbsolutePos pos(m_moduleConnectData, m_size, m_modulePos, 1, 30, 50);
        m_modulePos = pos.getAbsolutePos();

    }


///private:

    /// 输入数据
    QVector<QList<int>> m_moduleConnectData;// 模块间的连接数据 不包括单独的port与模块的连接数据
    QVector<QList<int>> m_portConnectData;// 模块间的连接数据 不包括单独的port与模块的连接数据

    QVector<QPoint> m_size;// 所有模块的大小

    int m_grid = 1;// 网格大小

    /// 中间数据
    QList<QList<int>> m_sccs;// strong connected component    /// 所有强连通分量
    QVector<QList<QPoint>> m_sccsConnectData;// 强连通分支间的connect 强连通分支内部的connect
    QVector<ASccInfo> m_sccsInfo;// 强连通分支间的info 强连通分支内部的info
    QVector<int> m_moduleSccIndex;// 模块所在强连通分支的index

    /// 输出结果
    QVector<QPoint> m_modulePos;
    QVector<QPoint> m_portPos;

};
