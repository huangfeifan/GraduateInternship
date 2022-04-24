
#include <QApplication>
#include <QPushButton>
#include <QTime>
#include <QDebug>

#include "PlaceAlgo/MyPaint.h"
#include "PlaceAlgo/MainWindow.h"



int main(int argc, char *argv[]) {
    Q_INIT_RESOURCE(images);

    QApplication app(argc, argv);
    app.setAttribute(Qt::AA_DontCreateNativeWidgetSiblings);

    MainWindow myPaint;
    myPaint.show();

    return app.exec();
}


