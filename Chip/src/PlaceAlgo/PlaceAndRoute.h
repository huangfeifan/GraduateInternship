//
// Created by Huangff on 2022/3/23.
//

#pragma once

#include <QPoint>
#include <QVector>

#include "PlaceAlgo/MyStruct.h"
#include "PlaceAlgo/Placement.h"

class PlaceAndRoute {

public:

    PlaceAndRoute() = default;

    PlaceAndRoute(QList<ConnectData> connectData, int leftPortNum,
                  int rightPortNum, QVector<QVector<int>> modulePortInfo);

    ~PlaceAndRoute() = default;

    // convert data to fit interface
    void dataConvert();

    // compute placing result
    void placing();

    // compute routing result
    void routing();

    QList<QPoint> getARoutingResult(const Pair &src, const Pair &dest);

    // get module pos
    QVector<QPoint> getModulePos();

    // get module size
    QVector<QPoint> getModuleSize();

    // get module port pos
    QVector<QVector<QPoint>> getPortPos();

    // get alone left input port pos
    QVector<QPoint> getLeftPortPos();

    // get alone right output port pos
    QVector<QPoint> getRightPortPos();

    // get all paths
    QVector<QList<QPoint>> getPaths();

    struct ModuleInfos {
        QPoint pos;//
        QPoint size;
        QVector<QPoint> portPos;
    };

private:
    QList<ConnectData> m_connectData;// 连接数据
    QVector<QPoint> m_modulePos;// 模块的绝对位置
    QVector<QPoint> m_moduleSize;// 模块的大小
    QVector<QPoint> m_rightPortPos;// 单独的输出port
    QVector<QPoint> m_leftPortPos;// 单独的输入port
    QVector<QVector<QPoint>> m_modulePortPos;// 模块的端口位置
    QVector<QList<QPoint>> m_paths;// 所有路径
    QVector<QVector<int>> m_modulePortInfo;// 模块的port信息

    int m_leftPortNum;// 左侧输入port
    int m_rightPortNum;// 右侧输出port
    int m_graphRowCount;
    int m_graphColumnCount;
};
