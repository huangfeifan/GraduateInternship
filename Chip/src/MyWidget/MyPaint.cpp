//
// Created by Huangff on 2022/2/28.
//

#include "MyPaint.h"


MyPaint::~MyPaint() {

}

void MyPaint::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    for (int i = 0; i < m_size.size(); ++i) {
        QRect rect(m_pos[i].x(), m_pos[i].y(), m_size[i].x(), m_size[i].y());
        painter.drawRect(rect);
        painter.drawText(m_pos[i].x() + 3,m_pos[i].y() + 10, QString::number(i));
    }

    QWidget::paintEvent(event);
}
