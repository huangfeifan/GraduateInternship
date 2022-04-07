//
// Created by Huangff on 2022/4/6.
//

#pragma once

#include <QList>
#include <QVector>
#include <QDebug>

#include "MyStruct.h"
#include "PlaceAlgo/FakeData.h"

class LineSearch {// 线探索法
    /**
     * 布线采用方法
     *      线探索
     * 线探索的难点在于回溯
     * 对于回溯的处理方式: 在没找到路径之前 不加入到障碍物中
     *                      被阻挡权重设为无穷大 则考虑从其余三个方向进行探索
     *                      探索结束后对路径进行简化处理
     *                      最后将处理后的路径加入到阻碍物中
     *
     * 通道内的数据都是基于网格的坐标 通道的index是坐标除以网格大小
     * */
public:

    LineSearch(const int &row, const int &column) {
        // 初始化行数列数
        m_rowChannel = QVector<QList<int>>(row);
        m_columnChannel = QVector<QList<int>>(column);

        QPoint start(30, 130);
        QPoint end(30, 100);
        QList<QPoint> path;
        bool findPath = routing(start, end, path);
        if (findPath) {
            qDebug() << path << "  Path";
        }
    }

    ~LineSearch() = default;

    bool routing(const QPoint &start, const QPoint &end, QList<QPoint> &path);// 根据起点和终点进行线探索

private:
    void routing(bool &isFindPath, const QPoint &start, const QPoint &end,
                 QList<QPoint> &path);

    void
    getPriorityDirections(QVector<Direction> &direction, const QPoint &start, const QPoint &end);// 根据起点和终点 计算探索方向的顺序

    void leftSearching(QPoint &start, const QPoint &end, QList<QPoint> &path);

    void rightSearching(QPoint &start, const QPoint &end, QList<QPoint> &path);

    void topSearching(QPoint &start, const QPoint &end, QList<QPoint> &path);

    void bottomSearching(QPoint &start, const QPoint &end, QList<QPoint> &path);

    bool removeIntersection(QList<QPoint> &path);// 去除自交

    QList<QPoint> simplifyPath(const QList<QPoint> &path);// 简化path

    void addPathToChannel(const QList<QPoint> &path);// 将一条路径添加到障碍物中

    void removePathFromChannel(const QList<QPoint> &path);// 从障碍物中移除一条路径

    void addModuleToChannel(const QPoint &pos, const QPoint &size);// 将模块添加到障碍物中

    void removeModuleFromChannel(const QPoint &pos, const QPoint &size);// 从障碍物中移除模块

    void addModulesToChannel(const QVector<QPoint> &poss, const QVector<QPoint> &sizes);// 将一组模块添加到障碍物中

    void removeModulesFromChannel(const QVector<QPoint> &poss, const QVector<QPoint> &sizes);// 从障碍物中移除一组模块

    void addLineToChannel(const QPoint &start, const QPoint &end);// 往通道添加线

    void addLineToChannel(QVector<QList<int>> &channel, const int &index, const int &start, const int &end);

    void addLineToRowChannel(const int &index, const int &start, const int &end);// 行通道添加线

    void addLineToColumnChannel(const int &index, const int &start, const int &end);// 列通道添加线

    // todo delete
    bool addLineToChannel(int minData, int maxData, int index, QVector<QList<int>> occupiedPos) {

        // 存放绝对坐标
        if (maxData <= minData) { // 数据异常
            qDebug() << "Error Type 0";
            return false;
        }

        int size = occupiedPos[index].size();
        if (size) {
            /// 列表不为空 则有三种插入情况
            // Type 1   插入到列表头部
            // Type 2   插入到列表尾部
            // Type 3   插入到列表中间

            if (maxData < occupiedPos[index][0]) { // Type 1 插入到列表头部
                occupiedPos[index].push_front(maxData);
                occupiedPos[index].push_front(minData);
                return true;
            }

            if (minData > occupiedPos[index][size - 1]) { // Type 2 插入到列表尾部
                occupiedPos[index].push_back(minData);
                occupiedPos[index].push_back(maxData);
                return true;
            }

            for (int k = 0; k < occupiedPos[index].size(); ++k) {// Type 3 插入到列表中间
                if (maxData < occupiedPos[index][k]) {
                    if (k % 2 == 0) {
                        if (minData > occupiedPos[index][k - 1]) {
                            qDebug() << "Error Type 1";
                            return false;
                        } else {
                            // 找到有效插入位置
                            occupiedPos[index].insert(k - 1, maxData);
                            occupiedPos[index].insert(k - 1, minData);
                            return true;
                        }
                    } else {
                        qDebug() << "Error Type 2";
                        return false;
                    }
                }
            }
        } else {
            /// 列表为空 则直接添加
            occupiedPos[index].push_back(minData);
            occupiedPos[index].push_back(maxData);
        }

        return false;
    }

private:

    QVector<QList<int>> m_rowChannel;// 行通道  从小到大的顺序存储
    QVector<QList<int>> m_columnChannel;// 列通道
};
