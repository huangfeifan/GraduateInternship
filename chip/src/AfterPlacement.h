//
// Created by Huangff on 2021/12/14.
//

#pragma once

#include <QWidget>
#include <QPainter>
#include <QDebug>


#include "Data.h"
#include "SchematicPlacement.h"

class AfterPlacement : public QWidget {

Q_OBJECT

public: // functions 函数
    AfterPlacement() {
        init();

    }

    ~AfterPlacement();

    //void setListRect(QList<QRect> listRect);

    void setListConnect(QList<QPoint> listConnect) {

    }


public: // variable 变量

    int m_num;// mudule 个数
    int grid = 66;

    QList<QPoint> position;// 位置

    QList<QList<int>> m_connection = graphData;//邻接表

    void init() {
        m_num = m_connection.size();
        setWindowTitle("AfterPlacement");

        int width = m_num * 2 * grid + grid;
        if (width > 0) {
            //setFixedSize(width, width);
            resize(width,width);
        }

        Placement placement;
        position = placement.getRelativePostion();
    }

protected:

    void paintEvent(QPaintEvent *event) override;


private:


};
