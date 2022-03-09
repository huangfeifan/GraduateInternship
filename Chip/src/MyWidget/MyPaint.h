//
// Created by Huangff on 2022/2/28.
//

#pragma once

#include <QWidget>
#include <QPainter>

#include "PlaceAlgo/MyStruct.h"

class MyPaint : public QWidget {

Q_OBJECT

public:
    MyPaint(QVector<QPoint> pos, QVector<QPoint> size, QVector<int> in, QVector<int> out) : m_pos(pos), m_size(size),
                                                                                            m_inDegree(in),
                                                                                            m_outDegree(out) {
        //m_size = size;
    }

    ~MyPaint();

protected:

    void paintEvent(QPaintEvent *event) override;

private:
    QVector<int> m_inDegree;
    QVector<int> m_outDegree;
    QVector<QPoint> m_pos;
    QVector<QPoint> m_size;

};


