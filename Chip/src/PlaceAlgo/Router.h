//
// Created by Huangff on 2022/1/26.
//

#pragma once

#include <QStack>
#include <QList>
#include <QPair>
#include <QDebug>

// Creating a shortcut for int, int pair type
typedef QPair<int, int> Pair;

// Creating a shortcut for pair<int, pair<int, int>> type
typedef QPair<double, QPair<int, int> > pPair;

// A structure to hold the necessary parameters
struct cell {
    // Row and Column index of its parent
    // Note that 0 <= i <= ROW-1 & 0 <= j <= COL-1
    int parent_i = -1;
    int parent_j = -1;
    // f = g + h
    double f = FLT_MAX;
    double g = FLT_MAX;
    double h = FLT_MAX;
};

class Router {
public:
    // Constructor Function
    Router(int row, int column, QVector<QPoint> pos, QVector<QPoint> size)
            : m_row(row), m_column(column), m_modulePos(pos), m_moduleSize(size) {
        initGridInfo();
    }

    // A Function to find the shortest path between
    // a given source cell to a destination cell according
    // to A* Search Algorithm
    void aStarSearch(const Pair src, const Pair dest);

    // update gridInfo
    void addPath(QList<QPoint> list);

    void addLine(QPoint start, QPoint end);

    void removeLine(QPoint start, QPoint end);

    void removePath(QList<QPoint> list);

    void removeRect(QPoint pos, QPoint size);

    void addRect(QPoint pos, QPoint size);

    // get the routing result
    QList<QPoint> getPaths();

    bool isSuccessFindPaths();

    void setRowGridInfo(QVector<QVector<int>> rowGridInfo);

    void setColumnGridInfo(QVector<QVector<int>> rowGridInfo);

private:
    // update blocked grid
    void updateRectInfo(QPoint pos, QPoint size, int blockType);

    // Init Grid info
    void initGridInfo();

    void updatePathInfo(QList<QPoint> list, int blockType);

    // A Utility Function to check whether given cell (row, col) is a valid cell or not.
    bool isValid(int row, int col);

    // A Utility Function to check whether the given cell is blocked or not
    bool isUnBlocked(int row, int col, QVector<QVector<int>> grid);

    // A Utility Function to check whether destination cell has been reached or not
    bool isDestination(int row, int col, Pair dest);

    // A Utility Function to calculate the 'h' heuristics.
    double calculateHValue(int row, int col, Pair dest);

    // A Utility Function to trace the path from the source to destination
    void tracePath(QVector<QVector<cell>> cellDetails, Pair dest);

private:
    // 行y 列x 行高列宽
    int m_row;// row count
    int m_column;// column count
    bool m_foundDest = false;
    QList<QPoint> m_path;// routing result //  m_paths[i].size()==0 means search path failed
    QVector<QVector<int>> m_rowGridInfo;// row Channel  同一行Y相同
    QVector<QVector<int>> m_columnGridInfo;// column Channel 同一列X相同
    QVector<QPoint> m_modulePos; // modulePos
    QVector<QPoint> m_moduleSize;// moduleSize
};
