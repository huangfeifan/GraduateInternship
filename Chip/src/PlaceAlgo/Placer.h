//
// Created by Huangff on 2021/12/15.
//

#pragma once

#include <QVector>
#include <QStack>
#include <QDebug>
#include <algorithm>

#include "Data.h"
#include "GetGraphSccs.h"
#include "GetTopologySort.h"
#include "ImprovedTarjanAlgo.h"

struct ModuleSize {
    int width = 0;
    int height = 0;
};

struct IndexDegree {
    int index;
    int degree;
};

class Placement {

public :
    // place scc
    QHash<int, int> m_moduleSccIndex;// module在第几个scc中
    int m_moduleCountScc = 0;
    QVector<bool> m_isPlacedScc;// 模块是否被摆放  todo delete 可以删除
    QVector<QHash<int, int>> m_isOccupyScc;// 位置被占据
    QVector<QList<int>> m_connectionScc;
    QVector<ModuleSize> m_moduleDegreeScc;// 记录模块的度数  width 出度 height 入度
    QVector<QPoint> m_relativePositionScc;// 改进的行列定位法坐标 相对坐标的计算


    QStringList m_nameListScc;
    // place a strong connect component
    QVector<QList<int>> sccConnect;
    QVector<bool> sccPlaced;
    QVector<QHash<int, int>> isOccupy;
    QVector<ModuleSize> moduleDegree;
    QVector<QPoint> relativePosition;
    QStringList sccList;
    QHash<int, int> sccIndexHash;// 连通分支内部结点的下标<index,innerIndex>
    QHash<int, int> sccHash;// 根据原index找到现index
    int sccIndex = 0;// 这是第几个强连通分支 用于调整强连通分支内部的摆放 todo modify

    void placeIndexAndChild(int index, int row, int column, QList<int> orderList);
    //void simpleAdjust(QVector<QPoint> &relativePosition);

public:

    int m_minWidth = 20;
    int m_minHeight = 20;
    int width_gap = 20;
    int height_gap = 20;
    bool m_upDown = false;
    QStringList m_nameList;

    QVector<QPoint> getRelativePosition() {
        return m_relativePosition;
    };

    QVector<QPoint> getRealPosition() {
        return m_realPosition;
    };

    QVector<ModuleSize> getModuleSize() {
        return m_moduleSize;
    };

    QVector<ModuleSize> getModuleDegree() {
        return m_moduleDegree;
    };

    Placement();

private:
    void computeModuleSize();
    QList<int> computeTraversalOrder();
    QList<int> computeTraversalOrder1();
    void breadthPlacement();
    void placeChildStack(QList<int> childList, int row, int column);
    void simplePlace();
    void deepPlacement();
    void placeChildRecursion(int index, int row, int column);
    void placeParent(int index, int row, int column);
    void init();// 初始化所有数据
    void initScc();// 初始化所有scc
    void placeAScc();// 计算一个强连通分支的摆放
    void sortConnectionData();
    void sortIndexList(QList<IndexDegree> &list);

private:
    QList<QList<int>> m_sccs;// strong connected component
    QVector<QList<int>> m_parent;//
    QVector<QPoint> m_realPosition;// 每个模块实际的坐标
    QVector<ModuleSize> m_moduleSize;// 记录模块的大小
    QVector<bool> m_isPlaced;// 模块是否被摆放

    int m_moduleCount = 0;
    QVector<QHash<int, int>> m_isOccupy;// 位置被占据
    QVector<QList<int>> m_connection;
    QVector<ModuleSize> m_moduleDegree;// 记录模块的度数  width 出度 height 入度
    QVector<QPoint> m_relativePosition;// 改进的行列定位法坐标 相对坐标的计算

};
