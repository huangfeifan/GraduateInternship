//
// Created by Huangff on 2021/11/6.
//
#include <QDebug>
#include <QRandomGenerator>

#include "GetLayout.h"


GetLayout::GetLayout() {

    if (NameVectorMapIndex.count() != COMPONENTNUM) {
        qDebug() << "Data ERROR!  number error";
        return;
    }

    /**
     * G=<V,E>
     * |V| 结点个数
     * |E| 边数
     *
     */

    // 随机生成数据 或者使用给定数据
    initConnectData();
    //m_connectData = FakeData;

    // 预处理数据 计算出入度
    /// 时间复杂度|V|+|E|    空间复杂度
    preHandleData();

    // 测试预处理后的数据
    printAllData();

    // 检测环后移除
    /// 时间复杂度   空间复杂度
    removeCircle();


}

GetLayout::~GetLayout() {

}

void GetLayout::initConnectData() {

    QList<ConnectionData> randomData;
    int linesNum = NameVectorMapIndex.count() * NameVectorMapIndex.count() * 10;
    for (int i = 0; i < linesNum; ++i) {
        // Generates one random 32-bit quantity in the range between 0 (inclusive) and highest (exclusive).
        // [0,n)-->表示 0,1,2,...,n-1
        int sender = QRandomGenerator::global()->bounded(NameVectorMapIndex.count()) + 1;
        int receiver = QRandomGenerator::global()->bounded(NameVectorMapIndex.count()) + 1;
        ConnectionData connectionData;
        connectionData.sender = sender;
        connectionData.receiver = receiver;
        randomData.push_back(connectionData);

        //qDebug() << sender << " " << receiver;
        //qDebug() << randomData.at(i).isFeedbackLine;
    }

    m_connectData = randomData;

}

void GetLayout::printAllData() {

    // allConnectData
    qDebug() << "All connect data(lines): ";

    for (int i = 0; i < m_connectData.count(); ++i) {
        int senderIndex = m_connectData.at(i).sender;
        int receiverIndex = m_connectData.at(i).receiver;
        qDebug() << senderIndex << "-->" << receiverIndex;
    }
    // 边数
    qDebug() << "linesCount: " << m_connectData.count();

    qDebug() << "All component info: ";
    for (int i = 0; i < COMPONENTNUM; ++i) {
        //qDebug() << senderIndex << "-->" << receiverIndex;
        qDebug() << i + 1 << " degree(in/out): " << m_componentsInfo[i].inDegree << " "
                 << m_componentsInfo[i].outDegree;
    }

    qDebug() << "Connect data hash: ";
    qDebug() << m_connectDataQHash;

    qDebug() << "simplify connect data hash: ";
    qDebug() << m_simplifyConnectData;
}

void GetLayout::preHandleData() {

    // 根据connectData计算出入度 计算dataHash
    QList<QHash<int, int>> dataList;
    for (int i = 0; i < COMPONENTNUM; ++i) {
        dataList.push_back(QHash<int, int>());

        // componentInfo
        ComponentInfo componentInfo;
        componentInfo.componentIndex = i + 1;
        m_componentsInfo.push_back(componentInfo);
    }
    //qDebug() << m_connectDataQhash;

    for (int i = 0; i < m_connectData.count(); ++i) {
        //qDebug() << m_connectData.at(i).receiver;
        //qDebug() << m_connectData.at(i).sender;
        int senderIndex = m_connectData.at(i).sender;
        int receiverIndex = m_connectData.at(i).receiver;

        if (senderIndex != receiverIndex) {

            // 非反馈线 并不是最终结果
            m_noneFeedbackLines.push_back(m_connectData[i]);

            // degree
            m_componentsInfo[receiverIndex - 1].inDegree++;
            m_componentsInfo[senderIndex - 1].outDegree++;

            //qDebug() << senderIndex << "-->" << receiverIndex;
            //qDebug() << dataList[senderIndex - 1].value(receiverIndex);
            int result = dataList[senderIndex - 1].value(receiverIndex);
            if (result == 0) {
                result = 1;
                // 第一次出现才添加
                m_simplifyConnectData.insert(senderIndex, receiverIndex);
            } else {
                result += 1;
            }
            //qDebug() << "------";
            dataList[senderIndex - 1].insert(receiverIndex, result);
        }
    }

    m_connectDataQHash.clear();
    /**
     * m_connectDataQHash <senderIndex,<receiverIndex,degree>>
     */
    for (int i = 0; i < NameVectorMapIndex.count(); ++i) {
        m_connectDataQHash.insert(i + 1, dataList[i]);
    }

    //qDebug() << m_connectDataQHash;
}

void GetLayout::removeCircle() {

    // 对度数进行排序 度数大 大概率在环内

    // 首先移除 1-->2-->1的环 时间复杂度|E|

    // 对非反馈边循环 |E| 循环内使用
}

void GetLayout::getDAG() {
    //DAG, Directed Acyclic Graph

    //
    for (int i = 0; i < COMPONENTNUM; ++i) {
        // if (m_)
    }

}

QList<line> GetLayout::getSortEdge() {
    /**
     * G=<V,E>
     * |V| 结点个数
     * |E| 边数
     *
     * 根据边的权重排序 |E|log|E|
     */
    QList<line> lines;
    for (int i = 0; i < 7; ++i) {

    }


    return lines;

}

QHash<int, QHash<int, int>> GetLayout::distinguishFeedbackLines() {
    // 根据leetdcode 无向图的冗余连接--并查集  时间复杂度:nlgn 类推实现有向图的冗余连接
    /**
     *  时间复杂度 MlogM
     *  1.对边根据权重(条数)进行排序 复杂度MlogM
     *  2.对边从高到低(E(i,j)表示从V(i)到V(j))  复杂度M
     *      如果边E(i,j)在不允许的边集-unableEdges里
     *          continue;
     *
     *      // 复杂度logM
     *      更新不允许的边集unableEdges
     *          计算V(j)的父节点将其加入不允许的边集
     */

    QList<line> sortEdges = getSortEdge();

    QHash<int, QHash<int, int>> unableEdges;

    // 预处理计算a-->b  b-->a类型的反馈线
    QHash<int, QHash<int, int>> feedbackLines;

    for (int i = 0; i < sortEdges.count(); ++i) {
        int sender = sortEdges.at(i).sender;
        int receiver = sortEdges.at(i).receiver;
        if (feedbackLines.contains(sender)) {
            if (true) {
                continue;
            }
        } else {
            //updateUnableEdges();
        }
    }

    return feedbackLines;
}


