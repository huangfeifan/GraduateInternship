//
// Created by Huangff on 2021/11/6.
//

#ifndef PLACE_GETLAYOUT_H
#define PLACE_GETLAYOUT_H

#include <QHash>
#include "Data.h"

class GetLayout {

public:
    GetLayout();

    ~GetLayout();

private:

    void initConnectData();

    void printAllData();

    void preHandleData();

    void removeCircle();

    QHash<int, QHash<int, int>> distinguishFeedbackLines();

    void getDAG();//DAG, Directed Acyclic Graph

    QList<line> getSortEdge();

private:
    QList<ConnectionData> m_connectData;// 连接信息
    QList<ConnectionData> m_feedbackLines;// 反馈线
    QList<ConnectionData> m_noneFeedbackLines;// 非反馈线
    QList<ComponentInfo> m_componentsInfo;//component info
    QVector<QVector<int>> m_topologySortResult;    // 列表存储拓扑排序的结果

    QHash<int, QHash<int, int>> m_connectDataQHash;//连接信息 <sender,<receiver,linesNum>>
    QMultiHash<int, int> m_simplifyConnectData;//<sender,receiver>
};


#endif //PLACE_GETLAYOUT_H
