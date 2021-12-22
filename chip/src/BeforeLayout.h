//
// Created by Huangff on 2021/12/14.
//

#pragma once

#include <QWidget>
#include <QDebug>

#include "Data.h"

class BeforeLayout : public QWidget {
Q_OBJECT

public: // functions 函数
    BeforeLayout() {

        init();

    }

    ~BeforeLayout();

    //void setListRect(QList<QRect> listRect);
    void init() {
        m_num = m_connection.size();
        //qDebug() << "num " << m_num;
        setWindowTitle("BeforePlacement");

        int width = m_num * 2 * grid + grid;

        if (width > 0) {
            setFixedSize(width, width);
        }


        // random position
        randomPosition();
    }


public: // variable 变量

    int m_num;// mudule 个数
    int grid = 30;

    QList<QPoint> position;// 位置

    QList<QList<int>> m_connection = graphData;//邻接表

    QList<bool> isPlaced;// 模块是否被摆放

    QList<QHash<int, int>> isOccupy;//位置被占据

protected:
    void paintEvent(QPaintEvent *event) override;


private:
    void randomPosition() {
        for (int i = 0; i < m_num; ++i) {
            isOccupy.push_back(QHash<int, int>());
        }
        //qDebug() << isOccupy << "  isOccupy";

        // todo modify------------
        for (int i = 0; i < m_num; ++i) {
            /*int row = 0;
            int column = 0;
            // 该位置没有被占据
            isOccupy[column].insert(row, row);

            // 设置i的position
            position[i].setX(column);
            position[i].setY(row);*/
        }

        qDebug() << position << "  position";

    }


};



