//
// Created by Huangff on 2021/10/26.
//

#include <QDebug>
#include <QStack>

#include "CompCompute.h"

void printMatrix(int **m_matrix);

CompCompute::CompCompute(QList<ConnectionData> connectData) {
    /**
     * 根据components间的连接数据 确定component的相对位置
     *
     * 步骤 :
     *      1.数据初始化 计算邻接矩阵 计算出入度 计算必要的hash表
     *      2.判断是否有环
     *      3.有环的情况下 先识别反馈线 之后得到新的连接数据(无环--再次检测)
     *      4.无环的情况下 直接拓扑排序 得到拓扑序列
     *      5.根据拓扑序列和出入度的关系计算优先级  以便确定X相对位置
     *      6.Y相对位置的确定...待定...
     */


    //------ version_2
    // 数据异常
/*    if (NameVectorMapIndex.count() != COMPONENTNUM) {
        qDebug() << "Data ERROR!  number error";
        return;
    }
    m_connectData = connectData;
    initData();*/


    //------ version_1
    // 算法的复杂度的计算
    // 数据异常
    if (NameVectorMapIndex.count() != COMPONENTNUM) {
        qDebug() << "Data ERROR!  number error";
        return;
    }

    m_connectData = connectData;
    //m_connectData = randomConnectData();
    initCompCompute();

    // 检测环 移除环
    detectAndRemoveCircle();

    // 计算反馈线信息
    distinguishFeedbackLines();

    // 打印摘要信息 : 矩阵 出入度 反馈线
    printSummaryInfo();

    // 无环有向图 使用拓扑排序得到拓扑序列
    topologicalSort();

    // 根据拓扑排序以及度数计算优先级
    computingPriority();

    // 实现X的定级
    getXPosition();

/*



*/

}

QList<ConnectionData> CompCompute::randomConnectData() {

    QList<ConnectionData> randomData;
    int linesNum = NameVectorMapIndex.count() * NameVectorMapIndex.count();
    for (int i = 0; i < linesNum; ++i) {
        // Generates one random 32-bit quantity in the range between 0 (inclusive) and highest (exclusive).
        // [0,n)-->表示 0,1,2,...,n-1
        int sender = QRandomGenerator::global()->bounded(NameVectorMapIndex.count());
        int receiver = QRandomGenerator::global()->bounded(NameVectorMapIndex.count());
        ConnectionData connectionData;
        connectionData.sender = sender + 1;
        connectionData.receiver = receiver + 1;
        qDebug() << sender << " " << receiver;
        randomData.push_back(connectionData);
        //qDebug() << randomData.at(i).isFeedbackLine;
    }

    return randomData;
}

void CompCompute::initCompCompute() {

    initHashMap();

    // 初始化数据 邻接矩阵 出入度
    initCompInfos();
    printAdjacencyMatrix();  // 邻接矩阵的打印
    printComponentInfo();    // 出入度的打印

}

void CompCompute::detectAndRemoveCircle() {
    /**
     *  检测是否有环 并且识别反馈线 最终得到有向无环图
     */

    QVector<QVector<int>> circles = getCircles();
    // 有circle 则打印circle
    if (circles.count()) {

        // 识别反馈线
        identifyFeedbackLine(circles);
        //重新初始化数据 邻接矩阵 出入度
        initCompInfos();

        circles = getCircles();

        if (circles.count()) {
            qDebug() << "After the processing, there is still has loop!";
            return;
        } else {
            qDebug() << "After the processing, there is no loop!-----****";
        }

    } else {
        qDebug() << "doesn't has circle!";
    }

}

void CompCompute::distinguishFeedbackLines() {
    /**
     *  根据识别 存储反馈线 打印所有反馈线的信息
     */

    qDebug() << "---distinguishFeedbackLines***";
    qDebug() << "   base-component-index :";

    for (int j = 0; j < m_connectData.count(); ++j) {
        if (m_connectData.at(j).isFeedbackLine) {

            QString string;
            // string+=m_connectData.at(j).isFeedbackLine error  \u0001 true \u000 false
            // string += m_connectData.at(j).isFeedbackLine ? "true" : "false";
            string += "componentIndex__";
            string += QString::number(m_connectData.at(j).sender);

            string += "-->componentIndex__";
            string += QString::number(m_connectData.at(j).receiver);

            qDebug() << "   " << string;

            // 存储反馈线
            m_feedbackLines.push_back(m_connectData.at(j));

        } else {
            m_noneFeedbackLines.push_back(m_connectData.at(j));
        }
    }

    qDebug() << "---distinguishFeedbackLines----";
}

void CompCompute::topologicalSort() {
    /**
     *  使用拓扑排序对有向无环图进行排序
     *
     *  步骤:
     *      1.使用三个QHash map sender receiver
     *          map      存储 indexI-->indexJ
     *          sender   所有存储信号发送者的index
     *          receiver 所有存储信号接受者的index
     *      2.sender-receiver = 度数为0的所有结点
     *      3.sender和receiver的更新  存储拓扑排序
     *          sender = receiver
     *          receiver = map(sender)
     *          直到receiver为空
     *
     */

    // 使用拓扑排序对无环有向图进行排序
    // 对输入输出列表递归做差 则可以得到拓扑序列
    qDebug() << "-------------------------------topologicalSort----------------------------------";
    qDebug() << "   base-component-index :";

    // 存放有向无环图 int(senderIndex)-->int(receiverIndex)
    QHash<int, int> connectMap;
    // 存放输入列表
    QHash<int, int> senderHash;
    // 存放输出列表
    QHash<int, int> receiverHash;

    // 打印非反馈环的信息
    //printAllData(m_noneFeedbackLines);

    // 存放有向无环图
    for (int i = 0; i < m_noneFeedbackLines.count(); i++) {
        int sender = m_noneFeedbackLines.at(i).sender;
        int receiver = m_noneFeedbackLines.at(i).receiver;;
        connectMap.insert(sender, receiver);
    }
    //qDebug() << "--Map : " << connectMap;

    // 初始化senderHash
    QHashIterator<int, int> mapIterator(connectMap);
    while (mapIterator.hasNext()) {
        mapIterator.next();
        //qDebug() << iter.key() << ": " << iter.value();
        senderHash.insert(mapIterator.value(), mapIterator.value());
    }

    // 临时存放拓扑排序的结果
    QVector<int> tempSortResult;
    for (int i = 0; i < NameVectorMapIndex.count(); i++) {
        // 保存第一次度数为0的结点
        if (!senderHash.contains(NameVectorMapIndex.at(i).index)) {
            tempSortResult.push_back(NameVectorMapIndex.at(i).index);
        }
    }

    m_topologySortResult.push_back(tempSortResult);
    tempSortResult.clear();
    //qDebug() << "-------------" << m_topologySortResult;

    // 初始化receiverHash
    QHashIterator<int, int> senderIterator(senderHash);
    while (senderIterator.hasNext()) {
        senderIterator.next();
        // 需要注意 hash表 用不存在的key去获取value 会出现默认值0
        int value = connectMap.value(senderIterator.key());
        //qDebug() << iterator.key() << " : " << iterator.value() << " -- " << value;
        // 有senderHash.key(iterator)作为输出 且 输入不在receiverHash中 则添加
        if (connectMap.contains(senderIterator.key()) && !receiverHash.contains(value) && value) {
            receiverHash.insert(value, value);
        }
    }

    //qDebug() << "---------" << receiverHash;

    qDebug() << "----init---";
    qDebug() << "------------Map  : " << connectMap;
    qDebug() << "senderHash       : " << senderHash;
    qDebug() << "receiverHash     : " << receiverHash;
    qDebug() << "";

    //printAllData(m_noneFeedbackLines);

    // 两个hash表做差 得到拓扑序列
    while (!senderHash.isEmpty()) {

        tempSortResult.clear();
        QHashIterator<int, int> senderIter(senderHash);
        while (senderIter.hasNext()) {
            senderIter.next();
            if (!receiverHash.contains(senderIter.key())) {
                // 拓扑排序号码牌 m_componentsInfo[iterator].topologyIndex;
                //qDebug() << senderIter.key();
                tempSortResult.push_back(senderIter.key());
            }
        }

        // 更新拓扑排序列表
        m_topologySortResult.push_back(tempSortResult);

        // 更新 senderHash 和 receiverHash
        senderHash = receiverHash;
        // 先清除receiverHash 然后重新计算receiverHash
        // 清除
        receiverHash.clear();
        // 更新
        QHashIterator<int, int> iterator(senderHash);
        while (iterator.hasNext()) {
            iterator.next();
            // 需要注意 hash表 用不存在的key去获取value 会出现默认值0
            int value = connectMap.value(iterator.key());
            //qDebug() << iterator.key() << " : " << iterator.value() << " -- " << value;
            // 有senderHash.key(iterator)作为输出 且 输入不在receiverHash中 则添加
            if (connectMap.contains(iterator.key()) && !receiverHash.contains(value) && value) {
                receiverHash.insert(value, value);
            }
        }

        // 当前所有结点度数都为0
        if (receiverHash.count() == 0 || senderHash.count() == 1) { //

            //qDebug() << "senderHash       : " << senderHash << "                -----------   if";
            //qDebug() << "receiverHash     : " << receiverHash;
            //qDebug() << "";

            // 清除临时拓扑排序记录
            tempSortResult.clear();
            // 最后一次往拓扑序列中添加元素
            QHashIterator<int, int> iterator(senderHash);
            while (iterator.hasNext()) {
                iterator.next();
                //int value = connectMap.value(iterator.key());
                //qDebug() << iterator.key() << " : " << iterator.value() << " -- ";
                tempSortResult.push_back(iterator.key());
            }

            //qDebug() << "-----------Test----------  ";
            //break;
        }

        // 打印
        qDebug() << "------------Map  : " << connectMap;
        qDebug() << "senderHash       : " << senderHash;
        qDebug() << "receiverHash     : " << receiverHash;
        qDebug() << "";

    }

    // 打印
    /*qDebug() << "---finish--- : ";
    qDebug() << "------------Map  : " << connectMap;
    qDebug() << "senderHash       : " << senderHash;
    qDebug() << "receiverHash     : " << receiverHash;
    qDebug() << "";*/

    // 检测拓扑排序结果
    int componentNum = 0;
    for (int i = 0; i < m_topologySortResult.count(); ++i) {
        for (int j = 0; j < m_topologySortResult[i].count(); ++j) {
            componentNum++;
        }
    }

    // 打印拓扑排序结果
    if (componentNum != COMPONENTNUM) {
        qDebug() << "topology_sort error! ";
    } else {
        qDebug() << "topology_sort success! ";
        qDebug() << "        " << "sort_result : " << m_topologySortResult;
    }

    qDebug() << "-------------------------------topologicalSort----------------------------------";

}

QHash<int, int> CompCompute::computingPriority() {

    /** 根据拓扑排序以及度数计算优先级
     *  优先级影响因素 weight : 拓扑排序(index) 出度(outDegree) 入度(inDegree)
     *  计算公式 weight = index*outDegree-inDegree
     *
     *  优先级存储  --- hash表
     *
     *  步骤:
     *      1.计算 outDegree inDegree
     *      2.根据计算公式计算优先级并存储在hash表中 <int,int> componentIndex degree
     */

    qDebug() << "--------computingPriority--------";

    QHash<int, int> topologyIndex = getTopologySortIndex();
    QHash<int, int> priority;

    //printAdjacencyMatrix();
    //printComponentInfo();

    for (int i = 0; i < m_componentsInfo.count(); ++i) {
        //qDebug() << m_componentsInfo.at(i).componentName;
        if (m_nameHashMapIndex.contains(m_componentsInfo.at(i).componentName)) {

            // 根据m_nameHashMapIndex表快速获取name对应的index
            int componentIndex = m_nameHashMapIndex.value(m_componentsInfo.at(i).componentName);

            // 根据topologyIndex表快速获取拓扑排序index
            int index = topologyIndex.value(componentIndex);

            // 出入度的获取
            int outDegree = m_componentsInfo.at(i).outDegree;
            int inDegree = m_componentsInfo.at(i).inDegree;

            int weight = index * outDegree - inDegree;
            //weight = index * inDegree - outDegree;
            priority.insert(componentIndex, weight);

            qDebug() << "  " << "componentName:" << m_componentsInfo.at(i).componentName << " componentIndex "
                     << componentIndex << " componentWeight " << weight;
        }
    }

    if (priority.count() != COMPONENTNUM) {
        qDebug() << "computing priority error!   ---";
    } else {
        qDebug() << "  " << "computing priority success!" << "  priority_Hash:<componentIndex,componentWeight> "
                 << priority;
    }

    qDebug() << "--------computingPriority--------";

    return priority;
}

void CompCompute::getXPosition() {
    /**
     *
     *
     */


}

bool CompCompute::hasCircle() {
    /**
     * DFS 深度优先遍历判断有向图是否含有环
     * 使用栈来辅助记录所走的路径信息  使用非递归的方法
     *
     * 测试结果 : 代码仍有问题
     */
    // 非递归法  使用stack
    // todo modify --problem: stack detect loop --status: error

    for (int i = 0; i < COMPONENTNUM; ++i) {
        // 深度最多为 COMPONENTNUM
        // 以某一点为起点 打印深度搜索路径

        if (m_componentsInfo[i].outDegree == 0) {
            continue;
        }

        // 记录是否遍历过 遍历过为true 否则为false
        bool isChecked[COMPONENTNUM];
        for (int k = 0; k < COMPONENTNUM; ++k) {
            isChecked[k] = false;
        }

        // 记录component的编号index
        QStack<int> compIndexStack;
        // 清除stack
        compIndexStack.clear();
        if (!isChecked[i]) {
            compIndexStack.push(i);
            // 从第i个点出发
            //isChecked[i] = true;
        }

        while (!compIndexStack.isEmpty()) {
            // 记录从哪个点出发
            int temp = compIndexStack.pop();
            isChecked[temp] = true;

            // todo modify : use QHash
            for (int j = 0; j < COMPONENTNUM; ++j) {
                // i到j有路径
                if (m_adjacencyMatrix[temp][j] != 0) {
                    if (isChecked[j]) {
                        qDebug() << temp << "--------______-----" << j << " " << i;
                        return true;
                    } else {
                        compIndexStack.push(j);
                        //isChecked[j] = true;
                    }
                }
            }
        }
    }

    return false;
}

QVector<QVector<int>> CompCompute::getCircles() {
    /**
     *  深度遍历 非递归
     *  使用栈来辅助记录所走的路径信息
     *  如果路径中有loop则说明有环
     *
     *  todo modify --- index_1 检测有无环 有则移除 继续
     */
    qDebug() << "\n--getCircles***";

    // 如果有回路则打印回路信息 按照 a-->b-->c-->d-->a 打印
    QVector<QVector<int>> circleList;

    QStack<QVector<int>> qStack;
    for (int i = 0; i < COMPONENTNUM; ++i) {

        // 出度为0则直接 该结点不可能在环内
        if (m_componentsInfo[i].outDegree == 0) {
            continue;
        }

        // stack清除
        qStack.clear();

        // push 第一个结点
        QVector<int> temp;
        temp.push_back(i);
        qStack.push(temp);

        // 栈非空 出栈结点指向的结点进栈
        while (!qStack.isEmpty()) {
            QVector<int> temp = qStack.pop();
            // todo modify : simplify and lower complexity
            for (int j = 0; j < COMPONENTNUM; ++j) {
                if (m_adjacencyMatrix[temp.last()][j] != 0) {
                    //push
                    QVector<int> ttemp = temp;
                    if (ttemp.contains(j)) {
                        // 当前点被访问过 输出路径
                        ttemp.push_back(j);
                        QVector<int> vector;
                        int index = -1;
                        for (int k = 0; k < ttemp.count() - 1; k++) {
                            if (ttemp[k] == j || index != -1) {
                                index = k;
                                vector.push_back(ttemp[k]);
                            }
                        }
                        // 打印circle
                        //qDebug() << vector << "  vector";
                        circleList.push_back(vector);
                        //break; //--bug
                    } else {
                        ttemp.push_back(j);
                        qStack.push(ttemp);
                        //qDebug() << qStack << "  stack";
                    }
                }
            }
        }
    }

    // 打印circle 有重复的circle
/*    for (int i = 0; i < circleList.count(); ++i) {
        qDebug() << circleList.at(i);
    }*/
    QVector<QVector<int>> distinctCircles;
    computeDiffCircle(circleList, distinctCircles);

    //qDebug() << "--------------------------------------------\n\ndistinct circles:";

    // 打印circle 无重复的circle matrix的index
/*    qDebug() << "base-matrix-index :";
    for (int i = 0; i < distinctCircles.count(); ++i) {
        qDebug() << "   " << distinctCircles[i];
    }*/

    // 打印circle 无重复的circle component的index
    // 转换
    QVector<QVector<int>> circles = distinctCircles;

    for (int i = 0; i < distinctCircles.count(); ++i) {
        for (int j = 0; j < distinctCircles[i].count(); ++j) {
            circles[i][j] = NameVectorMapIndex[distinctCircles[i][j]].index;
        }
    }

    if (circles.count()) {
        qDebug() << "There are circles!";
        qDebug() << "input circle :  ";

        qDebug() << "   " << "base-component-index :";
        // 打印
        for (int i = 0; i < circles.count(); ++i) {
            qDebug() << "      " << circles[i];
        }

    } else {
        qDebug() << "There is no circle!";
    }

    qDebug() << "--getCircles----\n";

    return distinctCircles;
}

void CompCompute::identifyFeedbackLine(const QVector<QVector<int>> &circles) {
    /**
     *  识别反馈线
     *  根据反馈线标识 存储所有反馈线的信息  存放在 m_feedbackLines 列表中
     */
    qDebug() << "--------identifyFeedbackLine-----------------------";
    qDebug() << "   " << "base-component-index : ";

    for (int i = 0; i < circles.count(); ++i) {
        //qDebug() << circles[i]<<"---------------";
        // minEdges 最小边数 startIndex endIndex 分别表示起始点的Index
        int minEdges = 100, startIndex = -1;

        for (int j = 0; j < circles[i].count() / 2; ++j) {
            //  边数最小且不是反馈线
            //  todo modify : may be don't need change anything
            if (minEdges > m_adjacencyMatrix[circles[i][j]][circles[i][j + 1]]) {
                minEdges = m_adjacencyMatrix[circles[i][j]][circles[i][j + 1]];
                startIndex = j;
                //qDebug() << startIndex << "-->" << "----------------------------------";
            }
        }

        // 计算最小反馈线的index
        // todo modify -- index out of range
        int start = circles[i][startIndex], end = circles[i][startIndex + 1];
        // 打印最小反馈线
        qDebug() << "    " << NameVectorMapIndex[start].index << "-->" << NameVectorMapIndex[end].index
                 << "  degree: "
                 << m_adjacencyMatrix[start][end]; // matrix base index
        //qDebug() << start << "-->" << end << "  " << m_adjacencyMatrix[start][end]; // matrix base index
        //qDebug() << m_nameList.at(start) << "-->" << m_nameList.at(end) << "  " << m_adjacencyMatrix[start][end] << "---";

        // 标记反馈线
        // todo modify : use QHash
        for (int j = 0; j < m_connectData.count(); ++j) {
            if (!m_connectData.at(j).isFeedbackLine) {
                if (m_connectData.at(j).sender == NameVectorMapIndex.at(start).index &&
                    m_connectData.at(j).receiver == NameVectorMapIndex.at(end).index) {
                    // 将连线标记为反馈线
                    m_connectData[j].isFeedbackLine = true;
                    //qDebug() << "____________________";
                }
            }
        }
        //QList<ConnectionData> m_connectData;

    }

    qDebug() << "--------identifyFeedbackLine-----------------------";
}

void CompCompute::initCompInfos() {
    /**
     *  初始化邻接矩阵
     *  初始化componentInfo (name componentIndex inDegree outDegree)
     */

    m_componentsInfo.clear();
    for (int i = 0; i < COMPONENTNUM; ++i) {
        for (int j = 0; j < COMPONENTNUM; ++j) {
            // 初始化邻接矩阵
            m_adjacencyMatrix[i][j] = 0;
        }

        ComponentInfo componentInfo;
        componentInfo.componentName = NameVectorMapIndex[i].name;
        m_componentsInfo.push_back(componentInfo);

    }

    //  qDebug()<<m_componentsInfo.count()<<" infosCount";
    for (int i = 0; i < m_connectData.count(); ++i) {
        if (m_connectData.at(i).isFeedbackLine) {
            // 反馈线数据不用于计算邻接矩阵
            continue;
        }

        // todo modify : 使用QHash可以降低时间复杂度
        int senderIndex = getComponentIndex(m_connectData.at(i).sender);
        int receiver = getComponentIndex(m_connectData.at(i).receiver);

        if (senderIndex != -1 && receiver != -1) {
            m_adjacencyMatrix[senderIndex][receiver]++;
        } else {
            qDebug() << "Data Error! -- Error Type : cannot find component index!";
        }

        // 计算出度和入度
        if (senderIndex != -1) {
            //qDebug() << m_componentsInfo.at(temp).componentName;
            m_componentsInfo[senderIndex].outDegree++;
        }
        if (receiver != -1) {
            //qDebug() << m_componentsInfo.at(temp).componentName;
            m_componentsInfo[receiver].inDegree++;
        }
        //qDebug() << computeIndex(m_connectData.at(i).sender, m_nameList);
    }


    // test component info
    //printComponentInfo();
    // test 邻接matrix
    //printAdjacencyMatrix();

}

void CompCompute::printConnectData(QList<ConnectionData> list) {
    /**
     *  打印components间的连接信息
     */
    qDebug() << "---- print-component-data -----";
    for (int j = 0; j < list.count(); ++j) {

        QString string;
        // string+=m_connectData.at(j).isFeedbackLine error  \u0001 true \u000 false
        string += list.at(j).isFeedbackLine ? "true" : "false";
        string += " ";
        string += QString::number(list.at(j).sender);

        string += "-->";
        string += QString::number(list.at(j).receiver);

        qDebug() << string;
    }
    qDebug() << "**** print-component-data ****";
}

void CompCompute::printComponentInfo() {
    /**
     *  打印component相关信息
     *  例如:
     *      name : "a"  inDegree 3  outDegree 6
     */
    if (m_componentsInfo.count() != COMPONENTNUM) {
        qDebug() << "error --function-- printComponentInfo";
    }

    for (int i = 0; i < m_componentsInfo.count(); ++i) {
        qDebug() << "componentIndex: " << m_componentsInfo.at(i).componentIndex << "componentName: "
                 << m_componentsInfo.at(i).componentName << " inDegree: "
                 << m_componentsInfo.at(i).inDegree
                 << "outDegree: "
                 << m_componentsInfo.at(i).outDegree;
    }
}

void CompCompute::printAdjacencyMatrix() {
    /**
     *  打印邻接矩阵matrix
     */
    qDebug() << "----print adjacency Matrix----------";
    for (int i = 0; i < COMPONENTNUM; ++i) {
        QString string = "";
        for (int j = 0; j < COMPONENTNUM; ++j) {
            string += QString::number(m_adjacencyMatrix[i][j]);
            string += "  ";
        }
        qDebug() << string;
    }
    qDebug() << "----print adjacency Matrix----------";
}

QString CompCompute::getComponentName(int index) {
    /**
     *  根据componentIndex返回name
     */
    for (int i = 0; i < NameVectorMapIndex.count(); ++i) {
        if (NameVectorMapIndex.at(i).index == index) {
            return NameVectorMapIndex.at(i).name;
        }
    }
    return QString();
}

QHash<int, int> CompCompute::getTopologySortIndex() {
    /**
     *  计算拓扑排序结果
     */

    QHash<int, int> topologyIndex;// <int,int> componentIndex weight
    for (int i = 0; i < m_topologySortResult.count(); ++i) {
        for (int j = 0; j < m_topologySortResult[i].count(); ++j) {
            if (!topologyIndex.contains(m_topologySortResult[i][j])) {
                topologyIndex.insert(m_topologySortResult[i][j], m_topologySortResult.count() - i);

            }
        }
    }

    // 打印 TopologySortIndex
    if (topologyIndex.count() == COMPONENTNUM) {
        qDebug() << "   " << " topologySortIndex" << topologyIndex;
        return topologyIndex;
    } else {
        qDebug() << "topologySort Error";
        return QHash<int, int>();
    }

}

int CompCompute::getComponentIndex(QString name) {
    /**
     *  根据name获得其在邻接矩阵的index
     */
    for (int i = 0; i < NameVectorMapIndex.count(); ++i) {
        if (NameVectorMapIndex.at(i).name == name) {
            return i;
        }
    }
    return -1;
}

int CompCompute::getComponentIndex(int index) {
    /**
     *  根据index获取其在邻接矩阵的index
     */
    for (int i = 0; i < NameVectorMapIndex.count(); ++i) {
        if (NameVectorMapIndex.at(i).index == index) {
            return i;
        }
    }
    return -1;
}

bool CompCompute::isLoopInList(QVector<int> loop, QVector<QVector<int>> list) {
    /**
     *  circle是否在list中 : 是,则返回true; 否则,返回false
     */

    for (int j = 0; j < list.count(); ++j) {
        int compareFailTimes = 0;

        // 长度不匹配算对比失败
        if (loop.count() * 2 == list[j].count()) {
            //solution1: 123变为123123  出现123或231或312  -- select this  --速度快 需要更多空间   ---
            //solution2: 123 与312匹配需要循环移位 即取余 --空间小 速度慢

            // 计算第一个匹配index   loop--231  list.at(i)--123123  index =1
            int index = -1;
            for (int i = 0; i < loop.count(); ++i) {
                if (loop[0] == list[j][i]) {
                    index = i;
                }
            }

            // 匹配不上
            if (index == -1) {
                compareFailTimes++;
                continue;
            } else {
                // circle起点匹配成功
                // 对比成功个数
                int compareSuccessCount = 1;
                for (int k = 1; k < loop.count(); ++k) {
                    if (loop[k] == list[j][k + index]) {
                        compareSuccessCount++;
                    }
                }
                // 全部对比成功
                if (compareSuccessCount == loop.count()) {
                    return true;
                } else {
                    // 对比失败
                    compareFailTimes++;
                }
            }

        } else {
            compareFailTimes++;
        }

        // 全部对比失败 则将其加入list
        if (compareFailTimes == list.count()) {
            return false;
        }
    }

    return false;
}

void CompCompute::computeDiffCircle(QVector<QVector<int>> circle, QVector<QVector<int>> &list) {
    /**
     *  列表circle中可能含有重复的 loop , list用于保存各不相同的loop
     */
    for (int i = 0; i < circle.count(); ++i) {

        // 第一个circle直接放进list中
        if (list.count() == 0) {
            // 第一个直接加入其中
            circle[i] += circle[i];
            list.push_back(circle.at(i));
            continue;
        }

        // 若list中没有circle[i]
        if (!isLoopInList(circle[i], list)) {
            circle[i] += circle[i];
            list.push_back(circle.at(i));
        }
    }
}

void CompCompute::initHashMap() {
    /**
     *  初始化Map表
     *  QHash<QString,int> componentName--componentIndex
     */

    for (int i = 0; i < NameVectorMapIndex.count(); ++i) {
        m_nameHashMapIndex.insert(NameVectorMapIndex.at(i).name, NameVectorMapIndex.at(i).index);
    }
}

void CompCompute::printSummaryInfo() {
    /**
     * 1.打印反馈线信息和非反馈线信息
     *
     * 2.反馈线多少根 反馈线的比例为多少 ---
     *
     */
    printConnectData(m_feedbackLines);
    printConnectData(m_noneFeedbackLines);

    qDebug() << m_connectData.count();
    qDebug() << m_noneFeedbackLines.count();
    qDebug() << m_feedbackLines.count() * 1.0 / m_connectData.count();
}

// ------------------------------------------------------------------------------------
// 调整使用的容器
void CompCompute::initData() {
    /**
     * init QHash m_connectDataQhash
     * init m_componentsInfo
     * init degree
     *
     */

    m_connectData = randomConnectData();
    //m_connectData = FakeData ;

    // 测试randomData
    //printAllData(m_connectData);
    //qDebug() << m_connectData.count();

    initComponentsInfo();
    //printComponentInfo();

    initDegree();// 根据connectData计算出入度 计算dataHash
    //printComponentInfo();
    qDebug() << "  " << " adjacencyHashTable: ";
    qDebug() << m_connectDataQhash;

}

void CompCompute::distinguishFeedbackLines(int index) {
    /**
     * 每次识别一个index开始的环 并及时标记反馈线
     *
     *
     */
}

void CompCompute::initDegree() {

    // 邻接表
    // 时间复杂度 |V|+|E|

    //qDebug() << " ----------initDegree---------  ";

    QList<QHash<int, int>> dataList;
    for (int i = 0; i < COMPONENTNUM; ++i) {
        dataList.push_back(QHash<int, int>());
    }
    //qDebug() << m_connectDataQhash;

    for (int i = 0; i < m_connectData.count(); ++i) {
        //qDebug() << m_connectData.at(i).receiver;
        //qDebug() << m_connectData.at(i).sender;
        int senderIndex = m_connectData.at(i).sender;
        int receiverIndex = m_connectData.at(i).receiver;
        m_componentsInfo[receiverIndex].inDegree++;
        m_componentsInfo[senderIndex].outDegree++;

        // dataHash
        if (m_connectDataQhash.contains(senderIndex)) {

            qDebug() << m_connectDataQhash.value(senderIndex);
            //qDebug() << "---";
        }

        //qDebug() << dataList[i];
        // qhash 自动去重

        qDebug() << senderIndex << "-->" << receiverIndex;
        qDebug() << dataList[senderIndex].value(receiverIndex + 1);
        qDebug() << "------";
        dataList[senderIndex].insert(receiverIndex + 1, receiverIndex);
    }

    m_connectDataQhash.clear();
    /**
     * m_connectDataQHash <senderIndex,<receiverIndex,degree>>
     */
    for (int i = 0; i < NameVectorMapIndex.count(); ++i) {
        m_connectDataQhash.insert(i, dataList[i]);
    }

    //qDebug() << " ----------initDegree---------  ";

}

void CompCompute::initComponentsInfo() {

    for (int i = 0; i < COMPONENTNUM; ++i) {
        ComponentInfo componentInfo;
        componentInfo.componentIndex = i + 1;
        m_componentsInfo.push_back(componentInfo);
    }
}

void CompCompute::printAdjacencyTable(QList<QList<int>> dataList) {

    for (int i = 0; i < COMPONENTNUM; ++i) {
        for (int j = 0; j < dataList[i].count(); ++j) {
            qDebug() << dataList[i][j];
        }
    }

}

void CompCompute::detectAndRemoveLoop() {
    /**
     *  检测是否有环 并且识别反馈线 最终得到有向无环图
     */

    QVector<QVector<int>> circles = getCircles();
    // 有circle 则打印circle

    // 识别反馈线
    identifyFeedbackLine(circles);
    //重新初始化数据 邻接矩阵 出入度
    initCompInfos();

    while (!getOneLoop().isEmpty()) {
        // 识别反馈线
        //identifyFeedbackLine1();

        // 初始化邻接矩阵 出入度
        initData();
    }

    if (circles.count()) {

        // 识别反馈线
        identifyFeedbackLine(circles);
        //重新初始化数据 邻接矩阵 出入度
        initCompInfos();

        circles = getCircles();

        if (circles.count()) {
            qDebug() << "After the processing, there is still has loop!";
            return;
        } else {
            qDebug() << "After the processing, there is no loop!-----****";
        }

    } else {
        qDebug() << "doesn't has circle!";
    }
}

QList<int> CompCompute::getOneLoop() {
    /**
     * 计算出一个环所包含的元素并返回列表
     */

    return QList<int>();
}

void CompCompute::identifyFeedbackLine1(const QList<int> &loop) {
    /**
     * 根据给定loop 根据度数判定其中一条边为反馈环(权重最小的边,权重即结点A到结点B的连线个数)
     */
}

void printMatrix(int **m_matrix) {
    /**
     *  打印邻接矩阵 matrix
     *  lambda函数
     */

    for (int i = 0; i < COMPONENTNUM; ++i) {
        for (int j = 0; j < COMPONENTNUM; ++j) {
            qDebug() << m_matrix[i][j];
        }
    }
}
