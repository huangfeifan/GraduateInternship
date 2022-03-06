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
        int left = m_pos[i].x() + xOffset;
        int top = m_pos[i].y() + yOffset;
        int width = m_size[i].x();
        int height = m_size[i].y();

        // 左侧输入port
        for (int j = 0; j < m_inDegree[i]; ++j) {
            QRect qRect(left - 5, top + (j + 1) * 10, 5, 5);
            painter.drawRect(qRect);
        }

        // 右侧输出port
        for (int j = 0; j < m_outDegree[i]; ++j) {
            QRect qRect(left + width, top + (j + 1) * 10, 5, 5);
            painter.drawRect(qRect);
        }

        QRect rect(left, top, width, height);
        painter.drawRect(rect);
        painter.drawText(m_pos[i].x() + 3 + xOffset, m_pos[i].y() + 10 + yOffset, QString::number(i));
    }

    QWidget::paintEvent(event);
}
