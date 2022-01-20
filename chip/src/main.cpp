#include <QApplication>
#include <QDebug>
#include <QTime>
#include <QQueue>


#include "mainwindow.h"
#include "SchematicPlacement.h"
#include "AfterPlacement.h"
#include "BeforeLayout.h"


struct BarAmout {
    int barLevel;
    QString diameter;
    double planAmount;
};

static QList <BarAmout> amout = {
        {1, "a"},
        {1, "b"},
        {1, "a"},
        {1, "c"},
        {2, "c"},
        {2, "b"},
        {2, "a"},
};

static bool compareBarData(const BarAmout &barAmout, const BarAmout &amout) {
    if (barAmout.barLevel < amout.barLevel) {
        // 从小到大排序
        return true;
    } else if (barAmout.barLevel > amout.barLevel) {
        return false;
    } else {
        QString strDiameter = barAmout.diameter;
        QString diameter = amout.diameter;
        if (strDiameter > diameter) {
            return true;
        } else {
            return false;
        }

    }
}

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

    AfterPlacement a;
    a.show();

/*    BeforeLayout b;
    b.show();*/


/*    // qt algorithm Sort的使用
    int 的比较
    QList<int> testSort = {2, 1, 5, 3};
    qDebug()<<testSort;
    std::sort(testSort.begin(),testSort.end());
    qDebug()<<testSort;

    // QString的比较
    QList <QString> list = {"aa", "ba", "bb", "ba", "ab"};
    std::sort(list.begin(), list.end());
    qDebug() << list;

    // 结构体元素的比较
    QList <BarAmout> list1 = amout;
    for (int i = 0; i < list1.size(); ++i) {
        qDebug() << list1[i].barLevel << " " << list1[i].diameter;
    }

    qDebug() << "-----";
    qSort(list1.begin(), list1.end(), compareBarData);
    for (int i = 0; i < list1.size(); ++i) {
        qDebug() << list1[i].barLevel << " " << list1[i].diameter;
    }*/

    return app.exec();
}
