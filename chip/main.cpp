
#include <QApplication>
#include <QPushButton>
#include <QTime>
#include <QDebug>

#include "PlaceAlgo/MyPaint.h"
#include "PlaceAlgo/LineSearch.h"
#include "PlaceAlgo/LineSearchPaint.h"

int main(int argc, char *argv[]) {
    Q_INIT_RESOURCE(images);

    QApplication app(argc, argv);
    app.setAttribute(Qt::AA_DontCreateNativeWidgetSiblings);

    //MyPaint myPaint;
    //myPaint.show();

    LineSearch ls(100, 100);

    LineSearchPaint lss;
    lss.show();

    return app.exec();
}