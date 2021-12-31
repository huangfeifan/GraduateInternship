//
// Created by Huangff on 2021/12/20.
//

#include "AfterPlacement.h"

void AfterPlacement::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.save();
    painter.setPen(Qt::black);
    //painter.setBrush(Qt::red);

    QStringList nameList;
    //nameList << "11" << "12" << "13" << "21" << "22" << "23" << "31" << "32" << "33";
    // 绘制矩形
    for (int i = 0; i < m_num; ++i) {
        int x = (position[i].x() * 2 + 1) * grid;
        int y = (position[i].y() * 2 + 1) * grid;

        painter.drawRect(x, y, grid, grid);
        QString str = QString::number(i);
        //str = nameList[i];
        painter.drawText(x, y, str);
    }

    // 绘制module之间的连线
    for (int i = 0; i < m_connection.size(); ++i) {
        for (int j = 0; j < m_connection[i].size(); ++j) {

        }
    }

    //qDebug() << "  paintEvent__AfterPlacement";

    painter.restore();
    QWidget::paintEvent(event);
}

AfterPlacement::~AfterPlacement() {

}
