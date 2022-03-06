#include <QApplication>
#include <QPushButton>
#include <QTime>
#include <QRandomGenerator>
#include <QDebug>

#include "MyWidget/mainwindow.h"
#include "MyWidget/MyPaint.h"
#include "MyWidget/AfterPlacement.h"

#include "PlaceAlgo/Placer.h"
#include "PlaceAlgo/ComputeAbsolutePos.h"
#include "PlaceAlgo/SchematicPlacement.h"

int main(int argc, char *argv[]) {
    Q_INIT_RESOURCE(images);

    QApplication app(argc, argv);
    app.setAttribute(Qt::AA_DontCreateNativeWidgetSiblings);

//    qDebug() << placement.getRelativePosition().sccSize();
//    qDebug() << placement.m_nameList.sccSize();
//    qDebug() << placement.m_nameListScc.sccSize();

    //AfterPlacement a(placement.getRelativePosition(), graphData, placement.m_nameList);
    //a.setWindowTitle("place all module");
    //a.show();

    //AfterPlacement b(placement.m_relativePositionScc, placement.m_connectionScc, placement.m_nameListScc);
    //b.setWindowTitle("place all scc");
    //b.show();

    //AfterPlacement c(placement.relativePosition, placement.sccConnect, placement.sccList);
    //c.setWindowTitle("a scc");
    //c.show();



    // 模块数 1-10
    int maxModuleCount = 10;
    int moduleCount = QRandomGenerator::global()->bounded(maxModuleCount);
    //moduleCount = 5;
    //qDebug() << moduleCount << " ModuleCount";

    // in the range between 0 (inclusive) and highest (exclusive)
    int connectLineCount = 5 * QRandomGenerator::global()->bounded(40);
    QVector<QPoint> connectList = QVector<QPoint>(connectLineCount);// 模拟连接数据
    QVector<int> inDegree = QVector<int>(moduleCount);
    QVector<int> outDegree = QVector<int>(moduleCount);
    for (int i = 0; i < connectLineCount; ++i) {
        int start = QRandomGenerator::global()->bounded(moduleCount + 1) - 1;
        int end = QRandomGenerator::global()->bounded(moduleCount + 1) - 1;
        //qDebug() << start << " " << end;
        if (start != end) {
            if (start == -1 && end == -1) {
                // 数据无效
                continue;
            }

            if (start != -1 && end != -1) {
                outDegree[start]++;
                inDegree[end]++;
                //qDebug() << start << " " << end;
            }


            if (start == -1) {
                // start = -1 表示单独的输入port
                if (end != -1) {

                }
            }

            if (end == -1) {
                // end = -1 表示单独的输出port
                if (start != -1) {

                }
            }
        }
    }

    //qDebug() << inDegree << " InDegree";
    //qDebug() << outDegree << " outDegree";

    //qDebug() << "-------------------------\n\n\n";
    int width = 100;
    int gap = 5;
    QVector<QPoint> size = QVector<QPoint>(moduleCount);
    for (int i = 0; i < moduleCount; ++i) {
        size[i].setX(width);

        int max = inDegree[i] > outDegree[i] ? inDegree[i] : outDegree[i];
        int height = (max + 1) * gap;

        size[i].setY(height);
    }
    //qDebug() << size << "   Size";

    qDebug() << "-------------------------\n\n\n";
    /// 计算模块大小 根据输入输出个数() 高度统一(参数)
    moduleCount = graphData.size();
    size = QVector<QPoint>(moduleCount);
    inDegree = QVector<int>(moduleCount);
    outDegree = QVector<int>(moduleCount);
    for (int i = 0; i < graphData.size(); ++i) {
        // 出度
        outDegree[i] = graphData[i].size();
        int end;
        for (int j = 0; j < graphData[i].size(); ++j) {
            end = graphData[i][j];
            inDegree[end]++;
        }
    }
    const int WIDTH = 30;
    int height = 10;// 一个port 20 两个port30 依次类推
    for (int i = 0; i < moduleCount; ++i) {
        size[i].setX(WIDTH);
        int temp = inDegree[i] > outDegree[i] ? inDegree[i] : outDegree[i];
        size[i].setY((temp + 1) * height);
    }
    //qDebug() << size;

    SchematicPlacement sp(graphData, size);


    MyPaint myPaint(sp.getModulePos(), size, inDegree, outDegree);
    myPaint.show();

    return app.exec();
}

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
//Placement placement;

// vector init Sze
/*    QVector<int> a(10);
    qDebug() << a;*/


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
