//
// Created by Huangff on 2022/4/7.
//

#pragma once

#include <QWidget>
#include <QPainter>
#include <QDebug>
#include <QVector>

#include "PlaceAlgo/ComputeAbsolutePos.h"
#include "PlaceAlgo/Placement.h"
#include "PlaceAlgo/MyStruct.h"
#include "PlaceAlgo/Router.h"
#include "PlaceAlgo/FakeData.h"
#include "PlaceAlgo/PlaceAndRoute.h"
#include "PlaceAlgo/RandomData.h"

class LineSearchPaint : public QWidget {
Q_OBJECT

public:
    LineSearchPaint() {
        initData();
        setFixedSize(1200, 1000);
    }

    ~LineSearchPaint();

private:

    void initData() {
        // 初始化数据
        RandomData r(10, 5, 4, 50);// 10个模块
        m_placementAndRoute = PlaceAndRoute(Fake_ConnectData, LEFT_PORT_NUM, RIGHT_PORT_NUM, Fake_ModulePortInfo);
        m_connectData = Fake_ConnectData;

        m_connectData = r.m_randomConnectData;
        m_placementAndRoute = PlaceAndRoute(r.m_randomConnectData, r.m_leftNum, r.m_rightNum, r.m_randomModulePortInfo);
        m_pos = m_placementAndRoute.getModulePos();
        m_size = m_placementAndRoute.getModuleSize();
        //m_paths = m_placementAndRoute.getPaths();

        // 获得版图大小
        m_row = m_placementAndRoute.getRowCount();
        m_column = m_placementAndRoute.getColumnCount();
        LineSearch lineSearch(m_row, m_column, m_pos, m_size);
        m_rowChannel = lineSearch.getRowChannel();
        m_columnChannel = lineSearch.getColumnChannel();
        qDebug() << m_rowChannel << "Row_Channel";
        qDebug() << m_columnChannel << "Column_Channel";

        // port位置信息
        m_rightPortPos = m_placementAndRoute.getRightPortPos();
        m_leftPortPos = m_placementAndRoute.getLeftPortPos();
        m_modulePortPos = m_placementAndRoute.getPortPos();

    }

protected:

    void paintEvent(QPaintEvent *event) override;

private:
    QVector<QPoint> m_pos;// 模块位置
    QVector<QPoint> m_size;// 模块大小
    PlaceAndRoute m_placementAndRoute;//
    QVector<QList<QPoint>> m_paths;// 路径
    QList<ConnectData> m_connectData;

    // 端口相关数据
    QVector<QPoint> m_rightPortPos;// 单独的输出port
    QVector<QPoint> m_leftPortPos;// 单独的输入port
    QVector<QVector<QPoint>> m_modulePortPos;// 模块的端口位置


    // lineSearch 数据
    QVector<QList<int>> m_rowChannel;
    QVector<QList<int>> m_columnChannel;

    // 版图大小
    int m_row = 0;
    int m_column = 0;
};



