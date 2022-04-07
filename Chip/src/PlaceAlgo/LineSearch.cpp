//
// Created by Huangff on 2022/4/6.
//

#include "LineSearch.h"

bool LineSearch::routing(const QPoint &start, const QPoint &end, QList<QPoint> &path) {
    if (start == end) {
        qDebug() << " Data Error";
        return false;
    }
    path.push_back(start);// 起点加入列表

    bool isFindPath = false;// 找到路径与否

    routing(isFindPath, start, end, path);// 迭代地靠近目标

    return isFindPath;// true 表明探索成功  false表示探索失败
}

void LineSearch::routing(bool &isFindPath, const QPoint &start, const QPoint &end,
                         QList<QPoint> &path) {
    if (start == end) {// 起点和终点相同 则探索结束
        isFindPath = true;
        return;
    }

    // 判断探索方向
    const int directionCount = 4;// 有4种探索方向
    QVector<Direction> direction(directionCount);
    getPriorityDirections(direction, start, end);
    QPoint point = start;// 备份

    // 对四个方向进行探索 如果point修改了 说明前进了 否则考虑另外一个方向 todo modify
    for (int i = 0; i < directionCount; ++i) {
        switch (direction[i]) {
            case Direction::LEFT:     // left
                leftSearching(point, end, path);// core
                break;
            case Direction::RIGHT :   // right
                rightSearching(point, end, path);
                break;
            case Direction::TOP:      // top
                topSearching(point, end, path);
                break;
            case Direction::BOTTOM:   // bottom
                bottomSearching(point, end, path);
                break;
            default:
                qDebug() << "Error!";
                break;
        }
        if (point == end) {// 成功探索
            isFindPath = true;
            return;
        }

        if (start == point) {// 探索的某一个方向被完全阻碍
            continue;
        }
        //getPriorityDirections(direction, point, end);// 判断探索方向
        routing(isFindPath, point, end, path);
        if (isFindPath) {// 路径成功找到 退出循环
            break;
        }
    }
}

void LineSearch::getPriorityDirections(QVector<Direction> &direction, const QPoint &start, const QPoint &end) {
    // todo consider improve performance ??

    if (start.x() == end.x()) {// 在同一列 X相同
        if (start.y() > end.y()) {// 起点在下方 先考虑向上探索
            direction[0] = Direction::TOP;// 先考虑向上探索
            direction[1] = Direction::BOTTOM;
        } else {
            direction[1] = Direction::TOP;
            direction[0] = Direction::BOTTOM;// 先考虑向下探索
        }
        direction[2] = Direction::RIGHT;
        direction[3] = Direction::LEFT;
        return;
    }

    if (start.y() == end.y()) {// 在同一行 Y相同
        if (start.x() > end.x()) {// 起点在右侧
            direction[1] = Direction::RIGHT;
            direction[0] = Direction::LEFT;// 先考虑向左探索
        } else {
            direction[0] = Direction::RIGHT;// 先考虑向右探索
            direction[1] = Direction::LEFT;
        }
        direction[2] = Direction::TOP;
        direction[3] = Direction::BOTTOM;
        return;
    }

    /// 起点和终点的X和Y都不相同 目前考虑先左右探索 后上下探索  todo consider
    // 根据高度差和水平差预计待途径长度的比例来考虑 先上下还是先左右
    // 起点在终点右上方
    direction[0] = Direction::LEFT;// 左
    direction[2] = Direction::RIGHT;// 右
    direction[1] = Direction::BOTTOM;// 下
    direction[3] = Direction::TOP;// 上

    if (start.x() < end.x()) {// 起点在终点左侧
        direction[0] = Direction::RIGHT;
        direction[2] = Direction::LEFT;
    }
    if (start.y() > end.y()) {// 起点在终点下方
        direction[1] = Direction::TOP;
        direction[3] = Direction::BOTTOM;
    }
}

void LineSearch::leftSearching(QPoint &start, const QPoint &end, QList<QPoint> &path) {
    // 向左探索 能一步到位最好 不能则探索到*** todo consider
    // m_rowChannel
    int index = start.y() / GRID;// 在哪个行通道上  todo check index validity index>=0&&index<row
    auto list = m_rowChannel[index];

    // 特殊情况 列表为空 则直接插入
    if (list.size() == 0) {
        start.setX(end.x());// 向左探索 x修改
        path.push_back(start);// update point
        return;
    }

    // 列表不为空 则有三种插入情况 表头 表尾 中间
    int min_x = end.x();
    int max_x = start.x();
    if (max_x < list[0] - GRID) {// 表头
        start.setX(end.x());// 向左探索 x修改
        path.push_back(start);// update point
        return;
    }
    if (min_x > list[list.size() - 1] + GRID) {// 表尾
        start.setX(end.x());// 向左探索 x修改
        path.push_back(start);// update point
        return;
    }
    // 列表中间
    for (int i = 0; i < list.size(); ++i) {
        if (i % 2 == 1) {// 奇数
            if (max_x < list[i + 1] - GRID) {// 找到插入位置
                if (min_x > list[i] + GRID) {// 能一步到位
                    start.setX(end.x());// 向左探索 x修改
                } else {
                    start.setX(list[i] + 1 * GRID);// 向左探索到尽头 todo consider
                }
                path.push_back(start);// update point
                return;
            }
        }
    }
}

void LineSearch::rightSearching(QPoint &start, const QPoint &end, QList<QPoint> &path) {
    // 向右探索 能一步到位最好 不能则探索到*** todo consider
    // m_rowChannel
    int index = start.y() / GRID;
    auto list = m_rowChannel[index];
    if (list.size() == 0) {
        start.setX(end.x());
        path.push_back(start);
        return;
    }

    int min_x = start.x();
    int max_x = end.x();
    // 表头或表尾可以插入
    if (max_x < list[0] - GRID || min_x > list[list.size() - 1] + GRID) {
        start.setX(end.x());
        path.push_back(start);
        return;
    }

    // 插入到列表中间
    for (int i = 0; i < list.size(); ++i) {
        if (i % 2 == 1) {// 奇数
            if (min_x > list[i] + GRID) {// 找到插入位置
                if (max_x < list[i + 1] - GRID) {// 能一步到位
                    start.setX(end.x());
                } else {
                    start.setX(list[i] + GRID * 1);
                }
                path.push_back(start);
                return;
            }
        }
    }
}

void LineSearch::topSearching(QPoint &start, const QPoint &end, QList<QPoint> &path) {
    // 向上探索 能一步到位最好 不能则探索到*** todo consider
    // m_columnChannel
    int index = start.x() / GRID;
    auto list = m_columnChannel[index];
    if (list.size() == 0) {// 一步到位
        start.setY(end.y());
        path.push_back(start);
        return;
    }

    int min_y = end.y();
    int max_y = start.y();
    if (max_y < list[0] - GRID || min_y > list[list.size() - 1] + GRID) {// 一步到位
        start.setY(end.y());
        path.push_back(start);
        return;
    }
    // 插入到列表中间
    for (int i = 0; i < list.size(); ++i) {
        if (i % 2 == 1) {// 奇数
            if (max_y < list[i + 1] - GRID) {
                if (min_y > list[i] + GRID) {
                    start.setY(end.y());
                } else {
                    start.setY(list[i] + GRID * 1);
                }
                path.push_back(start);
                return;
            }
        }
    }
}

void LineSearch::bottomSearching(QPoint &start, const QPoint &end, QList<QPoint> &path) {
    // 向下探索 能一步到位最好 不能则探索到*** todo consider
    // m_columnChannel
    int index = start.x() / GRID;
    auto list = m_columnChannel[index];
    if (list.size() == 0) {// 一步到位
        start.setY(end.y());
        path.push_back(start);
        return;
    }

    int max_y = end.y();
    int min_y = start.y();
    if (max_y < list[0] - GRID || min_y > list[list.size() - 1] + GRID) {// 一步到位
        start.setY(end.y());
        path.push_back(start);
        return;
    }
    // 插入到列表中间
    for (int i = 0; i < list.size(); ++i) {
        if (i % 2 == 1) {// 奇数
            if (min_y > list[i] + GRID) {
                if (max_y < list[i + 1] - GRID) {
                    start.setY(end.y());
                } else {
                    start.setY(list[i] + GRID * 1);
                }
                path.push_back(start);
                return;
            }
        }
    }
}

bool LineSearch::removeIntersection(QList<QPoint> &path) {
    // path可能出现自交 path的自交可以通过去除一些结点来解决
    // todo add
    return false;
}

QList<QPoint> LineSearch::simplifyPath(const QList<QPoint> &path) {
    // path可能出现三点共线 三点共线可以通过去除中间结点的方式来解决
    // todo add
    return QList<QPoint>();
}

void LineSearch::addPathToChannel(const QList<QPoint> &path) {
    // path中数据都是原始数据 不是基于网格的坐标
    for (int i = 1; i < path.size(); ++i) {
        QPoint start = path[i - 1];
        QPoint end = path[i];
        addLineToChannel(start, end);
    }
}

void LineSearch::addModuleToChannel(const QPoint &pos, const QPoint &size) {
    // m_columnChannel  要求所有数据都是网格的整数倍
    for (int i = 0; i <= size.x() / GRID; ++i) {
        int start = pos.y();
        int end = pos.y() + size.y();// x = pos.x()+i*GRID
        int index = pos.x() / GRID + i;
        addLineToChannel(m_columnChannel, index, start, end);
    }

    // m_rowChannel
    for (int i = 0; i <= size.y() / GRID; ++i) {
        int start = pos.x();
        int end = pos.x() + size.x();// y = pos.y() + i*GRID
        int index = pos.y() / GRID + i;
        addLineToChannel(m_rowChannel, index, start, end);
    }
}

void LineSearch::addModulesToChannel(const QVector<QPoint> &poss, const QVector<QPoint> &sizes) {
    if (poss.size() != sizes.size()) {
        qDebug() << "AddModulesToChannel";
        return;
    }

    for (int i = 0; i < poss.size(); ++i) {
        addModuleToChannel(poss[i], sizes[i]);
    }
}

void LineSearch::removePathFromChannel(const QList<QPoint> &path) {
    // todo consider 一驱多
    // todo add

}

void LineSearch::removeModuleFromChannel(const QPoint &pos, const QPoint &size) {
    // todo add

}

void LineSearch::removeModulesFromChannel(const QVector<QPoint> &poss, const QVector<QPoint> &sizes) {
    if (poss.size() != sizes.size()) {
        qDebug() << "RemoveModulesFromChannel";
        return;
    }

    for (int i = 0; i < poss.size(); ++i) {
        removeModuleFromChannel(poss[i], sizes[i]);
    }
}

void LineSearch::addLineToChannel(QVector<QList<int>> &channel, const int &index, const int &start, const int &end) {
    auto list = channel[index];
    if (start == end) {
        qDebug() << " Error Start == End";
        return;
    }

    int max = end;
    int min = start;
    if (start > end) {
        max = start;
        min = end;
    }

    int size = list.size();
    if (size == 0) {
        list.push_back(min);
        list.push_back(max);
        return;
    }

    // size !=0  列表不为空 则存在头部 中间 尾部三种插入方式
    if (max < list[0]) {// 头部
        list.push_back(min);
        list.push_back(max);
        return;
    }

    if (min > list[size - 1]) {// 尾部
        list.push_back(min);
        list.push_back(max);
        return;
    }

    // 中间
    for (int i = 0; i < size; ++i) {
        if (i % 2 == 1) {// 奇数
            if (min > list[i] && max < list[i + 1]) {
                // 找到合适位置
                list.insert(i, min);
                list.insert(i + 1, max);
                return;
            }
        }
    }
}

void LineSearch::addLineToChannel(const QPoint &start, const QPoint &end) {
    if (start.x() == end.x()) {// 列通道
        int index = start.x() / GRID;
        int start_y = start.y();
        int end_y = end.y();
        addLineToColumnChannel(index, start_y, end_y);
        return;
    }

    if (start.y() == end.y()) {// 行通道
        int index = start.y() / GRID;
        int start_x = start.x();
        int end_x = end.x();
        addLineToRowChannel(index, start_x, end_x);
        return;
    }
    qDebug() << "Error AddLineToChannel ";
    return;
}

void LineSearch::addLineToRowChannel(const int &index, const int &start, const int &end) {
    // 水平直线添加到行通道中  m_rowChannel
    addLineToChannel(m_rowChannel, index, start, end);
}

void LineSearch::addLineToColumnChannel(const int &index, const int &start, const int &end) {
    // 竖直直线添加到列通道中  m_columnChannel
    addLineToChannel(m_columnChannel, index, start, end);
}
