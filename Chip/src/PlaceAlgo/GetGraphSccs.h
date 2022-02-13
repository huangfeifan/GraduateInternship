//
// Created by Huangff on 2022/1/26.
//

#pragma once

#include <QDebug>
#include <QStack>

// 递归版本 Trajan_algorithm
class GetGraphSccs {
    // Time Complexity O(N+E) Space Complexity O(N+E) 邻接表存储数据
    // link https://www.bilibili.com/video/BV19J411J7AZ?p=5 邻接矩阵
    // link https://segmentfault.com/a/1190000039149539  邻接表
public :
    // 递归版本
    GetGraphSccs(QVector<QList<int>> graphData): m_connection(graphData) {
        // 初始化邻接表
        //m_connection = graphData;
        // number 模块数量
        m_numVertices = m_connection.size();
        // init
        for (int i = 0; i < m_numVertices; ++i) {
            m_dfn.push_back(-1);
            m_low.push_back(-1);
            m_inStack.push_back(false);
        }
        //qDebug() << m_connection.size();
        getSCC();
        //qDebug() << m_sccs.size() << "  scc_size";
        for (int i = 0; i < m_sccs.size(); ++i) {
            qDebug() << m_sccs[i];
        }
        qDebug() << "TrajanAlgo----------------------------------------------------";
    }

    QList<QList<int>> getGraphAllScc() {
        return m_sccs;
    }

private:
    QVector<QList<int>> m_connection;// 邻接表
    int m_numVertices = 0;// 图顶点个数
    int m_count = 0;// 搜索的次序编号(时间戳)
    QVector<int> m_dfn; // 结点i搜索的次序编号(时间戳)  表示真实访问次序
    QVector<int> m_low; // 结点i或i的子结点能够追溯到的最早时间戳 通过邻接表以及子结点的dfn来实现更新
    QVector<bool> m_inStack; // 结点是否在栈内
    QStack<int> m_stack; // 用于回退结点
    QList<QList<int>> m_sccs;// strongly connected components 保存强连通分量

    void getSCC() {
        for (int i = 0; i < m_numVertices; ++i) {
            if (m_dfn[i] == -1) {
                DFS(i);
            }
        }
    }

    void DFS(int start) {
        m_dfn[start] = m_low[start] = ++m_count;
        m_stack.push(start);
        m_inStack[start] = true;
        // 邻接矩阵 https://www.bilibili.com/video/BV19J411J7AZ?p=5
/*        for (int i = 0; i < m_numVertices; ++i) {
            if (isConnected(start, i)) {
                if (m_dfn[i] == 0) {
                    DFS(i);
                    m_low[start] = m_low[start] > m_low[i] ? m_low[i] : m_low[start];
                } else if (m_stack.contains(i)) {
                    m_low[start] = m_low[start] > m_low[i] ? m_low[i] : m_low[start];
                }
            }
        }*/

        // 邻接表 https://segmentfault.com/a/1190000039149539
        for (int i = 0; i < m_connection[start].size(); ++i) {
            // 从邻接表选一个start可达的结点end
            int end = m_connection[start][i];

            // end结点没有被访问
            if (m_dfn[end] == -1) {
                // 递归调用
                DFS(end);
                // 更新能够到达的上层结点
                m_low[start] = m_low[start] > m_low[end] ? m_low[end] : m_low[start];
            } else if (m_inStack[end]) {// 在栈中且被访问过
                // 因为start能够抵达end 如果end的时间戳更小 则更新start的low
                m_low[start] = m_low[start] > m_dfn[end] ? m_dfn[end] : m_low[start];
            }
        }

        if (m_dfn[start] == m_low[start]) {
            //
            QList<int> strongComponent;// 一个强连通分支
            int temp = -1;
            while (start != temp) {
                temp = m_stack.pop();
                strongComponent.push_back(temp);
                m_inStack[temp] = false;
            }
            m_sccs.push_back(strongComponent);
        }
    }

};
