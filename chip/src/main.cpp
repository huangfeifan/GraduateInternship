
#include <QApplication>
#include <QDebug>
#include <QTime>

#include "mainwindow.h"
#include "TarjanAlgo.h"

int main(int argc, char *argv[]) {
    Q_INIT_RESOURCE(images);

    QApplication app(argc, argv);
    app.setAttribute(Qt::AA_DontCreateNativeWidgetSiblings);

    //MainWindow window;
    //window.show();

/*    QTime now = QTime::currentTime();
    qsrand(now.msec());
    qDebug() << qrand();*/


    TarjanAlgo tarjanAlgo(7);


    return app.exec();
}
