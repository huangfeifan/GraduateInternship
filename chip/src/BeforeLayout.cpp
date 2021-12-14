//
// Created by Huangff on 2021/12/14.
//

#include "BeforeLayout.h"

BeforeLayout::BeforeLayout(int num) {

}

BeforeLayout::~BeforeLayout() {

}

void BeforeLayout::paintEvent(QPaintEvent *event) {
    // 根据矩形粗略绘制


    QWidget::paintEvent(event);
}

/*void BeforeLayout::setListRect(QList<QRect> listRect) {
    m_listRect = listRect;

}*/

void BeforeLayout::setListConnect(QList<QPoint> listConnect) {
    m_listConnect = listConnect;
}
