//
// Created by Huangff on 2022/1/26.
//

#pragma once

#include <QWidget>
#include <QDebug>
#include <QStack>

// DAG :=  Directed Acyclic Graph
// 有向无环图的拓扑排序  用Tarjan算法遍历一次图  拓扑排序可以确定哪些结点在同一列
class GetTopologySort {

public:
    GetTopologySort(QVector<QList<int>> graphData) :m_connection(graphData){

        //m_connection = graphData;
        //qDebug() << "\nGetTopologySort---------";
        // 检查是否有环 Tarjan_algorithm 需要参数  m_connection
        initConnect();
        computeResult();
        detectVerticesNum();
        //qDebug() << "GetTopologySort---------\n";
        //initParent();
    }

    // 拓扑排序结果
    QList<QStack<int>> getSortResult() {
        return m_result;
    };
    QList<QStack<int>> m_result;// 拓扑排序结果

private:
    QVector<QList<int>> m_connection;//有向无环图的邻接表
    QList<int> m_inDegree;//入度
    //QList<int> m_outDegree;//出度
    QStack<int> m_stack;
    QStack<int> m_qStack;

    bool isDAG = true;//是有向无环图 DAG :=  Directed Acyclic Graph

    // 初始化connect数据
    void initConnect() {
        // 有向无环图 DAG directed acyclic graph
        for (int i = 0; i < m_connection.size(); ++i) {
            m_inDegree.push_back(0);
        }
        // 初始化入度
        for (int i = 0; i < m_connection.size(); ++i) {
            for (int j = 0; j < m_connection[i].size(); ++j) {
                m_inDegree[m_connection[i][j]]++;
            }
        }
    }

    void computeResult() {
        /// 有向图的入度为0的
        // TODO optimize 扫一遍入度为0 入队列  while(队列非空) 队列pop 子节点入度减一 若子节点入度为0则放在队列最后 增加辅助信息
        for (int i = 0; i < m_connection.size(); ++i) {
            //QList<int> temp;
            if (m_inDegree[i] == 0) {
                m_qStack.push_back(i);
            }
        }
        //qDebug() << m_qStack;
        m_result.push_back(m_qStack);

        //qDebug() << "while----";
        while (!m_qStack.isEmpty()) {
            int index = m_qStack.pop();
            //qDebug() << m_qStack;
            for (int i = 0; i < m_connection[index].size(); ++i) {
                int temp = m_connection[index][i];
                m_inDegree[temp]--;
                if (m_inDegree[temp] == 0) {
                    m_stack.push_back(temp);
                }
            }

            // 更新qStack
            if (m_qStack.isEmpty()) {
                m_qStack = m_stack;
                //qDebug() << m_qStack << "-------------";
                if (!m_qStack.isEmpty()) {
                    m_result.push_back(m_qStack);
                }
                m_stack.clear();
            }
        }
        //qDebug() << "while----end";
        //qDebug() << "****    Topology sort m_result:\n" << m_result;
    }

    void detectVerticesNum() {
        // 拓扑排序的顶点数
        int sortVerticesNum = 0;
        for (int i = 0; i < m_result.size(); ++i) {
            sortVerticesNum += m_result[i].size();
        }

        // 拓扑排序顶点数不等于有向图的顶点数  说明有向图有环
        if (sortVerticesNum != m_connection.size()) {
            isDAG = false;
            //qDebug() << " has circuit";
        } else {
            //qDebug() << " has not circuit";
        }
    }

    // 根据邻接表计算每个顶点的父节点
    void initParent() {
        QList<QList<int>> parent;//

        for (int i = 0; i < m_connection.size(); ++i) {
            QList<int> temp;
            parent.push_back(temp);
        }

        for (int i = 0; i < m_connection.size(); ++i) {
            for (int j = 0; j < m_connection[i].size(); ++j) {
                int end = m_connection[i][j];
                parent[end].push_back(i);
            }
        }
        //qDebug() << parent << " IndexParent";
    }

};
