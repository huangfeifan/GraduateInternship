//
// Created by Huangff on 2021/10/26.
//

#pragma once

#include <QString>
#include <QVector>
#include <QRandomGenerator>

#include "Data.h"

struct noLoopConnectionData {
    int senderIndex = 0;
    int receiverIndex = 0;
    int linesNum = 0;
    int markIndex = 1;

};

class CompCompute {


public:

    CompCompute(QList<ConnectionData> connectData);


private:

    QList<ConnectionData> randomConnectData();

    void initCompCompute();

    void detectAndRemoveCircle();

    void distinguishFeedbackLines();

    void topologicalSort();

    QHash<int, int> computingPriority();

    void getXPosition();


private:

    void initCompInfos();

    void printConnectData(QList<ConnectionData> list);

    void printComponentInfo();

    void printAdjacencyMatrix();

    void printSummaryInfo();

    void initHashMap();

    bool hasCircle();   // error--todo modify -- Status: error!

    bool isLoopInList(QVector<int> loop, QVector<QVector<int>> list);

    int getComponentIndex(QString name);

    int getComponentIndex(int index);

    void identifyFeedbackLine(const QVector<QVector<int>> &circles);

    void computeDiffCircle(QVector<QVector<int>> circle, QVector<QVector<int>> &list);

    QHash<int, int> getTopologySortIndex();

    QVector<QVector<int>> getCircles();

    QString getComponentName(int index);


private:  // variable 变量

    QList<ConnectionData> m_connectData;// 连接信息
    QList<ConnectionData> m_feedbackLines;// 反馈线
    QList<ConnectionData> m_noneFeedbackLines;// 非反馈线
    QList<ComponentInfo> m_componentsInfo;//component info

    QVector<QVector<int>> m_topologySortResult;    // 列表存储拓扑排序的结果
    QVector<QVector<int>> m_xPosition;    // 列表存储X定级的结果

    QHash<QString, int> m_nameHashMapIndex;

    int m_adjacencyMatrix[COMPONENTNUM][COMPONENTNUM];//Adjacency matrix 邻接矩阵 存储结点到结点的直接连线个数


private:

    void initData();

    void initComponentsInfo();

    void initDegree();

    void distinguishFeedbackLines(int index);

    void detectAndRemoveLoop();

    void printAdjacencyTable(QList<QList<int>> dataList);

    QList<int> getOneLoop();

    void identifyFeedbackLine1(const QList<int> &loop);


private:
    QHash<int, QHash<int, int>> m_connectDataQhash;//连接信息


};