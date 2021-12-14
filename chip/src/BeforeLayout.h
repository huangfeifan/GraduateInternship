//
// Created by Huangff on 2021/12/14.
//

#pragma once

#include <QWidget>

class BeforeLayout : public QWidget {
Q_OBJECT

public: // functions 函数
    BeforeLayout(int num);

    ~BeforeLayout();

    //void setListRect(QList<QRect> listRect);

    void setListConnect(QList<QPoint> listConnect);


public: // variable 变量

    int m_num;// mudule 个数
    QList<QRect> m_listRect;// module size
    QList<QPoint> m_listConnect;// connect


protected:
    void paintEvent(QPaintEvent *event) override;


private:


};



