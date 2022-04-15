//
// Created by Huangff on 2022/4/6.
//

#include "LineSearch.h"

const int STEP = 1;

bool LineSearch::routing(QPoint &start, const QPoint &end, QList<QPoint> &path) {
    if (start == end) {
        qDebug() << " Data Error";
        return false;
    }
    path.push_back(start);// 起点加入列表

    bool isFindPath = false;// 找到路径与否
    if (false) {// todo check startPoint and endPoint validity
        return isFindPath;
    }

    // 判断探索方向
    const int directionCount = 4;// 有4种探索方向
    QVector<Direction> direction(directionCount);
    getPriorityDirections(direction, start, end);
    QVector<bool> isBlocked(4);
    //qDebug() << isBlocked << "IsBlocked";
    //isBlocked[2] = true;// 设置
    //isBlocked[3] = true;
    // direction 0 1 2 3 用来存储方向
    // isBlocked 0 1 2 3 用来表示是否允许探索
    // start和end的连线不是水平或者竖直的直线 说明需要从两个方向去探索 剩下两个是遇到阻碍时的备选方向

    routing(isFindPath, start, end, path, direction, isBlocked);// 迭代地靠近目标

    return isFindPath;// true 表明探索成功  false表示探索失败
}

void LineSearch::routing(bool &isFindPath, QPoint &start, const QPoint &end,
                         QList<QPoint> &path, QVector<Direction> &direction, QVector<bool> &isBlocked) {
    if (start == end) {// 起点和终点相同 则探索结束
        isFindPath = true;
        return;
    }
    int directionCount = 2;// 2 从起点向终点探索需要几个方向 1 表示起点和终点的连线为水平或者竖直
    if (start.x() == end.x() || start.y() == end.y()) {// 在同一列 X相同   或者   在同一行 Y相同
        directionCount = 1;
    }
    printDirection(direction);
    bool searchFail = true;//directionCount个探索方向都被阻挡 则需要反方向探索
    for (int i = 0; i < directionCount; ++i) {
        if (!isBlocked[i]) {//
            searchFail = false;
            break;
        }
    }
    QPoint point = start;// 备份
    while (!searchFail) {
        for (int i = 0; i < directionCount; ++i) {
            if (isBlocked[i]) {
                continue;
            }

            if (direction[i] == Direction::TOP) {
                // 允许向上探索
                topSearching(isFindPath, point, end, path, direction, isBlocked);
                if (start == point) {// 探索失败
                    isBlocked[i] = true;
                    continue;
                } else if (point.y() == end.y()) {  // 一步到位
                    // 重新探索
                    //getPriorityDirections(direction, point, end);
                    isBlocked = QVector<bool>(4);// 重置
                } else {// 探索遇到了障碍 没能一步到位
                    //getPriorityDirections(direction, point, end);
                    //direction[0] = Direction::LEFT;// 左
                    //direction[1] = Direction::RIGHT;// 右
                    //direction[2] = Direction::BOTTOM;// 下
                    //direction[3] = Direction::TOP;// 上
                    isBlocked = QVector<bool>(4);// 重置
                    isBlocked[i] = true;
                }
                path.push_back(point);
                qDebug() << start << " --> " << point << " Top";
                start = point;
            } else if (direction[i] == Direction::BOTTOM) {
                // 允许向下探索
                bottomSearching(isFindPath, point, end, path, direction, isBlocked);
                if (start == point) {// 探索失败
                    isBlocked[i] = true;
                    continue;
                } else if (point.y() == end.y()) {// 一步到位
                    // 重新探索
                    //getPriorityDirections(direction, point, end);
                    isBlocked = QVector<bool>(4);// 重置
                } else {// 探索遇到了障碍  未能一步到位
                    //getPriorityDirections(direction, point, end);
                    //direction[0] = Direction::LEFT;// 左
                    //direction[1] = Direction::RIGHT;// 右
                    //direction[2] = Direction::BOTTOM;// 下
                    //direction[3] = Direction::TOP;// 上
                    isBlocked = QVector<bool>(4);// 重置
                    isBlocked[i] = true;
                }
                path.push_back(point);
                qDebug() << start << " --> " << point << " Bottom";
                start = point;
            } else if (direction[i] == Direction::LEFT) {
                // 允许向左探索
                leftSearching(isFindPath, point, end, path, direction, isBlocked);
                if (start == point) {// 探索失败
                    isBlocked[i] = true;
                    continue;
                } else if (point.x() == end.x()) {// 一步到位
                    // 重新探索
                    //getPriorityDirections(direction, point, end);
                    isBlocked = QVector<bool>(4);// 重置
                    path.push_back(point);
                    qDebug() << start << " --> " << point << " Left";
                    routing(isFindPath, point, end, path, direction, isBlocked);
                } else {// 探索遇到了障碍
                    //getPriorityDirections(direction, point, end);
                    //direction[3] = Direction::LEFT;// 左
                    //direction[2] = Direction::RIGHT;// 右
                    //direction[0] = Direction::BOTTOM;// 下
                    //direction[1] = Direction::TOP;// 上
                    isBlocked = QVector<bool>(4);// 重置
                    isBlocked[i] = true;
                }
                path.push_back(point);
                qDebug() << start << " --> " << point << " Left";
                start = point;
            } else if (direction[i] == Direction::RIGHT) {
                // 允许向右探索
                rightSearching(isFindPath, point, end, path, direction, isBlocked);

                if (start == point) {// 探索失败
                    isBlocked[i] = true;
                    continue;
                } else if (point.x() == end.x()) {// 一步到位
                    // 重新探索
                    //getPriorityDirections(direction, point, end);
                    isBlocked = QVector<bool>(4);// 重置
                } else {// 探索遇到了障碍
                    //getPriorityDirections(direction, point, end);
                    //direction[2] = Direction::LEFT;// 左
                    //direction[3] = Direction::RIGHT;// 右
                    //direction[0] = Direction::BOTTOM;// 下
                    //direction[1] = Direction::TOP;// 上
                    isBlocked = QVector<bool>(4);// 重置
                    isBlocked[i] = true;
                }
                path.push_back(point);
                qDebug() << start << " --> " << point << " Right";
                start = point;
            }
        }

        if (point == end) {// 探索结束   路径成功找到 退出循环
            isFindPath = true;
            return;
        }

        qDebug() << start << " point " << point;
        for (int i = 0; i < directionCount; ++i) {

        }

        if (start == point) {// 被阻碍 被阻碍方向可能为1 2
            break;
        }

        if (start.x() == end.x() || start.y() == end.y()) {// 在同一列 X相同   或者   在同一行 Y相同
            directionCount = 1;
        }

        searchFail = true;
        for (int i = 0; i < directionCount; ++i) {
            if (!isBlocked[i]) {//directionCount个探索方向都被阻挡 则需要反方向探索
                searchFail = false;
                break;
            }
        }
    }

    // 从其他方向继续探索
    qDebug() << start << " point " << end;
    qDebug() << isBlocked << "isBlocked";
    //getPriorityDirections(direction, start, end);
    printDirection(direction);
    qDebug() << directionCount << "DirectionCount";
    /*

    // 对四个方向进行探索 如果point修改了 说明前进了 否则考虑另外一个方向 todo modify  Bug -- 0407
    for (int i = 0; i < 4; ++i) {// 根据优先级进行探索
        if (!isBlocked[0] && direction[i] == Direction::TOP) {
            // 允许向上探索
            topSearching(isFindPath, point, end, path, direction, isBlocked);
            if (start == point) {// 探索失败
                isBlocked[0] = true;
                continue;
            } else if (point.y() == end.y()) {  // 一步到位
                // 重新探索
                //getPriorityDirections(direction, point, end);
                isBlocked = QVector<bool>(4);// 重置
            } else {// 探索遇到了障碍 没能一步到位
                //getPriorityDirections(direction, point, end);
                direction[0] = Direction::LEFT;// 左
                direction[1] = Direction::RIGHT;// 右
                direction[2] = Direction::BOTTOM;// 下
                direction[3] = Direction::TOP;// 上
                isBlocked = QVector<bool>(4);// 重置
                isBlocked[0] = true;
            }
            path.push_back(point);
            qDebug() << start << " --> " << point << " Top";
            routing(isFindPath, point, end, path, direction, isBlocked);
        } else if (!isBlocked[1] && direction[i] == Direction::BOTTOM) {
            // 允许向下探索
            bottomSearching(isFindPath, point, end, path, direction, isBlocked);
            if (start == point) {// 探索失败
                isBlocked[1] = true;
                continue;
            } else if (point.y() == end.y()) {// 一步到位
                // 重新探索
                //getPriorityDirections(direction, point, end);
                isBlocked = QVector<bool>(4);// 重置
            } else {// 探索遇到了障碍  未能一步到位
                //getPriorityDirections(direction, point, end);
                direction[0] = Direction::LEFT;// 左
                direction[1] = Direction::RIGHT;// 右
                direction[2] = Direction::BOTTOM;// 下
                direction[3] = Direction::TOP;// 上
                isBlocked = QVector<bool>(4);// 重置
                isBlocked[1] = true;
            }
            path.push_back(point);
            qDebug() << start << " --> " << point << " Bottom";
            routing(isFindPath, point, end, path, direction, isBlocked);
        } else if (!isBlocked[2] && direction[i] == Direction::LEFT) {
            // 允许向左探索
            leftSearching(isFindPath, point, end, path, direction, isBlocked);
            if (start == point) {// 探索失败
                isBlocked[2] = true;
                continue;
            } else if (point.x() == end.x()) {// 一步到位
                // 重新探索
                //getPriorityDirections(direction, point, end);
                isBlocked = QVector<bool>(4);// 重置
                path.push_back(point);
                qDebug() << start << " --> " << point << " Left";
                routing(isFindPath, point, end, path, direction, isBlocked);
            } else {// 探索遇到了障碍
                //getPriorityDirections(direction, point, end);
                direction[3] = Direction::LEFT;// 左
                direction[2] = Direction::RIGHT;// 右
                direction[0] = Direction::BOTTOM;// 下
                direction[1] = Direction::TOP;// 上
                isBlocked = QVector<bool>(4);// 重置
                isBlocked[2] = true;
            }
            path.push_back(point);
            qDebug() << start << " --> " << point << " Left";
            routing(isFindPath, point, end, path, direction, isBlocked);
        } else if (!isBlocked[3] && direction[i] == Direction::RIGHT) {
            // 允许向右探索
            rightSearching(isFindPath, point, end, path, direction, isBlocked);

            if (start == point) {// 探索失败
                isBlocked[3] = true;
                continue;
            } else if (point.x() == end.x()) {// 一步到位
                // 重新探索
                //getPriorityDirections(direction, point, end);
                isBlocked = QVector<bool>(4);// 重置
            } else {// 探索遇到了障碍
                //getPriorityDirections(direction, point, end);
                direction[2] = Direction::LEFT;// 左
                direction[3] = Direction::RIGHT;// 右
                direction[0] = Direction::BOTTOM;// 下
                direction[1] = Direction::TOP;// 上
                isBlocked = QVector<bool>(4);// 重置
                isBlocked[3] = true;
            }
            path.push_back(point);
            qDebug() << start << " --> " << point << " Right";
            routing(isFindPath, point, end, path, direction,
                    isBlocked);// & isFindPath path direction isBlocked 可以删除 todo consider delete
        }
        if (point == end) {// 探索结束   路径成功找到 退出循环
            isFindPath = true;
            return;
        }
        //aStarRouting(isFindPath, point, end, path, direction, isBlocked);
        //if (isFindPath) {
        //    return;
        //}
    }

    */

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

void LineSearch::leftSearching(bool &isFindPath, QPoint &start, const QPoint &end, QList<QPoint> &path,
                               QVector<Direction> &direction, QVector<bool> &isBlocked) {
    // 向左探索 能一步到位最好 不能则探索到*** todo consider
    // m_rowChannel
    int min_x = end.x();
    int max_x = start.x();// 向左探索 起点在终点右侧(起点的x坐标大于终点的x坐标)
    if (max_x < min_x) {
        return;
    }
    if (start.x() == end.x()) {
        min_x = start.x() - STEP * GRID;
    }

    int index = start.y() / GRID;// 在哪个行通道上
    auto list = m_rowChannel[index];
    qDebug() << list << "LIST";
    // 特殊情况 列表为空 则直接插入
    if (list.size() == 0) {
        start.setX(min_x);// 向左探索 x修改
        return;
    }

    // 列表不为空 则有三种插入情况 表头 表尾 中间

    if (max_x < list[0]) {// 表头 能一步到位
        start.setX(min_x);// 向左探索 x修改
        return;
    }
    if (max_x > list[list.size() - 1]) {// 表尾
        if (min_x < list[list.size() - 1]) {// 能一步到位
            start.setX(min_x);
        } else {
            start.setX(list[list.size() - 1] + GRID);// 不能一步到位
            //isBlocked = QVector<bool>(4);
            //isBlocked[2] = true;// 碰到障碍

            //direction[3] = Direction::LEFT;// 左
            //direction[2] = Direction::RIGHT;// 右
            //
            //if (start.y() > end.y()) {// 起点在终点下方
            //    direction[1] = Direction::TOP;
            //    direction[3] = Direction::BOTTOM;
            //} else {
            //    direction[0] = Direction::BOTTOM;// 下
            //    direction[1] = Direction::TOP;// 上
            //}
            //path.push_back(start);// update point
            //aStarRouting(isFindPath, start, end, path, direction);
        }
        return;
    }
    // 列表中间
    for (int i = 0; i < list.size(); ++i) {
        if (i % 2 == 1) {// 奇数
            if (max_x < list[i + 1] - GRID) {// 找到插入位置
                if (min_x > list[i] + GRID) {// 能一步到位
                    start.setX(min_x);// 向左探索 x修改
                } else {
                    start.setX(list[i] + 1 * GRID);// 向左探索到尽头 todo consider
                    //isBlocked = QVector<bool>(4);
                    //isBlocked[2] = true;// 碰到障碍

                    //direction[3] = Direction::LEFT;// 左
                    //direction[2] = Direction::RIGHT;// 右
                    //
                    //if (start.y() > end.y()) {// 起点在终点下方
                    //    direction[1] = Direction::TOP;
                    //    direction[3] = Direction::BOTTOM;
                    //} else {
                    //    direction[0] = Direction::BOTTOM;// 下
                    //    direction[1] = Direction::TOP;// 上
                    //}
                    //path.push_back(start);// update point
                    //aStarRouting(isFindPath, start, end, path, direction);
                }
                return;
            }
        }
    }
}

void LineSearch::rightSearching(bool &isFindPath, QPoint &start, const QPoint &end, QList<QPoint> &path,
                                QVector<Direction> &direction, QVector<bool> &isBlocked) {
    // 向右探索 能一步到位最好 不能则探索到*** todo consider
    // m_rowChannel
    int min_x = start.x();// 向右探索 则起点在终点左侧(起点的x坐标小于终点的x坐标)
    int max_x = end.x();
    if (max_x < min_x) {
        return;
    }

    if (start.x() == end.x()) {
        max_x = start.x() + STEP * GRID;
    }

    int index = start.y() / GRID;
    auto list = m_rowChannel[index];
    qDebug() << list << "LIST";
    if (list.size() == 0) {// 一步到位
        start.setX(max_x);
        return;
    }

    if (min_x < list[0]) {    // 表头可以插入
        if (max_x < list[0]) {      // 一步到位
            start.setX(max_x);
        } else {
            start.setX(list[0] - GRID * 1);
            //isBlocked = QVector<bool>(4);
            //isBlocked[3] = true;// 碰到障碍
            //
            //direction[3] = Direction::LEFT;// 左
            //direction[2] = Direction::RIGHT;// 右
            //direction[1] = Direction::BOTTOM;// 下
            //direction[0] = Direction::TOP;// 上
        }
        return;
    }

    if (min_x > list[list.size() - 1]) {    // 表尾可以插入 // 一步到位
        start.setX(max_x);
        return;
    }

    // 插入到列表中间
    for (int i = 1; i < list.size(); i += 2) {// todo modify  int i=0 --> i=1;;i++ -->i+=2
        // i 为 奇数
        if (min_x > list[i]) {// 找到插入位置
            if (max_x < list[i + 1]) {// 能一步到位
                start.setX(max_x);
            } else {
                start.setX(list[i] + GRID * 1);
                //isBlocked = QVector<bool>(4);
                //isBlocked[3] = true;// 碰到障碍
            }
            return;
        }
    }
}

void LineSearch::topSearching(bool &isFindPath, QPoint &start, const QPoint &end, QList<QPoint> &path,
                              QVector<Direction> &direction, QVector<bool> &isBlocked) {
    // 向上探索 能一步到位最好 不能则探索到*** todo consider
    // m_columnChannel
    int min_y = end.y();
    int max_y = start.y();// 起点在终点下方 起点的y坐标大于终点的y坐标

    if (max_y < min_y) {
        return;
    }

    if (start.y() == end.y()) {// 水平方向的探索被阻挡
        // 被阻挡 设置每次探索的步长    1
        min_y = start.x() - STEP * GRID;
    }

    int index = start.x() / GRID;
    auto list = m_columnChannel[index];
    qDebug() << list << "LIST";

    if (list.size() == 0) {// 一步到位
        start.setY(min_y);
        return;
    }

    if (max_y < list[0]) {// 表头插入 能一步到位
        start.setY(min_y);
        return;
    }
    if (max_y > list[list.size() - 1]) {// 表尾插入
        if (min_y > list[list.size() - 1]) { // 能一步到位
            start.setY(min_y);
            return;
        } else {
            start.setY(list[list.size() - 1] + GRID);
            //isBlocked = QVector<bool>(4);
            //isBlocked[0] = true;// 碰到障碍
            return;
        }
    }

    // 插入到列表中间
    for (int i = 1; i < list.size(); i += 2) {
        // 奇数
        if (max_y < list[i + 1]) {
            if (min_y > list[i]) {// 能一步到位
                start.setY(min_y);
            } else {
                start.setY(list[i] + GRID * 1);
                //isBlocked = QVector<bool>(4);
                //isBlocked[0] = true;// 碰到障碍
            }
            return;
        }
    }
}

void LineSearch::bottomSearching(bool &isFindPath, QPoint &start, const QPoint &end, QList<QPoint> &path,
                                 QVector<Direction> &direction, QVector<bool> &isBlocked) {
    // 向下探索 能一步到位最好 不能则探索到*** todo consider
    // m_columnChannel
    int max_y = end.y();
    int min_y = start.y();// 起点在终点上方 起点的y坐标小于终点的y坐标
    if (max_y < min_y) {
        return;
    }

    if (start.y() == end.y()) {
        max_y = start.y() + STEP * GRID;
    }

    int index = start.x() / GRID;
    auto list = m_columnChannel[index];
    qDebug() << list << "LIST";
    if (list.size() == 0) {// 一步到位
        start.setY(max_y);
        return;
    }

    if (min_y < list[0]) {// 表头
        if (max_y < list[0]) {// 能一步到位
            start.setY(max_y);
        } else {
            start.setY(list[0] - GRID);
            //isBlocked = QVector<bool>(4);
            //isBlocked[1] = true;// 碰到障碍
        }
        return;
    }

    if (min_y > list[list.size() - 1]) {// 一步到位 表尾
        start.setY(max_y);
        return;
    }

    // 插入到列表中间
    for (int i = 1; i < list.size(); i += 2) {
        // 奇数
        if (min_y > list[i]) {
            if (max_y < list[i + 1]) {
                start.setY(max_y);
            } else {
                start.setY(list[i] + GRID * 1);
                //isBlocked = QVector<bool>(4);
                //isBlocked[1] = true;// 碰到障碍
            }
            return;
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

    int size = channel[index].size();
    if (size == 0) {
        channel[index].push_back(min);
        channel[index].push_back(max);
        return;
    }

    // size !=0  列表不为空 则存在头部 中间 尾部三种插入方式
    if (max < channel[index][0]) {// 头部
        channel[index].push_back(min);
        channel[index].push_back(max);
        return;
    }

    if (min > channel[index][size - 1]) {// 尾部
        channel[index].push_back(min);
        channel[index].push_back(max);
        return;
    }

    // 中间
    int temp = getIndex(min, channel[index]);
    if (temp == -1) {
        return;
    }
    if (temp % 2 == 1) {// 奇数
        if (min > channel[index][temp] && max < channel[index][temp + 1]) {
            // 找到合适位置
            channel[index].insert(temp, min);
            channel[index].insert(temp + 1, max);
            return;
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

void LineSearch::printDirection(const QVector<Direction> &direction) {
    QString str;// 探索方向优先级
    for (int i = 0; i < 4; ++i) {
        switch (direction[i]) {
            case LEFT:
                str += "Left ";
                break;
            case RIGHT:
                str += "Right ";
                break;
            case TOP:
                str += "Top ";
                break;
            case BOTTOM:
                str += "Bottom ";
                break;
        }
    }
    qDebug() << str << "Direction";
}

void LineSearch::getPriorityDirections(QVector<Direction> &direction, const QPoint &start, const QPoint &end,
                                       QVector<bool> &isBlocked) {
    // todo delete
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
}
