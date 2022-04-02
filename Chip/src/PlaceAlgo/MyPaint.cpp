//
// Created by Huangff on 2022/2/28.
//

#include "MyPaint.h"
#include "PlaceAlgo/Placement.h"
#include "PlaceAlgo/FakeData.h"

MyPaint::~MyPaint() {

}

void MyPaint::paintEvent(QPaintEvent *event) {
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
    //painter.translate(100, 100);

    /// 模块的绘制
    painter.setPen(QColor(0, 0, 0));

    //qDebug() << m_pos << "POS";
    //qDebug() << m_size << "SIZE";
    for (int i = 0; i < m_size.size(); ++i) {
        int left = m_pos[i].x() * GRID;
        int top = m_pos[i].y() * GRID;
        int width = m_size[i].x() * GRID;
        int height = m_size[i].y() * GRID;

        QRect rect(left, top, width, height);
        painter.drawRect(rect);
        painter.drawText(left, top, QString::number(i));
    }


    //qDebug() << m_paths.size() << "MY_PAINT_M_PATHS";
    for (int i = 0; i < m_paths.size(); ++i) {
        //i = 1;
        if (m_paths[i].size()) {// A*成功
            painter.setPen(Qt::red);
            for (int j = 1; j < m_paths[i].size(); ++j) {
                painter.drawLine(m_paths[i][j] * GRID, m_paths[i][j - 1] * GRID);
                //painter.drawText(m_paths[i][j] * GRID, QString::number(i));
            }
        } else {
            int startModule = m_connectData[i].startModuleIndex;
            int startPort = m_connectData[i].startPortIndex;
            int endModule = m_connectData[i].endModuleIndex;
            int endPort = m_connectData[i].endPortIndex;
            QPoint startPoint, endPoint;
            if (startModule == -1) {
                startPoint = m_leftPortPos[startPort];
            } else {
                startPoint = m_modulePortPos[startModule][startPort];
            }
            if (endModule == -1) {
                endPoint = m_rightPortPos[endPort];
            } else {
                endPoint = m_modulePortPos[endModule][endPort];
            }
            //painter.setPen(Qt::darkBlue);
            painter.setPen(Qt::DashDotLine);
            painter.drawLine(startPoint * GRID, endPoint * GRID);
        }
        //qDebug() << m_paths[i].size();
    }

    /// 绘制端口
    painter.setPen(Qt::blue);
    //qDebug() << m_leftPortPos.size() << " LEFT_PORT_SIZE";
    for (int i = 0; i < m_leftPortPos.size(); ++i) {
        QPoint point = m_leftPortPos[i] * GRID;
        //qDebug() << point;
        painter.drawLine(point.x(), point.y(), point.x() + GRID, point.y());
    }
    //qDebug() << m_rightPortPos.size() << " LEFT_PORT_SIZE";
    for (int i = 0; i < m_rightPortPos.size(); ++i) {
        QPoint point = m_rightPortPos[i] * GRID;
        //qDebug() << point;
        painter.drawLine(point.x(), point.y(), point.x() + GRID, point.y());
    }
    for (int i = 0; i < m_modulePortPos.size(); ++i) {
        for (int j = 0; j < m_modulePortPos[i].size(); ++j) {
            QPoint point = m_modulePortPos[i][j] * GRID;
            painter.drawLine(point.x(), point.y(), point.x() + GRID, point.y());
        }
    }

    QWidget::paintEvent(event);
}

void MyPaint::initData() {
    // 初始化数据
    //qDebug() << Fake_ModulePortInfo.size();

    RandomData r(10, 5, 4, 50);
    m_placementAndRoute = PlaceAndRoute(Fake_ConnectData, LEFT_PORT_NUM, RIGHT_PORT_NUM, Fake_ModulePortInfo);
    m_connectData = Fake_ConnectData;

    m_connectData = r.m_randomConnectData;
    m_placementAndRoute = PlaceAndRoute(r.m_randomConnectData, r.m_leftNum, r.m_rightNum, r.m_randomModulePortInfo);
    m_pos = m_placementAndRoute.getModulePos();
    m_size = m_placementAndRoute.getModuleSize();
    m_paths = m_placementAndRoute.getPaths();

    // port位置信息
    m_rightPortPos = m_placementAndRoute.getRightPortPos();
    m_leftPortPos = m_placementAndRoute.getLeftPortPos();
    m_modulePortPos = m_placementAndRoute.getPortPos();

    //qDebug() << m_pos << " POS";
    //qDebug() << m_size << " SIZE";
    //qDebug() << m_paths << "MY_PAINT_PATH";
}
