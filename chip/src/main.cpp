
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
            {1, 5},//0
            {},//1
            {3, 4},//2
            {1},//3
            {},//4
            {6},//5
            {}//6
    };
    QList<bool> inStack;// 结点i是否遍历过
    QList<int> m_inDegree;//入度
    QList<QList<int>> result;
    QStack<int> stack;
    QStack<int> qStack;
    int time = 0;

    for (int i = 0; i < m_connection.size(); ++i) {
        m_inDegree.push_back(0);
        inStack.push_back(false);
    }
    // 初始化入度
    for (int i = 0; i < m_connection.size(); ++i) {
        for (int j = 0; j < m_connection[i].size(); ++j) {
            m_inDegree[m_connection[i][j]]++;
        }
    }
    qDebug() << inStack << "  inStack";
    qDebug() << m_inDegree << "  inDegree";

    int num = 0;
    for (int i = 0; i < m_connection.size(); ++i) {
        QList<int> temp;
        if (m_inDegree[i] == 0) {
            qStack.push_back(i);
            inStack[i] = true;
        }
    }
    time++;
    qDebug() << qStack;

    qDebug() << "while----";
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
            qDebug() << qStack<<"-------------";
            stack.clear();
            time++;
        }


    }
    qDebug() << "while----end";




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
