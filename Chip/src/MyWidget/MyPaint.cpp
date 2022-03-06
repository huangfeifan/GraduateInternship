//
// Created by Huangff on 2022/2/28.
//

#include "MyPaint.h"


MyPaint::~MyPaint() {

}

void MyPaint::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    const int xOffset = 66;
    const int yOffset = 66;
    for (int i = 0; i < m_size.size(); ++i) {
        QRect rect(m_pos[i].x() + xOffset, m_pos[i].y() + yOffset, m_size[i].x(), m_size[i].y());
        painter.drawRect(rect);
        painter.drawText(m_pos[i].x() + 3 + xOffset, m_pos[i].y() + 10 + yOffset, QString::number(i));
    }

    QWidget::paintEvent(event);
}
