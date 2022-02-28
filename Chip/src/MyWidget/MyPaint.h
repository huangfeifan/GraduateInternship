//
// Created by Huangff on 2022/2/28.
//

#pragma once

#include <QWidget>
#include <QPainter>

class MyPaint : public QWidget {

Q_OBJECT

public:
    MyPaint(QVector<QPoint> pos, QVector<QPoint> size) {
        m_pos = pos;
        m_size = size;
    }

    ~MyPaint();

protected:

    void paintEvent(QPaintEvent *event) override;

private:
    QVector<QPoint> m_pos;
    QVector<QPoint> m_size;

};


