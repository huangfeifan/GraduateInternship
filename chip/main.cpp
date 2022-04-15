
#include <QApplication>
#include <QPushButton>
#include <QTime>
#include <QDebug>

#include "PlaceAlgo/MyPaint.h"
#include "PlaceAlgo/LineSearch.h"
#include "PlaceAlgo/LineSearchPaint.h"
#include "PlaceAlgo/MikamiTabuchiAlgo.h"


int main(int argc, char *argv[]) {
    Q_INIT_RESOURCE(images);

    QApplication app(argc, argv);
    app.setAttribute(Qt::AA_DontCreateNativeWidgetSiblings);

    MyPaint myPaint;
    myPaint.show();
    //
    //LineSearch ls(100, 100);




    //QTime t;
    //t.start();
    //QList<QPoint> path = mt.routing(start, end);// point合法性的判断 todo add
    //qDebug("LineSearch Time elapsed: %d ms", t.elapsed());
    //qDebug() << path << " Path";


    //LineSearchPaint lss;
    //lss.show();

    return app.exec();
}


/*
QList<QPoint> path;
int index = 0;
int iterateCount = 2;
QList<QList<LineInfo>> startList;

QList<LineInfo> s0;// 零级轨道
QList<LineInfo> s1;
QList<LineInfo> s2;

LineInfo s0_h;
s0_h.start = QPoint(10, 40);
s0_h.end = QPoint(40, 40);
s0_h.isHorizontal = true;
s0.push_back(s0_h);

LineInfo s0_v;
s0_v.start = QPoint(20, 20);
s0_v.end = QPoint(20, 50);
s0_v.isHorizontal = false;
s0.push_back(s0_v);

LineInfo s1_v;
s1_v.start = QPoint(30, 20);
s1_v.end = QPoint(30, 50);
s1_v.isHorizontal = false;
s1.push_back(s1_v);

LineInfo s1_h;
s1_h.start = QPoint(10, 30);
s1_h.end = QPoint(40, 30);
s1_h.isHorizontal = true;
s1.push_back(s1_h);

LineInfo s1_hh;
s1_hh.start = QPoint(10, 20);
s1_hh.end = QPoint(50, 20);
s1_hh.isHorizontal = true;
s1.push_back(s1_hh);

LineInfo s2_v;
s2_v.start = QPoint(50, 10);
s2_v.end = QPoint(50, 50);
s2_v.isHorizontal = false;
s2.push_back(s2_v);


startList.push_back(s0);
startList.push_back(s1);
startList.push_back(s2);

//qDebug() << startList.size();
//for (int i = 0; i < startList.size(); ++i) {
//    qDebug() << startList[i].size();
//}
//mt.computePath(path, index, iterateCount, startList);


LineInfo t0_v;
t0_v.start = QPoint(30, 10);
t0_v.end = QPoint(30, 50);
t0_v.isHorizontal = false;


LineInfo t0_h;
t0_h.start = QPoint(10, 20);
t0_h.end = QPoint(50, 20);
t0_h.isHorizontal = true;

QVector<LineInfo> ttt(2);
ttt[0] = t0_h;
ttt[1] = t0_v;

qDebug() << mt.isIntersected(s0, ttt);

qDebug() << path;

mt.m_rowChannel[10].push_back(0);
mt.m_rowChannel[10].push_back(50);
mt.m_rowChannel[10].push_back(200);
mt.m_rowChannel[10].push_back(400);

LineInfo LineInfo_0;
LineInfo_0 = mt.computeHorizontalLine(QPoint(100, 100));
qDebug() << LineInfo_0.start;
qDebug() << LineInfo_0.end;

QList<int> channel;
channel.push_back(0);
channel.push_back(3);
channel.push_back(4);
channel.push_back(5);
int target = 3;
qDebug() << channel;
channel.insert(3, target);
qDebug() << channel;
channel.insert(3, 100);
qDebug() << channel;

channel.removeAt(3);
qDebug() << channel;
channel.removeAt(3);
qDebug() << channel;
*/


/*if (point.y() == end.y()) {// 一步到位
               // 重新探索
               getPriorityDirections(direction, point, end);
               isBlocked = QVector<bool>(4);// 重置
               aStarRouting(isFindPath, point, end, path, direction, isBlocked);
           } else {
               if (start == point) {// 探索失败
                   isBlocked[1] = true;
                   continue;
               } else {// 探索遇到了障碍
                   isBlocked = QVector<bool>(4);// 重置
                   isBlocked[1] = true;
                   continue;
               }
           }*/


