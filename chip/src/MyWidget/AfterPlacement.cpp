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

    QFont font = painter.font();

/* twice the size than the current font size */
    font.setPointSize(font.pointSize() * 2);
/* set the modified font to the painter */
    painter.setFont(font);

    // 绘制矩形
    for (int i = 0; i < m_num; ++i) {
        int x = (m_position[i].x() * 2 + 1) * grid;
        int y = (m_position[i].y() * 2 + 1) * grid;


        painter.drawRect(x, y, grid, grid);
        QString str = m_nameList[i];
        //str = nameList[i];

        painter.drawText(x, y, str);
    }

    // 简单绘制module之间的连线
    for (int i = 0; i < m_connection.size(); ++i) {
        for (int j = 0; j < m_connection[i].size(); ++j) {
            int start = i;
            int end = m_connection[i][j];

            int lineStartX = (m_position[start].x() * 2 + 1) * grid + grid;
            int lineStartY = (m_position[start].y() * 2 + 1) * grid + grid / 2;

            int lineEndX = (m_position[end].x() * 2 + 1) * grid;
            int lineEndY = (m_position[end].y() * 2 + 1) * grid + grid / 2;

            painter.drawLine(lineStartX, lineStartY, lineEndX, lineEndY);

        }
    }

    //qDebug() << "  paintEvent__AfterPlacement";

    painter.restore();
    QWidget::paintEvent(event);
}

AfterPlacement::~AfterPlacement() {

}
