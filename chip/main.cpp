#include <QApplication>
#include <QPushButton>
#include <QDebug>
#include <QTime>

#include "MyWidget/mainwindow.h"
#include "PlaceAlgo/Placer.h"
#include "MyWidget/AfterPlacement.h"
#include "PlaceAlgo/ComputeAbsolutePos.h"
#include "PlaceAlgo/SchematicPlacement.h"

int main(int argc, char *argv[]) {
    Q_INIT_RESOURCE(images);

    QApplication app(argc, argv);
    app.setAttribute(Qt::AA_DontCreateNativeWidgetSiblings);

    // graphView
/*    MainWindow window;
    window.show();*/

/*    QTime now = QTime::currentTime();
    qsrand(now.msec());
    qDebug() << qrand();*/


/*
    QHash<int, int> testHash;
    testHash.insert(4, -1);
    testHash.insert(5, 1);
    testHash.insert(6, 1);
    if (testHash.contains(4)) {
        qDebug() << testHash.value(4);
    }
*/

    // test scc algo
    //TrajanAlgo tarjanAlgo;

    // test topology sort
    //GetTopologySort topologySort;

    // test placement
    Placement placement;

/*    qDebug() << placement.getRelativePosition().sccSize();
    qDebug() << placement.m_nameList.sccSize();
    qDebug() << placement.m_nameListScc.sccSize();*/

    AfterPlacement a(placement.getRelativePosition(), graphData, placement.m_nameList);
    a.setWindowTitle("place all module");
    //a.show();

    AfterPlacement b(placement.m_relativePositionScc, placement.m_connectionScc, placement.m_nameListScc);
    b.setWindowTitle("place all scc");
    //b.show();

    AfterPlacement c(placement.relativePosition, placement.sccConnect, placement.sccList);
    c.setWindowTitle("a scc");
    //c.show();


    qDebug() << "-------------------------\n\n\n";

    QVector<QPoint> size = QVector<QPoint>(graphData.size());
    for (int i = 0; i < graphData.size(); ++i) {
        size[i].setX(10);
        size[i].setY(10);
    }

    //qDebug() << size;

    SchematicPlacement sp(graphData, size);


    // vector init Sze
/*    QVector<int> a(10);
    qDebug() << a;*/


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
    QList <Bar> list1 = amout;
    for (int i = 0; i < list1.sccSize(); ++i) {
        qDebug() << list1[i].barLevel << " " << list1[i].diameter;
    }

    qDebug() << "-----";
    qSort(list1.begin(), list1.end(), compareBarData);
    for (int i = 0; i < list1.sccSize(); ++i) {
        qDebug() << list1[i].barLevel << " " << list1[i].diameter;
    }*/

    return app.exec();
}


struct Bar {
    int barLevel;
    QString diameter;
    double planAmount;
};

static QList<Bar> amout = {
        {1, "a"},
        {1, "b"},
        {1, "a"},
        {1, "c"},
        {2, "c"},
        {2, "b"},
        {2, "a"},
};

static bool compareBarData(const Bar &bar, const Bar &bar2) {
    // 根据多个维度来排序 在指标Ⅰ相同的情况下，根据指标Ⅱ进行排序

    if (bar.barLevel < bar2.barLevel) {
        // 从小到大排序
        return true;
    } else if (bar.barLevel > bar2.barLevel) {
        return false;
    } else {
        QString strDiameter = bar.diameter;
        QString diameter = bar2.diameter;
        if (strDiameter > diameter) {
            return true;
        } else {
            return false;
        }

    }
}
