//
// Created by Huangff on 2022/4/11.
//

#pragma once

#include <QList>
#include <QPoint>
#include <QDebug>

#include "FakeData.h"

struct PointInfo {
    int point;
    int distance;
};


struct LineInfo {
    QPoint start;
    QPoint end;
    bool isHorizontal;
};

class MikamiTabuchiAlgo {
    /// Attention
    // row column
    // x [0,column)
    // y [0,row)
    // all position based on GRID
public:

    MikamiTabuchiAlgo() = default;

    ~MikamiTabuchiAlgo() = default;

    MikamiTabuchiAlgo(const int &x, const int &y) {// 行通道数x 列通道数y
        // 初始化行数列数
        m_rowChannel = QVector<QList<int >>(x);
        m_columnChannel = QVector<QList<int >>(y);
        m_row = x;
        m_column = y;
        //qDebug() << m_row << "  ----  " << m_column;
    }

    int m_row;
    int m_column;


    static bool cmpDistance(const PointInfo &P, const PointInfo &Q) {
        /// 按照 distance 从小到大排序
        if (P.distance < Q.distance) {
            return true;
        } else {
            return false;
        }
    }

    QList<QPoint> routing(QPoint start, QPoint end) {
        if (start == end) {
            QList<QPoint> path;
            return path;
        }

        // 0级轨道
        QVector<LineInfo> endZero(2);
        endZero[0] = computeHorizontalLine(end);
        endZero[1] = computeVerticalLine(end);

        QList<QList<LineInfo>> startList;
        startList.push_back(QList<LineInfo>());// 增加0级轨道
        startList[0].push_back(computeHorizontalLine(start));
        startList[0].push_back(computeVerticalLine(start));

        if (endZero[0].start == endZero[0].end && endZero[1].start == endZero[1].end) {
            qDebug() << "end point is blocked!";
            return QList<QPoint>();
        }
        if (startList[0][1].start == startList[0][0].end && startList[0][1].start == startList[0][1].end) {
            qDebug() << "start point is blocked!";
            return QList<QPoint>();
        }

        QHash<int, int> rowOccupy;// y
        QHash<int, int> columnOccupy;// x 不是基于格点的坐标 而是实际坐标
        rowOccupy.insert(start.y(), start.y());
        columnOccupy.insert(start.x(), start.x());

        QList<QPoint> path;// QVector<int>(iterateCount+3)  todo consider
        path.push_back(end);

        // 判断是否相交
        int iterateCount = 0;// 是否需要避免死循环 即限定迭代次数
        int index = -1;
        // 判断两个零级轨道是否相交
        if (index == -1) {
            if (isIntersected(startList[0][1].start, startList[0][1].end, endZero[0].start, endZero[0].end)) {
                path.push_front(QPoint(startList[0][1].start.x(), endZero[0].start.y()));
                path.push_front(start);
                return path;
            }

            if (isIntersected(endZero[1].start, endZero[1].end, startList[0][0].start, startList[0][0].end)) {
                path.push_front(QPoint(endZero[1].start.x(), startList[0][0].start.y()));
                path.push_front(start);
                return path;
            }
        }
        while (index == -1 && startList[iterateCount].size()) {// 迭代     // 起点开始更新轨道
            startList.push_back(QList<LineInfo>());// 增加轨道
            iterateCount++;
            bool isIntersect = false;
            for (int i = 0; i < startList[iterateCount - 1].size(); ++i) {
                LineInfo lineInfo = startList[iterateCount - 1][i];
                //qDebug() << lineInfo.start << " point" << lineInfo.end << "_________\n";
                if (lineInfo.isHorizontal) {
                    int left = lineInfo.start.x();// start在end的左侧
                    int right = lineInfo.end.x();
                    int temp = left;
                    int y = lineInfo.start.y();
                    while (temp <= right && temp < m_column) {
                        if (!columnOccupy.contains(temp)) {   // 增加轨道 // to do delete !rowOccupy.contains(y) ||
                            LineInfo tempLine = computeVerticalLine(QPoint(temp, y));
                            //qDebug() << tempLine.start << " point" << tempLine.end << " Vertical ";
                            if (tempLine.start != tempLine.end) {// 找到有效的line
                                startList[iterateCount].push_back(tempLine);
                                // update hash
                                columnOccupy.insert(temp, temp);

                                if (isIntersected(tempLine.start, tempLine.end, endZero[0].start,
                                                  endZero[0].end)) {
                                    path.push_front(QPoint(tempLine.start.x(), endZero[0].start.y()));
                                    isIntersect = true;
                                    break;// while
                                }
                            }
                        }
                        temp += 1;
                    }
                } else {
                    int up = lineInfo.start.y();// start在end的上方
                    int down = lineInfo.end.y();
                    int temp = up;
                    int x = lineInfo.start.x();
                    while (temp <= down && temp < m_row) {
                        if (!rowOccupy.contains(temp)) {//
                            // 增加轨道
                            LineInfo tempLine = computeHorizontalLine(QPoint(x, temp));
                            //qDebug() << tempLine.start << " point" << tempLine.end << " Horizontal ";

                            if (tempLine.start != tempLine.end) {// 找到有效的line
                                startList[iterateCount].push_back(tempLine);
                                // hash
                                rowOccupy.insert(temp, temp);
                                if (isIntersected(endZero[1].start, endZero[1].end, tempLine.start,
                                                  tempLine.end)) {
                                    path.push_front(QPoint(endZero[1].start.x(), tempLine.start.y()));
                                    isIntersect = true;
                                    break;// while
                                }
                            }
                        }
                        temp += 1;
                    }
                }
                if (isIntersect) {
                    break;// for
                }
            }
            //qDebug() << iterateCount << " iterate | size " << startList[iterateCount].size();
            if (isIntersect) {
                break;// while
            }
        }

        index = startList[iterateCount].size() - 1;
        //qDebug() << index << " Index";
        if (index == -1) {
            // 被阻挡
            QList<QPoint> path;
            return path;
        }

        //qDebug() << path << " Path----\n\n";
        // 计算path
        bool findPath = false;
        while (true) {
            if (computePath(path, index, iterateCount, startList)) {
                break;
            }
        }

        path.push_front(start);// 最终得到的list是从起点开始的bend最少的路径

        return path;
    };

    QList<QPoint> routingSort(QPoint start, QPoint end) {
        if (start == end) {
            return QList<QPoint>();
        }

        //qDebug() << m_row << " <--row  col--> " << m_column;

        // 0级轨道
        QVector<LineInfo> endZero(2);
        endZero[0] = computeHorizontalLine(end);
        endZero[1] = computeVerticalLine(end);
        //qDebug() << endZero[0].start << " point" << endZero[0].end << " Horizontal ";
        //qDebug() << endZero[1].start << " point" << endZero[1].end << " Vertical ";

        QList<QList<LineInfo>> startList;
        startList.push_back(QList<LineInfo>());// 增加0级轨道
        startList[0].push_back(computeHorizontalLine(start));
        startList[0].push_back(computeVerticalLine(start));
        //qDebug() << startList[0][0].start << " point" << startList[0][0].end << " Horizontal ";
        //qDebug() << startList[0][1].start << " point" << startList[0][1].end << " Vertical ";

        if (endZero[0].start == endZero[0].end && endZero[1].start == endZero[1].end) {
            qDebug() << "end point is blocked!";
            return QList<QPoint>();
        }
        if (startList[0][1].start == startList[0][0].end && startList[0][1].start == startList[0][1].end) {
            qDebug() << "start point is blocked!";
            return QList<QPoint>();
        }

        QHash<int, int> rowOccupy;// y
        QHash<int, int> columnOccupy;// x 不是基于格点的坐标 而是实际坐标
        rowOccupy.insert(start.y(), start.y());
        columnOccupy.insert(start.x(), start.x());

        QList<QPoint> path;// QVector<int>(iterateCount+3)  todo consider
        path.push_back(end);

        // 判断是否相交
        int iterateCount = 0;// 是否需要避免死循环 即限定迭代次数
        int index = -1;
        // 判断两个零级轨道是否相交
        if (index == -1) {

            if (isIntersected(startList[0][1].start, startList[0][1].end, endZero[0].start, endZero[0].end)) {
                path.push_front(QPoint(startList[0][1].start.x(), endZero[0].start.y()));
                path.push_front(start);
                return path;
            }

            if (isIntersected(endZero[1].start, endZero[1].end, startList[0][0].start, startList[0][0].end)) {
                path.push_front(QPoint(endZero[1].start.x(), startList[0][0].start.y()));
                path.push_front(start);
                return path;
            }
        }
        while (index == -1 && startList[iterateCount].size()) {// 迭代     // 起点开始更新轨道
            startList.push_back(QList<LineInfo>());// 增加轨道
            iterateCount++;
            bool isIntersect = false;
            for (int i = 0; i < startList[iterateCount - 1].size(); ++i) {
                LineInfo lineInfo = startList[iterateCount - 1][i];
                if (lineInfo.isHorizontal) {
                    /// 水平线上的点 从左往右 ——> 按距离遍历
                    int left = lineInfo.start.x();// start在end的左侧
                    int right = lineInfo.end.x();
                    //int temp = left;
                    int y = lineInfo.start.y();

                    QVector<PointInfo> points(right - left + 1);
                    for (int j = 0; j < points.size(); ++j) {
                        points[j].point = left + j;
                        points[j].distance = qAbs(end.x() - points[j].point);
                    }

                    qSort(points.begin(), points.end(), cmpDistance);
                    for (int j = 0; j < points.size(); ++j) {
                        if (!columnOccupy.contains(points[j].point)) {
                            LineInfo tempLine = computeVerticalLine(QPoint(points[j].point, y));
                            //qDebug() << tempLine.start << " point" << tempLine.end << " Vertical ";
                            if (tempLine.start != tempLine.end) {// 找到有效的line
                                startList[iterateCount].push_back(tempLine);
                                // update hash
                                columnOccupy.insert(points[j].point, points[j].point);

                                if (isIntersected(tempLine.start, tempLine.end, endZero[0].start, endZero[0].end)) {
                                    path.push_front(QPoint(tempLine.start.x(), endZero[0].start.y()));
                                    isIntersect = true;
                                    break;// for
                                }
                            }
                        }
                    }

                } else {
                    int up = lineInfo.start.y();// start在end的上方
                    int down = lineInfo.end.y();
                    //int temp = up;
                    int x = lineInfo.start.x();

                    QVector<PointInfo> points(down - up + 1);
                    for (int j = 0; j < points.size(); ++j) {
                        points[j].point = up + j;
                        points[j].distance = qAbs(end.y() - points[j].point);
                    }
                    qSort(points.begin(), points.end(), cmpDistance);

                    for (int j = 0; j < points.size(); ++j) {
                        if (!rowOccupy.contains(points[j].point)) {
                            // 增加轨道
                            LineInfo tempLine = computeHorizontalLine(QPoint(x, points[j].point));
                            //qDebug() << tempLine.start << " point" << tempLine.end << " Horizontal ";

                            if (tempLine.start != tempLine.end) {// 找到有效的line
                                startList[iterateCount].push_back(tempLine);
                                // hash
                                rowOccupy.insert(points[j].point, points[j].point);
                                if (isIntersected(endZero[1].start, endZero[1].end, tempLine.start, tempLine.end)) {
                                    path.push_front(QPoint(endZero[1].start.x(), tempLine.start.y()));
                                    isIntersect = true;
                                    break;// while
                                }
                            }
                        }
                    }
                }
                if (isIntersect) {
                    break;// for
                }
            }
            //qDebug() << iterateCount << " iterate | size " << startList[iterateCount].size();
            if (isIntersect) {
                break;// while
            }
        }

        index = startList[iterateCount].size() - 1;
        //qDebug() << index << " Index";
        if (index == -1) {
            // 被阻挡
            QList<QPoint> path;
            return path;
        }

        //qDebug() << path << " Path----\n\n";
        // 计算path
        bool findPath = false;
        while (true) {
            if (computePath(path, index, iterateCount, startList)) {
                break;
            }
        }

        path.push_front(start);// 最终得到的list是从起点开始的bend最少的路径

        return path;
    }

    bool isIntersected(const QPoint &up, const QPoint &down, const QPoint &left, const QPoint &right) {
        bool isIntersected = false;
        if (up.x() >= left.x() && up.x() <= right.x() && left.y() >= up.y() &&
            left.y() <= down.y()) {
            isIntersected = true;
        }
        return isIntersected;
    }

    bool computePath(QList<QPoint> &path, int index, int iterateCount, QList<QList<LineInfo>> startList) {

        LineInfo lineInfo = startList[iterateCount][index];// S的最后一级轨道

        bool isHorizontal = lineInfo.isHorizontal;//
        //qDebug() << iterateCount << " iterateCount";
        //qDebug() << lineInfo.start << " point" << lineInfo.end << "  " << isHorizontal;
        iterateCount--;
        if (iterateCount < 0) {
            return true;
        }
        for (int i = 0; i < startList[iterateCount].size(); ++i) {
            LineInfo nextLine = startList[iterateCount][i];
            //qDebug() << nextLine.start << " point" << nextLine.end << "  " << nextLine.isHorizontal;
            if (nextLine.isHorizontal == isHorizontal) {// 平行
                continue;
            }

            QPoint left = nextLine.start;
            QPoint right = nextLine.end;
            QPoint up = lineInfo.start;// 默认 index所对应的line是竖直的
            QPoint down = lineInfo.end;

            if (isHorizontal) {// 若index所对应的line是水平的
                left = lineInfo.start;
                right = lineInfo.end;
                up = nextLine.start;
                down = nextLine.end;
            }
            if (isIntersected(up, down, left, right)) {
                path.push_front(QPoint(up.x(), left.y()));
                //qDebug() << path << " Path";
                if (computePath(path, i, iterateCount, startList)) {
                    return true;
                }
            }
        }

        return false;
    }

    int isIntersected(const QList<LineInfo> &list, const QVector<LineInfo> &endZero) {
        for (int i = 0; i < list.size(); ++i) {
            // 默认是竖直的线
            QPoint left = endZero[0].end;
            QPoint right = endZero[0].start;// 计算line的时候都是 start<end
            QPoint up = list[i].start;
            QPoint down = list[i].end;

            if (list[i].isHorizontal) {// 水平 start和end的Y坐标相同
                up = endZero[1].start;
                down = endZero[1].end;
                left = list[i].start;
                right = list[i].end;
            }

            if (left.x() <= up.x() && up.x() <= right.x() && left.y() >= up.y() && left.y() <= down.y()) {// 判断是否相交
                return i;
            }
        }
        return -1;
    }

    LineInfo computeLine(QPoint point, bool isHorizontal) {// 给一个点 计算line
        LineInfo lineInfo = isHorizontal ? computeHorizontalLine(point) : computeVerticalLine(point);
        return lineInfo;
    }

    LineInfo computeHorizontalLine(QPoint point) {
        // x [0,column)
        // y [0,row)
        LineInfo lineInfo;
        lineInfo.end = point;
        lineInfo.start = point;
        lineInfo.isHorizontal = true;

        if (0 > point.x() || point.x() >= m_column || 0 > point.y() || point.y() >= m_row) {
            qDebug() << "Error___index_out_of_range " << point;
            return lineInfo;
        }

        // 探索成功 则修改线        m_rowChannel
        int target = point.x();
        //qDebug() << point << " ChannelSize  " << m_rowChannel.size() << "  column " << point.y();
        //if (m_rowChannel.size() <= point.y()) {
        //    qDebug() << "Error---HLine";
        //}
        auto channel = m_rowChannel[point.y()];// 越界检查没做 todo add

        if (channel.empty()) {// 特殊情况的处理 通道为空
            lineInfo.start = QPoint(0, point.y());
            lineInfo.end = QPoint(m_column - 1, point.y());
            return lineInfo;
        }

        int index = getIndex(target, channel);// channel[index]严格大于target

        if (index == 0) {// 头部
            lineInfo.start = QPoint(0, point.y());
            lineInfo.end = QPoint(channel[index] - 1, point.y());
            return lineInfo;
        }
        if (index % 2 == 1 || channel[index - 1] == target) {// 被阻塞
            return lineInfo;
        }

        // 合适的插入位置
        if (index == channel.size()) {// 尾部
            lineInfo.start = QPoint(channel[channel.size() - 1] + 1, point.y());
            lineInfo.end = QPoint(m_column - 1, point.y());
        } else {// 中间
            lineInfo.start = QPoint(channel[index - 1] + 1, point.y());
            lineInfo.end = QPoint(channel[index] - 1, point.y());
        }
        return lineInfo;
    }

    LineInfo computeVerticalLine(QPoint point) {
        LineInfo lineInfo;
        lineInfo.end = point;
        lineInfo.start = point;
        lineInfo.isHorizontal = false;

        if (0 > point.x() || point.x() >= m_column || 0 > point.y() || point.y() >= m_row) {// 越界检查
            qDebug() << "Error___index_out_of_range_HLine " << point;
            return lineInfo;
        }

        // 探索成功 则修改线        m_columnChannel     列通道 x相同
        int target = point.y();
        //qDebug() << point << " ChannelSize  " << m_columnChannel.size() << "  column " << point.x();
        //if (m_columnChannel.size() <= point.x()) {
        //    qDebug() << "Error---VLine";
        //}
        auto channel = m_columnChannel[point.x()];// 越界检查没做 todo add

        if (channel.size() == 0) {// 特殊情况的处理 通道为空
            lineInfo.start = QPoint(point.x(), 0);
            lineInfo.end = QPoint(point.x(), m_row - 1);
            return lineInfo;
        }

        int index = getIndex(target, channel);

        if (index == 0) {// 头部
            lineInfo.start = QPoint(point.x(), 0);
            lineInfo.end = QPoint(point.x(), channel[index] - 1);
            return lineInfo;
        }

        if (index % 2 == 1 || channel[index - 1] == target) {// 被阻塞
            return lineInfo;
        }

        // 合适的插入位置
        if (index == channel.size()) {// 尾部
            lineInfo.start = QPoint(point.x(), channel[channel.size() - 1] + 1);
            lineInfo.end = QPoint(point.x(), m_row - 1);
        } else {// 中间
            lineInfo.start = QPoint(point.x(), channel[index - 1] + 1);
            lineInfo.end = QPoint(point.x(), channel[index] - 1);
        }
        return lineInfo;
    }

    int getIndex(int target, QList<int> channel) {// index的数字大于target

        // 有序数组 二分法 找插入位置  从小到大排序
        int low = 0;
        int high = channel.size() - 1;
        while (low <= high) {
            int mid = (high - low) / 2 + low;
            int num = channel[mid];
            if (num > target) {
                high = mid - 1;
            } else {
                low = mid + 1;
            }
        }
        return low;
    }

    void addLineToChannel(LineInfo lineInfo) {

        if (lineInfo.isHorizontal) {
            QPoint left = lineInfo.start;
            QPoint right = lineInfo.end;
            int row = left.y();

        } else {
            QPoint up = lineInfo.start;
            QPoint down = lineInfo.end;
            int column = up.x();
        }
    }

    void addHLineToChannel(int left, int right, int row) {// left < right
        auto channel = m_rowChannel[row];
        if (channel.empty()) {
            m_rowChannel[row].push_back(left);
            m_rowChannel[row].push_back(right);
            return;
        }

        int index = getIndex(right, channel);
        m_rowChannel[row].insert(index, right);
        m_rowChannel[row].insert(index, left);
    }

    void addVLineToChannel(int up, int down, int column) { // up < down
        auto channel = m_columnChannel[column];
        if (channel.empty()) {// 特殊情况 通道未空
            m_columnChannel[column].push_back(up);
            m_columnChannel[column].push_back(down);
            return;
        }
        int index = getIndex(down, channel);// index是channel中第一个比down大的数字对应的下标减一
        m_columnChannel[column].insert(index, down);
        m_columnChannel[column].insert(index, up);
    }

    void removeHLineFromChannel(int left, int right, int row) {// todo add
        auto channel = m_rowChannel[row];
        int index = getIndex(right, channel) - 2;
        m_rowChannel[row].removeAt(index);
        m_rowChannel[row].removeAt(index);
    }

    void removeVLineFromChannel(int up, int down, int column) {// todo add
        auto channel = m_columnChannel[column];
        int index = getIndex(down, channel) - 2;
        m_columnChannel[column].removeAt(index);
        m_columnChannel[column].removeAt(index);
    }

    void addPathToChannel(const QList<QPoint> &path) {
        for (int i = 1; i < path.size(); ++i) {
            QPoint start = path[i - 1];
            QPoint end = path[i];
            if (start.x() == end.x()) {
                // add vertical line to columnChannel
                int up = start.y();
                int down = end.y();
                if (up > down) {
                    down = start.y();
                    up = end.y();
                }
                addVLineToChannel(up, down, start.x());
            } else {
                // add horizontal line to rowChannel
                int left = start.x();
                int right = end.x();
                if (left > right) {
                    left = end.x();
                    right = start.x();
                }
                addHLineToChannel(left, right, start.y());
            }
        }
    }

    void addModuleToChannel(const QPoint &pos, const QPoint &size) {
        // 添加行
        for (int i = 0; i <= size.y(); ++i) {
            int index = pos.y() + i;
            addHLineToChannel(pos.x(), pos.x() + size.x(), index);
        }

        // 添加列
        for (int i = 0; i <= size.x(); ++i) {
            int index = pos.x() + i;
            addVLineToChannel(pos.y(), pos.y() + size.y(), index);
        }
    }

    void addModulesToChannel(const QList<QPoint> &pos, const QList<QPoint> &size) {
        for (int i = 0; i < pos.size(); ++i) {
            addModuleToChannel(pos[i], size[i]);
        }
    }

    void addModulesToChannel(const QVector<QPoint> &pos, const QVector<QPoint> &size) {
        for (int i = 0; i < pos.size(); ++i) {
            addModuleToChannel(pos[i], size[i]);
        }
    }

    void removePathFromChannel(const QList<QPoint> &path) {
        // todo something
    }

    QVector<QList<int>> m_rowChannel;// 行通道  从小到大的顺序存储
    QVector<QList<int>> m_columnChannel;// 列通道
private:

};
