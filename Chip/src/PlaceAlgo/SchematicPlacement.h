//
// Created by Huangff on 2022/2/11.
//

#pragma once

#include "Data.h"
#include "Router.h"
//#include "Placer.h"
#include "PlaceSccs.h"
#include "PlaceAScc.h"
#include "GetGraphSccs.h"
#include "GetTopologySort.h"
//#include "ImprovedTarjanAlgo.h"
#include "ComputeAbsolutePos.h"

#include "PlaceAlgo/MyStruct.h"

#include <QList>
#include <QVector>
#include <QPoint>
#include <QStack>
#include <QDebug>
#include <algorithm>
#include <algorithm>

class PlaceSccs;

class PlaceAScc;

class SchematicPlacement {

public:
    SchematicPlacement(QVector<QList<int>> graphData, QVector<QPoint> moduleSize);

    // 给连接数据 模块大小
    SchematicPlacement(QList<ConnectData> connectData, QVector<QPoint> moduleSize);

    // 给连接数据 模块大小需要按照规则计算
    SchematicPlacement(QList<ConnectData> connectData);

    ~SchematicPlacement() = default;

    QVector<QPoint> getModulePos();

    QVector<QPoint> getModuleRelativePos();

    QVector<QPoint> getLeftPortPos();

    QVector<QPoint> getRightPortPos();

    QVector<QVector<int>> getModulePortPos();

//private: // Todo open comment
    void computeScc() ;

    void computeModuleConnectData();

    void initSccsInfo();

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
        //qDebug() << m_sccsInfo[0].graph;// 新索引下的连接数据
        PlaceSccs placeSccs(m_sccsInfo[0].graph);
        // 保存相对位置数据
        m_sccsInfo[0].relativePos = placeSccs.getRelativePos();

    }

    void computeSccsAbsolutePosition();

    void computeASccPosition();

    void computePortPos() {
        // todo add something 用于计算port的位置
    }

    void computeASccInnerPosition();

    void computePosition();

    void printInfo();

    void adjustModulePos() {
        // 根据俄罗斯方块调整最终位置
    }

    void adjustRelativePos(QVector<QPoint> &relativePos);

    void computeAllModulePos();

    void computeModuleSize();

    void computeConnectData() {
        /// 根据
    }

///private:

    /// 输入数据 QList<ConnectData> data;
    QList<ConnectData> m_connectData;// 输入的模块间的连接数据
    QVector<QPoint> m_size;// 所有模块的大小
    int m_moduleCount = 0;// 模块个数

    /// 修改
    // Todo modify
    QVector<QList<int>> m_moduleConnectData;// 模块间的连接数据 不包括单独的port与模块的连接数据
    // Todo delete
    QVector<QList<int>> m_portConnectData;// 模块间的连接数据 不包括单独的port与模块的连接数据
    int m_grid = 1;// 网格大小

    /// 中间数据
    QList<QList<int>> m_sccs;// strong connected component    /// 所有强连通分量
    QVector<QList<QPoint>> m_sccsConnectData;// 强连通分支间的connect 强连通分支内部的connect
    QVector<ASccInfo> m_sccsInfo;// 强连通分支间的info 强连通分支内部的info
    QVector<int> m_moduleSccIndex;// 模块所在强连通分支的index

    /// 输出结果
    QVector<QPoint> m_modulePos;// 模块的绝对位置
    QVector<QPoint> m_moduleRelativePos;// 模块的相对位置
    QVector<QPoint> m_rightPortPos;// 单独的输出port
    QVector<QPoint> m_leftPortPos;// 单独的输入port
    /// port初始放在module的两侧 输入为左侧 输出为右侧 所以只需要高度也就是y坐标(相对坐标)
    QVector<QVector<int>> m_modulePortPos;// 模块的端口位置

};
