#include <QApplication>
#include <QDebug>
#include <QTime>
#include <QQueue>


#include "mainwindow.h"
#include "SchematicPlacement.h"
#include "AfterPlacement.h"
#include "BeforeLayout.h"

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
    //SchematicPlacement tarjanAlgo;

    // test topology sort
    //TopologySort topologySort;

    // test placement
    Placement placement;

/*    AfterPlacement a;
    a.show();*/

/*    BeforeLayout b;
    b.show();*/


    return app.exec();
}
