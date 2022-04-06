//
// Created by Huangff on 2022/4/6.
//

#include "LineSearch.h"

bool LineSearch::simplifyPath(QList<QPoint> &path) {

    return false;
}

QList<QPoint> LineSearch::simplifyPath(const QList<QPoint> &path) {
    return QList<QPoint>();
}

void LineSearch::addPathToChannel(const QList<QPoint> &path) {

}

void LineSearch::removePathFromChannel(const QList<QPoint> &path) {

}

void LineSearch::addModuleToChannel(const QPoint &pos, const QPoint &size) {

}

void LineSearch::removeModuleFromChannel(const QPoint &pos, const QPoint &size) {

}

void LineSearch::addModulesToChannel(const QVector<QPoint> &poss, const QVector<QPoint> &sizes) {

}

void LineSearch::removeModulesFromChannel(const QVector<QPoint> &poss, const QVector<QPoint> &sizes) {

}

bool LineSearch::routing(const QPoint &start, const QPoint &end) {
    if (start == end) {
        return true;
    }

    QList<QPoint> path;// 路径
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
    QVector<Direction> direction(4);
    getPriorityDirections(direction, start, end);
    const int size = direction.size();// 有4种探索方向
    QPoint point = start;

    // 对四个方向进行探索 如果point修改了 说明前进了 否则考虑另外一个方向 todo modify
    for (int i = 0; i < size; ++i) {
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
        if (start == point) {// 探索的某一个方向被完全阻碍
            continue;
        }
        getPriorityDirections(direction, point, end);// 判断探索方向
        routing(isFindPath, point, end, path);
    }
}

void LineSearch::getPriorityDirections(QVector<Direction> &direction, const QPoint &start, const QPoint &end) {
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
    int index = start.y();// 在哪个行通道上
    auto list = m_rowChannel[index];

    // 特殊情况 列表为空 则直接插入
    if (list.size() == 0) {
        start.setX(end.x());// 向左探索 x修改
        path.push_back(start);// update point
        return;
    }

    // 可以一步到位
    bool temp = false;// 能否直接抵达end.x()
    for (int i = 0; i < list.size(); ++i) {

    }


}

void LineSearch::rightSearching(QPoint &start, const QPoint &end, QList<QPoint> &path) {
    // 向右探索 能一步到位最好 不能则探索到*** todo consider
    // m_rowChannel

}

void LineSearch::topSearching(QPoint &start, const QPoint &end, QList<QPoint> &path) {
    // 向上探索 能一步到位最好 不能则探索到*** todo consider

}

void LineSearch::bottomSearching(QPoint &start, const QPoint &end, QList<QPoint> &path) {
    // 向下探索 能一步到位最好 不能则探索到*** todo consider

}
