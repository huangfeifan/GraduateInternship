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
#include "PlaceAlgo/Router.h"
#include "PlaceAlgo/FakeData.h"
#include "PlaceAlgo/PlaceAndRoute.h"
#include "PlaceAlgo/RandomData.h"

//#include "MyWidget/Data.h"


class MyPaint : public QWidget {

Q_OBJECT

public:
    MyPaint() {
        initData();
        setFixedSize(1200, 1000);
        //m_moduleSize = size;
    }

    ~MyPaint();

private:

    void initData();

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


