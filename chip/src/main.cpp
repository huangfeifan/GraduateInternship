
#include <QApplication>
#include <QDebug>
#include <QTime>

#include "mainwindow.h"

int main(int argc, char *argv[]) {
    Q_INIT_RESOURCE(images);

    QApplication app(argc, argv);
    app.setAttribute(Qt::AA_DontCreateNativeWidgetSiblings);

    //MainWindow window;
    //window.show();

    QTime now = QTime::currentTime();
    qsrand(now.msec());
    qDebug() << qrand();

    return app.exec();
}
