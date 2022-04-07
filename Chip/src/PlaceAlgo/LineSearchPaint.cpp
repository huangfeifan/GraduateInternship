//
// Created by Huangff on 2022/4/7.
//

#include "LineSearchPaint.h"

LineSearchPaint::~LineSearchPaint() {

}

void LineSearchPaint::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    /// 浅色绘制网格
    int widgetWidth = this->size().width();// 界面宽度
    int widgetHeight = this->size().height();// 界面高度
    int column = widgetWidth / GRID;
    int row = widgetHeight / GRID;
    painter.setPen(QColor(225, 229, 217));
    for (int i = 0; i < column; ++i) {
        // 列
        painter.drawLine(i * GRID, 0, i * GRID, widgetHeight);
    }
    for (int i = 0; i < row; ++i) {
        // 行
        painter.drawLine(0, i * GRID, widgetWidth, i * GRID);
    }

    /// 模块的绘制
    painter.setPen(QColor(0, 0, 0));
    for (int i = 0; i < m_size.size(); ++i) {
        int left = m_pos[i].x() * GRID;
        int top = m_pos[i].y() * GRID;
        int width = m_size[i].x() * GRID;
        int height = m_size[i].y() * GRID;

        QRect rect(left, top, width, height);
        painter.drawRect(rect);
        painter.drawText(left, top, QString::number(i));
    }

    // 切换颜色 查看
    painter.setPen(QColor(177, 177, 177));
    painter.drawLine(0, 0, 100, 100);


    QWidget::paintEvent(event);
}

