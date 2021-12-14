//
// Created by Huangff on 2021/12/13.
//

#pragma once

#include <QColor>
#include <QGraphicsItem>

struct Object
{
    QString name;
};

struct Port : public Object
{
    int direction; // 1-input, 2-output, 3-inout

    QPoint center; // 位置（中心点）
    int radius; // 半径
};

struct Connection : public Object
{
    Port *src; // 源端口
    Port *dest; // 目的端口

    QList<QPoint> points; // 线段
};

struct Module : public Object
{
    QList<Port *> ports; // 端口
    QList<Connection *> connections; // 连接

    QPoint center; // 位置（中心点）
    QSize size; // 尺寸（长宽）
};



class MyChip : public QGraphicsItem {
public:
    MyChip(int x, int y, int width, int height);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override;

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;

    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

private:
    int m_x;
    int m_y;
    int m_width;
    int m_height;
};


