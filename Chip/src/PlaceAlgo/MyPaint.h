//
// Created by Huangff on 2022/2/28.
//

#pragma once

#include <QWidget>
#include <QPainter>
#include <QDebug>
#include <QVector>

#include "PlaceAlgo/ComputeAbsolutePos.h"
#include "PlaceAlgo/Placement.h"
#include "PlaceAlgo/MyStruct.h"
#include "PlaceAlgo/AStarRouter.h"
#include "PlaceAlgo/FakeData.h"
#include "PlaceAlgo/PlaceAndRoute.h"
#include "PlaceAlgo/RandomData.h"

//#include "MyWidget/Data.h"


class MyPaint : public QWidget {

Q_OBJECT

public:
    MyPaint(QWidget *parent = nullptr) {
        initData();
        //setFixedSize(1200, 1000);
        //m_moduleSize = size;
    }

    ~MyPaint();

private:

    void initData() {
        // 初始化数据
        //qDebug() << Fake_ModulePortInfo.size();

        RandomData r(MODULE_COUNT, LEFT_PORT_NUM, RIGHT_PORT_NUM, CONNECT_NUM);// 10个模块
        if (isRandomData) {
            m_connectData = r.m_randomConnectData;
            m_placementAndRoute = PlaceAndRoute(r.m_randomConnectData, r.m_leftNum, r.m_rightNum,
                                                r.m_randomModulePortInfo);
        } else {
            m_placementAndRoute = PlaceAndRoute(Fake_ConnectData, LEFT_PORT_NUM, RIGHT_PORT_NUM, Fake_ModulePortInfo);
            m_connectData = Fake_ConnectData;
        }

        m_pos = m_placementAndRoute.getModulePos();
        m_size = m_placementAndRoute.getModuleSize();
        m_paths = m_placementAndRoute.getPaths();

        // port位置信息
        m_rightPortPos = m_placementAndRoute.getRightPortPos();
        m_leftPortPos = m_placementAndRoute.getLeftPortPos();
        m_modulePortPos = m_placementAndRoute.getPortPos();

        //qDebug() << m_pos << " POS";
        //qDebug() << m_size << " SIZE";
        //qDebug() << m_paths << "MY_PAINT_PATH";
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

};


