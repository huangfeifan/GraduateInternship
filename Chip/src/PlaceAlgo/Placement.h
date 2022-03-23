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

class Placement {

public:
    Placement(QVector<QList<int>> graphData, QVector<ModuleInfo> moduleInfo);

    // 给连接数据 模块信息
    Placement(QList<ConnectData> connectData, QVector<ModuleInfo> moduleInfo, int leftPortNum, int rightPortNum);

    // 给连接数据 根据出入度计算模块大小
    Placement(QVector<QPoint> moduleDegree, QList<ConnectData> connectData);

    Placement(QList<ConnectData> connectData);

    ~Placement() = default;

    QVector<QPoint> getModulePos();

    QVector<QPoint> getModuleRelativePos();

    QVector<QPoint> getLeftPortPos();

    QVector<QPoint> getRightPortPos();

    QVector<QVector<QPoint>> getModulePortPos();

private: // Todo open comment

    void preHandleData();

    void computeScc();

    void initSccsInfo();

    void computePosition();

    void computeSccsRelativePosition();

    void computeASccPosition();

    void computeSccsAbsolutePosition();

    void computeASccInnerPosition();

    void computeAllModulePos();

    void adjustModulePos();

    void computePortPos();

    void computeModulePortPos();

    void computeLeftPortPos();

    void computeRightPortPos();


    void computeModuleConnectData();

    void adjustRelativePos(QVector<QPoint> &relativePos);

    void printInfo();

    void computeModuleSize();

    void initPortRelatedInfo();

private:

    /// 输入数据 QList<ConnectData> data;
    QList<ConnectData> m_connectData;// 连接数据
    QVector<ModuleInfo> m_moduleInfo;// 所有模块的信息
    int m_leftPortNum = 0;// 左侧输入port个数
    int m_rightPortNum = 0;// 右侧输出port个数
    int m_moduleCount = 0;// 模块个数

    /// 中间数据 邻接表的形式存储
    QVector<QList<int>> m_moduleConnectData;// 模块间的连接数据 不包括单独的port与模块的连接数据

    QVector<QVector<PortInfo>> m_modulePort;
    QVector<PortInfo> m_leftInputPort;
    QVector<PortInfo> m_rightOutputPort;
    QList<ConnectData> m_leftPortConnectData;// 左侧输入port的连接数据
    QList<ConnectData> m_rightPortConnectData;// 右侧输出port的连接数据

    int m_grid = 10;// 网格大小

    QList<QList<int>> m_sccs;// strong connected component    /// 所有强连通分量
    QVector<QList<QPoint>> m_sccsConnectData;// 强连通分支间的connect 强连通分支内部的connect
    QVector<ASccInfo> m_sccsInfo;// 强连通分支间的info 强连通分支内部的info
    QVector<int> m_moduleSccIndex;// 模块所在强连通分支的index

    /// 输出结果
    QVector<QPoint> m_modulePos;// 模块的绝对位置
    QVector<QPoint> m_moduleRelativePos;// 模块的相对位置
    QVector<QPoint> m_rightPortPos;// 单独的输出port
    QVector<QPoint> m_leftPortPos;// 单独的输入port
    QVector<QVector<QPoint>> m_modulePortPos;// 模块的端口位置

};