
#include <QApplication>
#include <QPushButton>
#include <QTime>
#include <QDebug>

#include "PlaceAlgo/MyPaint.h"

int main(int argc, char *argv[]) {
    Q_INIT_RESOURCE(images);

    QApplication app(argc, argv);
    app.setAttribute(Qt::AA_DontCreateNativeWidgetSiblings);

    MyPaint myPaint;
    myPaint.show();

    return app.exec();
}