
#include <QApplication>
#include <QDebug>
#include <QTime>
#include <qqueue.h>

#include "mainwindow.h"
#include "TarjanAlgo.h"

int main(int argc, char *argv[]) {
    Q_INIT_RESOURCE(images);

    QApplication app(argc, argv);
    app.setAttribute(Qt::AA_DontCreateNativeWidgetSiblings);

    //MainWindow window;
    //window.show();

/*    QTime now = QTime::currentTime();
    qsrand(now.msec());
    qDebug() << qrand();*/


    //TarjanAlgo tarjanAlgo(7);

    QList<QList<int>> m_connection;//有向无环图的邻接表
    m_connection = {
            {},//0
            {},//1
            {3, 4},//2
            {1},//3
            {},//4
            {6},//5
            {4}//6
    };
    QList<int> m_inDegree;//入度
    QList<QStack<int>> result;// 拓扑排序结果
    QStack<int> stack;
    QStack<int> qStack;

    int sortVerticesNum = 0;    // 拓扑排序的顶点数
    bool isDAG = true;  //是有向无环图 DAG :=  Directed Acyclic Graph

    for (int i = 0; i < m_connection.size(); ++i) {
        m_inDegree.push_back(0);
    }
    // 初始化入度
    for (int i = 0; i < m_connection.size(); ++i) {
        for (int j = 0; j < m_connection[i].size(); ++j) {
            m_inDegree[m_connection[i][j]]++;
        }
    }
    //qDebug() << inStack << "  inStack";
    //qDebug() << m_inDegree << "  inDegree";

    for (int i = 0; i < m_connection.size(); ++i) {
        QList<int> temp;
        if (m_inDegree[i] == 0) {
            qStack.push_back(i);
        }
    }
    //qDebug() << qStack;
    result.push_back(qStack);


    //qDebug() << "while----";
    while (!qStack.isEmpty()) {
        int index = qStack.pop();
        qDebug() << qStack;
        for (int i = 0; i < m_connection[index].size(); ++i) {
            int temp = m_connection[index][i];
            m_inDegree[temp]--;
            if (m_inDegree[temp] == 0) {
                stack.push_back(temp);
            }
        }

        // 更新qStack
        if (qStack.isEmpty()) {
            qStack = stack;
            qDebug() << qStack << "-------------";
            if (!qStack.isEmpty()) {
                result.push_back(qStack);
            }
            stack.clear();
        }


    }
    qDebug() << "while----end";
    qDebug() << result;


    for (int i = 0; i < result.size(); ++i) {
        sortVerticesNum += result[i].size();
    }


    // 拓扑排序顶点数不等于有向图的顶点数  说明有向图有环
    if (sortVerticesNum != m_connection.size()) {
        isDAG = false;
    }







/*    for (int i = 0; i < m_connection.size(); ++i) {
        QList<int> temp;
        if (m_inDegree[i] == 0 && !inStack[i]) {
            inStack[i] = true;//
            for (int j = 0; j < m_connection[i].size(); ++j) {
                int index = m_connection[i][j];

                if (inStack[index]) {
                    continue;
                }
                m_inDegree[index]--;
            }
            temp.push_back(i);
            num++;
        }
        qDebug() << temp;
    }*/


    return app.exec();
}
