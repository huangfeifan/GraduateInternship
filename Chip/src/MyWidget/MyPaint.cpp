//
// Created by Huangff on 2022/2/28.
//

#include "MyPaint.h"

MyPaint::~MyPaint() {

}

void MyPaint::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    const int xOffset = 0;
    const int yOffset = 0;
    /// 浅色绘制网格
    int widgetWidth = this->size().width();// 界面宽度
    int widgetHeight = this->size().height();// 界面高度
    int grid = 5;
    int column = widgetWidth / grid;
    int row = widgetHeight / grid;
    painter.setPen(QColor(225, 229, 217));
    for (int i = 0; i < column; ++i) {
        // 列
        painter.drawLine(i * grid, 0, i * grid, widgetHeight);
    }
    for (int i = 0; i < row; ++i) {
        // 行
        painter.drawLine(0, i * grid, widgetWidth, i * grid);
    }

    /// 模块的绘制
    painter.setPen(QColor(0, 0, 0));
    // 绘制module和port
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

    /// 单独的port

    /// 模块间的飞线
    //painter.setPen(Qt::DashDotLine);// search Qt::PenStyle
    //painter.drawLine(0, 0, 100, 100);

    // 模块间的连线
    QVector<QVector<int>> gridInfo = QVector<QVector<int>>(column);// 列
    for (int i = 0; i < gridInfo.size(); ++i) {
        gridInfo[i] = QVector<int>(row);// 行
        for (int j = 0; j < gridInfo[i].size(); ++j) {
            gridInfo[i][j] = 1;
        }
    }

    for (int i = 0; i < m_pos.size(); ++i) {
        int left = m_pos[i].x() / grid;// 列
        int top = m_pos[i].y() / grid;
        int width = m_size[i].x() / grid;//列
        int height = m_size[i].y() / grid;
        for (int j = -1; j < width + 1; ++j) {// 列
            for (int k = -1; k < height + 1; ++k) {
                if (top + k > 0 && left + j > 0) {
                    gridInfo[left + j][top + k] = 0;
                }
            }
        }
    }
    painter.setBrush(Qt::white);

    Router router(row, column);
    router.setGridInfo(gridInfo);
    // Source is the left-most bottom-most corner
    int x = m_size[5].x() / grid + 1;
    int y = m_pos[5].y() / grid + 3;
    Pair src = Pair(x, y);
    // Destination is the left-most top-most corner
    int x1 = m_pos[2].x() / grid + m_size[2].x() / grid + 1;
    int y1 = m_pos[0].y() / grid + 3;
    painter.setPen(Qt::DashDotLine);// search Qt::PenStyle
    painter.drawLine(x * grid, y * grid, x1 * grid, y1 * grid);
    Pair dest = Pair(x1, y1);
    router.aStarSearch(src, dest);
    QList<QPoint> list = paintRouterPaths(router.getPaths());

    painter.setPen(Qt::red);
    for (int i = 1; i < list.size(); ++i) {
        painter.drawLine(list[i - 1].x() * grid, list[i - 1].y() * grid, list[i].x() * grid, list[i].y() * grid);
    }

    // update gridInfo
    for (int i = 0; i < list.size(); ++i) {
        int x = list[i].x();
        int y = list[i].y();
        gridInfo[y][x] = 0;// blocked
        //qDebug() << x << " " << y;
    }

    // 绘制可布线区域
    for (int i = 0; i < column; ++i) {
        for (int j = 0; j < row; ++j) {
            if (gridInfo[i][j]) {
                //painter.drawRect(i * grid, j * grid, grid, grid);
            }
        }
    }

    //router = Router(row, column);
    //router.setGridInfo(gridInfo);
    x = m_size[5].x() / grid + 1;
    y = m_pos[5].y() / grid + 4;
    src = Pair(x, y);
    // Destination is the left-most top-most corner
    x1 = m_pos[2].x() / grid + m_size[2].x() / grid + 1;
    y1 = m_pos[0].y() / grid;
    //painter.setPen(Qt::DashDotLine);// search Qt::PenStyle
    //painter.drawLine(x * grid, y * grid, x1 * grid, y1 * grid);
    dest = Pair(x1, y1);
    //router.aStarSearch(src, dest);
    list.clear();
    list = paintRouterPaths(router.getPaths());

    painter.setPen(Qt::red);
    for (int i = 1; i < list.size(); ++i) {
        painter.drawLine(list[i - 1].x() * grid, list[i - 1].y() * grid, list[i].x() * grid, list[i].y() * grid);
    }

    QWidget::paintEvent(event);
}

QList<QPoint> MyPaint::paintRouterPaths(QStack<Pair> paths) {
    QList<QPoint> list;
    while (!paths.isEmpty()) {
        Pair pair = paths.pop();
        int x = pair.first;
        int y = pair.second;
        list.push_back(QPoint(x, y));
    }
    return list;
}


