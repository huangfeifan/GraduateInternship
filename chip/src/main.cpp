#include <QApplication>
#include <QDebug>
#include <QTime>
#include <QQueue>


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


    // test scc algo
    TarjanAlgo tarjanAlgo;


    // test topology sort
    TopologySort topologySort;



    return app.exec();
}
