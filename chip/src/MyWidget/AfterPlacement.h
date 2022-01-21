//
// Created by Huangff on 2021/12/14.
//

#pragma once

#include <QWidget>
#include <QPainter>
#include <QDebug>


#include "Data.h"
#include "PlaceAlgo/SchematicPlacement.h"

class AfterPlacement : public QWidget {

Q_OBJECT

public: // functions 函数
    AfterPlacement(QList<QPoint> positionData, QList<QList<int>> connectionData, QStringList nameList) {
        m_connection = connectionData;
        m_position = positionData;
        m_nameList = nameList;
        m_num = m_connection.size();

        init();

    }

    ~AfterPlacement();

    //void setListRect(QList<QRect> listRect);

    void setListConnect(QList<QPoint> listConnect) {

    }


public: // variable 变量

    int m_num;// mudule 个数
    int grid = 66;
    QStringList m_nameList;

    QList<QPoint> m_position;// 位置

    QList<QList<int>> m_connection;//邻接表

    void init() {
        setWindowTitle("AfterPlacement");

        int width = m_num * 2 * grid + grid;
        if (width > 0) {
            resize(width, width);
        } else {
            qDebug() << "Data Error";
        }

    }

protected:

    void paintEvent(QPaintEvent *event) override;


private:


};
