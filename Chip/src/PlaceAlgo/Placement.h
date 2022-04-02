//
// Created by Huangff on 2022/2/11.
//

#pragma once

//#include "MyWidget/Data.h"
#include "Router.h"
#include "PlaceSccs.h"
#include "PlaceAScc.h"
#include "GetGraphSccs.h"
#include "GetTopologySort.h"
#include "ComputeAbsolutePos.h"

#include "PlaceAlgo/MyStruct.h"
#include "PlaceAlgo/FakeData.h"

#include <QList>
#include <QVector>
#include <QPoint>
#include <QStack>
#include <QDebug>
#include <algorithm>

class PlaceSccs;

class PlaceAScc;

class Placement {

public:
    // 给连接数据 模块信息
    Placement(QList<ConnectData> connectData, const QVector<QVector<int>> &modulePortInfo, int leftPortNum,
              int rightPortNum);

    Placement(QVector<QList<int>> graphData, QVector<QVector<int>> modulePortInfo);

    // 给连接数据
    Placement(QList<ConnectData> connectData);

    Placement(QList<ConnectData> connectData, QVector<QPoint> moduleSize);

    ~Placement() = default;

    QVector<QPoint> getModulePos();    // 获取模块的位置

    QVector<QPoint> getModuleRelativePos();    // 获取模块的相对位置

    QVector<QPoint> getLeftInputPortPos();    // 获取左侧输入port的位置

    QVector<QPoint> getRightOutputPortPos();    // 获取右侧输出port的位置

    QVector<QVector<QPoint>> getModulePortPos();    // 获取模块的port的位置

    QVector<QVector<QPoint>> getModulePortDirectionAndOrder();

    QVector<QPoint> getModuleSize();    // 获取模块的大小

    int getSchematicRowCount();         // 版图行数 高度除以网格大小

    int getSchematicColumnCount();    // 版图列数 宽度除以网格大小

    QVector<int> getModuleLeftPortNum();    // 模块i左侧port个数

    QVector<int> getModuleRightPortNum();    // 模块i右左侧port个数

    QVector<int> getModuleTopPortNum();    // 模块i上方port个数

    QVector<int> getModuleBottomPortNum();    // 模块下方port个数

private:

    void preHandleData();    // 数据预处理

    void computeScc();    // 计算scc

    void initSccsInfo();    // 计算scc的信息

    void computePosition();    // 计算模块和端口的位置

    void computeSccsRelativePosition();    // 计算sccs的相对位置

    void computeASccPosition();    // 计算sccInner的相对位置

    void computeSccsAbsolutePosition();    // 计算sccs的绝对位置

    void computeASccInnerPosition();    // 计算sccInner的绝对位置

    void computeAllModulePos();    // 调整相对位置 计算所有模块的绝对位置

    void adjustModulePos();

    void computePortPos();    // 用于计算所有port的位置

    void computeModulePortPos();    // 计算模块上port的位置

    void computeLeftPortPos();    // 计算左侧输入port的位置

    void computeRightPortPos();    // 计算右侧输出port的位置

    void computeModuleConnectData();    // 计算邻接表

    void adjustRelativePos(QVector<QPoint> &relativePos);    // 微调布局

    void printInfo();

    void computeModuleSize();

    void initVector();

    void adjustAllPos();

    void computeXPos();

    void getDistinctRelativePos();

private:

    /// 输入数据 QList<ConnectData> data;
    QList<ConnectData> m_connectData;// 连接数据
    int m_leftPortNum = 0;// 左侧输入port个数
    int m_rightPortNum = 0;// 右侧输出port个数
    int m_moduleCount = 0;// 模块个数
    QVector<QPoint> m_moduleSize;// 模块大小
    QVector<QVector<int>> m_modulePortInfo;// 模块的port的输入输出

    /// 中间数据 邻接表的形式存储
    QVector<QList<int>> m_moduleConnectData;// 模块间的连接数据 不包括单独的port与模块的连接数据
    QVector<QVector<PortInfo>> m_modulePort;// 模块的port数据
    QVector<PortInfo> m_leftInputPort;// 左侧输入port的数据
    QVector<PortInfo> m_rightOutputPort;// 右侧输出port的数据
    QList<ConnectData> m_leftPortConnectData;// 左侧输入port的连接数据
    QList<ConnectData> m_rightPortConnectData;// 右侧输出port的连接数据

    int m_leftPortXGap = 20 * GRID;// 左侧输入port与第一列模块的间距
    int m_rightPortXGap = 20 * GRID;// 右侧输出port与最后一列模块的间距
    int m_maxY = 0;// 没有连接关系的port的摆放 遵循从下往上 // 所有模块的竖直方向最大值
    int m_minY = 0;// 所有模块的竖直方向的最小值

    QList<QList<int>> m_sccs;// strong connected components
    QVector<QList<QPoint>> m_sccsConnectData;// sccs和sccInner的connectData
    QVector<ASccInfo> m_sccsInfo;// sccs和sccInner的info
    QVector<int> m_moduleSccIndex;// 模块在第几个scc中

    /// 输出结果
    QVector<QPoint> m_moduleRelativePos;// 模块的相对位置
    QVector<QPoint> m_modulePos;// 模块的绝对位置
    QVector<QPoint> m_rightPortPos;// 单独的输出port
    QVector<QPoint> m_leftPortPos;// 单独的输入port
    QVector<QVector<QPoint>> m_modulePortPos;// 模块的端口位置
    QVector<QVector<QPoint>> m_moduleDirectionAndOrder;// 模块port的方向和排序编号
    QVector<int> m_moduleLeftPortNum;// 模块i左侧port个数
    QVector<int> m_moduleRightPortNum;
    QVector<int> m_moduleTopPortNum;
    QVector<int> m_moduleBottomPortNum;

    int m_graphRowCount = 0;// 版图用网格剖分后的行数
    int m_graphColumnCount = 0;// 版图用网格剖分后的列数

};