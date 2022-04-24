//
// Created by Huangff on 2022/4/8.
//

#include "MainWindow.h"
#include "MyPaint.h"

MainWindow::~MainWindow() {


}

MainWindow::MainWindow(QWidget *parent) {
    setWindowTitle("MainWindow");

    QScrollArea *scrollArea = new QScrollArea(this);
    setCentralWidget(scrollArea);

    MyPaint *widget = new MyPaint(scrollArea);
    widget->setFixedSize(4000, 4000);
    scrollArea->setWidget(widget);
}
