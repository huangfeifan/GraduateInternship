//
// Created by Huangff on 2022/3/28.
//

#include "Router.h"

void Router::tracePath(QVector<QVector<cell>> cellDetails, Pair dest) {
    //printf("\nThe path is ");
    int row = dest.first;
    int col = dest.second;

    QList<QPoint> path;

    while (!(cellDetails[row][col].parent_i == row
             && cellDetails[row][col].parent_j == col)) {
        path.push_back(QPoint(row, col));
        int temp_row = cellDetails[row][col].parent_i;
        int temp_col = cellDetails[row][col].parent_j;
        row = temp_row;
        col = temp_col;
    }

    path.push_back(QPoint(row, col));
    m_path = path;
    for (int i = 0; i < m_path.size(); ++i) {
        //printf("-> (%d,%d) ", m_path[i].y(), m_path[i].x());
    }
    return;
}

double Router::calculateHValue(int row, int col, Pair dest) {
    // Return using the distance formula
    return ((double) sqrt(
            (row - dest.first) * (row - dest.first)
            + (col - dest.second) * (col - dest.second)));
}

bool Router::isDestination(int row, int col, Pair dest) {
    if (row == dest.first && col == dest.second)
        return (true);
    else
        return (false);
}

bool Router::isUnBlocked(int row, int col, QVector<QVector<int>> grid) {
    // Returns true if the cell is not blocked else false
    if (grid[row][col] == 1)
        return (true);
    else
        return (false);
}

bool Router::isValid(int row, int col) {
    // Returns true if row number and column number
    // is in range
    return (row >= 0) && (row < m_row) && (col >= 0)
           && (col < m_column);
}

void Router::updatePathInfo(QList<QPoint> list, int blockType) {

    // 注意行列 GridInfo 先行后列 行高列宽 行Y列X
    for (int i = 1; i < list.size(); ++i) {
        int startX = list[i - 1].x();//
        int startY = list[i - 1].y();//
        int endX = list[i].x();//
        int endY = list[i].y();//

        // todo consider  边界条件的考量
        if (startX == endX) {
            m_columnGridInfo[endY][startX] = blockType;
            //m_columnGridInfo[startY][startX] = blockType;

            //m_rowGridInfo[startY][startX] = blockType;
            //m_rowGridInfo[startY][startX + 1] = blockType;
            //m_rowGridInfo[endY][startX] = blockType;
            //m_rowGridInfo[endY][startX + 1] = blockType;

        }

        if (startY == endY) {
            m_rowGridInfo[endY][endX] = blockType;
            //m_rowGridInfo[endY][startX] = blockType;

            //m_rowGridInfo[endY][endX + 1] = blockType;
            //m_rowGridInfo[endY][startX + 1] = blockType;
            //m_columnGridInfo[endY][startX] = blockType;
            //m_columnGridInfo[endY][endX] = blockType;
        }
    }
}

void Router::initGridInfo() {
    // 初始化数组
    m_rowGridInfo = QVector<QVector<int>>(m_row);
    m_columnGridInfo = QVector<QVector<int>>(m_row);
    for (int i = 0; i < m_row; ++i) {
        auto temp = QVector<int>(m_column);
        m_rowGridInfo[i] = temp;
        m_columnGridInfo[i] = temp;
        for (int j = 0; j < m_column; ++j) {
            m_rowGridInfo[i][j] = 1;
            m_columnGridInfo[i][j] = 1;
        }
    }
    for (int i = 0; i < m_modulePos.size(); ++i) {
        addRect(m_modulePos[i], m_moduleSize[i]);
    }
}

void Router::updateRectInfo(QPoint pos, QPoint size, int blockType) {
    const int gap = -1;// 线与模块的间距
    int left = pos.x();// 列 x
    int top = pos.y();
    int width = size.x();//列 x
    int height = size.y();
    for (int j = gap; j < width - gap; ++j) {// 列 x
        for (int k = gap; k < height - gap; ++k) {
            if ((top + k) >= 0 && (left + j) >= 0 && (top + k) < m_row && (left + j) < m_column) {
                m_rowGridInfo[top + k][left + j] = blockType;
                m_columnGridInfo[top + k][left + j] = blockType;
            }
        }
    }
}

void Router::aStarSearch(const Pair src, const Pair dest) {
    // If the source is out of range
    if (isValid(src.first, src.second) == false) {
        printf("Source is invalid\n");
        m_foundDest = false;
        return;
    }

    // If the destination is out of range
    if (isValid(dest.first, dest.second) == false) {
        printf("Destination is invalid\n");
        m_foundDest = false;
        return;
    }

    // Either the source or the destination is blocked
    if (isUnBlocked(src.first, src.second, m_rowGridInfo) == false
        || isUnBlocked(dest.first, dest.second, m_rowGridInfo)
           == false || isUnBlocked(src.first, src.second, m_columnGridInfo) == false
        || isUnBlocked(dest.first, dest.second, m_columnGridInfo)
           == false) {
        printf("Source or the destination is blocked\n");
        m_foundDest = false;
        return;
    }

    // If the destination cell is the same as source cell
    if (isDestination(src.first, src.second, dest)) {
        printf("We are already at the destination\n");
        m_foundDest = false;
        return;
    }

    // Create a closed list and initialise it to false which
    // means that no cell has been included yet This closed
    // list is implemented as a boolean 2D array
    QVector<QVector<bool>> closedList = QVector<QVector<bool>>(m_row);
    for (int i = 0; i < m_row; ++i) {
        closedList[i] = QVector<bool>(m_column);
    }
    //qDebug() << closedList[0][0];

    int i, j;

    // Declare a 2D array of structure to hold the details
    // of that cell
    QVector<QVector<cell>> m_cellDetails = QVector<QVector<cell>>(m_row);
    for (i = 0; i < m_row; ++i) {
        m_cellDetails[i] = QVector<cell>(m_column);
        for (j = 0; j < m_column; ++j) {
            cell cellInstance;
            m_cellDetails[i][j] = cellInstance;
        }
    }

    // Initialising the parameters of the starting node
    i = src.first, j = src.second;
    m_cellDetails[i][j].f = 0.0;
    m_cellDetails[i][j].g = 0.0;
    m_cellDetails[i][j].h = 0.0;
    m_cellDetails[i][j].parent_i = i;
    m_cellDetails[i][j].parent_j = j;

    /*
    Create an open list having information as-
    <f, <i, j>>
    where f = g + h,
    and i, j are the row and column index of that cell
    Note that 0 <= i <= ROW-1 & 0 <= j <= COL-1
    This open list is implemented as a set of pair of
    pair.*/
    QList<pPair> openList;

    // Put the starting cell on the open list and set its 'f' as 0
    openList.push_back(pPair(0.0, Pair(i, j)));

    // We set this boolean value as false as initially the destination is not reached.
    m_foundDest = false;

    while (!openList.empty()) {
        pPair p = *openList.begin();

        // Remove this vertex from the open list
        openList.erase(openList.begin());

        // Add this vertex to the closed list
        i = p.second.first;
        j = p.second.second;
        closedList[i][j] = true;

        /*
        Generating all the 8 successor of this cell

              N.W N N.E
                \ | /
            W----Cell----E
                / | \
              S.W S S.E

        Cell-->Popped Cell (i, j)
        N --> North	 (i-1, j)
        S --> South	 (i+1, j)
        E --> East	 (i, j+1)
        W --> West		 (i, j-1)
        N.E--> North-East (i-1, j+1)
        N.W--> North-West (i-1, j-1)
        S.E--> South-East (i+1, j+1)
        S.W--> South-West (i+1, j-1)

        But Schematic Routing only allow four direction(N,E,S,W).
         */

        // To store the 'g', 'h' and 'f' of the 4 successors
        double gNew, hNew, fNew;

        //----------- 1st Successor (North) ------------
        // Only process this cell if this is a valid one
        if (isValid(i - 1, j)) {
            // If the destination cell is the same as the
            // current successor
            if (isDestination(i - 1, j, dest)) {
                // Set the Parent of the destination cell
                m_cellDetails[i - 1][j].parent_i = i;
                m_cellDetails[i - 1][j].parent_j = j;
                //printf("The destination cell is found\n");
                tracePath(m_cellDetails, dest);
                m_foundDest = true;
                return;
            }
                // If the successor is already on the closed
                // list or if it is blocked, then ignore it.
                // Else do the following
            else if (closedList[i - 1][j] == false
                     && isUnBlocked(i - 1, j, m_columnGridInfo)) {
                gNew = m_cellDetails[i][j].g + 1.0;
                hNew = calculateHValue(i - 1, j, dest);
                fNew = gNew + hNew;

                // If it isn’t on the open list, add it to
                // the open list. Make the current square
                // the parent of this square. Record the
                // f, g, and h costs of the square cell
                //			 OR
                // If it is on the open list already, check
                // to see if this path to that square is
                // better, using 'f' cost as the measure.
                if (m_cellDetails[i - 1][j].f == FLT_MAX
                    || m_cellDetails[i - 1][j].f > fNew) {
                    openList.push_back(pPair(
                            fNew, Pair(i - 1, j)));

                    // Update the details of this cell
                    m_cellDetails[i - 1][j].f = fNew;
                    m_cellDetails[i - 1][j].g = gNew;
                    m_cellDetails[i - 1][j].h = hNew;
                    m_cellDetails[i - 1][j].parent_i = i;
                    m_cellDetails[i - 1][j].parent_j = j;
                }
            }
        }

        //----------- 2nd Successor (South) ------------
        // Only process this cell if this is a valid one
        if (isValid(i + 1, j)) {
            // If the destination cell is the same as the current successor
            if (isDestination(i + 1, j, dest)) {
                // Set the Parent of the destination cell
                m_cellDetails[i + 1][j].parent_i = i;
                m_cellDetails[i + 1][j].parent_j = j;
                //printf("The destination cell is found\n");
                tracePath(m_cellDetails, dest);
                m_foundDest = true;
                return;
            }
                // If the successor is already on the closed
                // list or if it is blocked, then ignore it.
                // Else do the following
            else if (closedList[i + 1][j] == false
                     && isUnBlocked(i + 1, j, m_columnGridInfo)) {
                gNew = m_cellDetails[i][j].g + 1.0;
                hNew = calculateHValue(i + 1, j, dest);
                fNew = gNew + hNew;

                // If it isn’t on the open list, add it to
                // the open list. Make the current square
                // the parent of this square. Record the
                // f, g, and h costs of the square cell
                //			 OR
                // If it is on the open list already, check
                // to see if this path to that square is
                // better, using 'f' cost as the measure.
                if (m_cellDetails[i + 1][j].f == FLT_MAX
                    || m_cellDetails[i + 1][j].f > fNew) {
                    openList.push_back(pPair(
                            fNew, Pair(i + 1, j)));
                    // Update the details of this cell
                    m_cellDetails[i + 1][j].f = fNew;
                    m_cellDetails[i + 1][j].g = gNew;
                    m_cellDetails[i + 1][j].h = hNew;
                    m_cellDetails[i + 1][j].parent_i = i;
                    m_cellDetails[i + 1][j].parent_j = j;
                }
            }
        }

        //----------- 3rd Successor (East) ------------
        // Only process this cell if this is a valid one
        if (isValid(i, j + 1)) {
            // If the destination cell is the same as the
            // current successor
            if (isDestination(i, j + 1, dest)) {
                // Set the Parent of the destination cell
                m_cellDetails[i][j + 1].parent_i = i;
                m_cellDetails[i][j + 1].parent_j = j;
                //printf("The destination cell is found\n");
                tracePath(m_cellDetails, dest);
                m_foundDest = true;
                return;
            }

                // If the successor is already on the closed
                // list or if it is blocked, then ignore it.
                // Else do the following
            else if (closedList[i][j + 1] == false
                     && isUnBlocked(i, j + 1, m_rowGridInfo)
                    ) {
                gNew = m_cellDetails[i][j].g + 1.0;
                hNew = calculateHValue(i, j + 1, dest);
                fNew = gNew + hNew;

                // If it isn’t on the open list, add it to
                // the open list. Make the current square
                // the parent of this square. Record the
                // f, g, and h costs of the square cell
                //			 OR
                // If it is on the open list already, check
                // to see if this path to that square is
                // better, using 'f' cost as the measure.
                if (m_cellDetails[i][j + 1].f == FLT_MAX
                    || m_cellDetails[i][j + 1].f > fNew) {
                    openList.push_back(pPair(
                            fNew, Pair(i, j + 1)));

                    // Update the details of this cell
                    m_cellDetails[i][j + 1].f = fNew;
                    m_cellDetails[i][j + 1].g = gNew;
                    m_cellDetails[i][j + 1].h = hNew;
                    m_cellDetails[i][j + 1].parent_i = i;
                    m_cellDetails[i][j + 1].parent_j = j;
                }
            }
        }

        //----------- 4th Successor (West) ------------
        // Only process this cell if this is a valid one
        if (isValid(i, j - 1)) {
            // If the destination cell is the same as the
            // current successor
            if (isDestination(i, j - 1, dest)) {
                // Set the Parent of the destination cell
                m_cellDetails[i][j - 1].parent_i = i;
                m_cellDetails[i][j - 1].parent_j = j;
                //printf("The destination cell is found\n");
                tracePath(m_cellDetails, dest);
                m_foundDest = true;
                return;
            }

                // If the successor is already on the closed
                // list or if it is blocked, then ignore it.
                // Else do the following
            else if (closedList[i][j - 1] == false
                     && isUnBlocked(i, j - 1, m_rowGridInfo)
                    ) {
                gNew = m_cellDetails[i][j].g + 1.0;
                hNew = calculateHValue(i, j - 1, dest);
                fNew = gNew + hNew;

                // If it isn’t on the open list, add it to
                // the open list. Make the current square
                // the parent of this square. Record the
                // f, g, and h costs of the square cell
                //			 OR
                // If it is on the open list already, check
                // to see if this path to that square is
                // better, using 'f' cost as the measure.
                if (m_cellDetails[i][j - 1].f == FLT_MAX
                    || m_cellDetails[i][j - 1].f > fNew) {
                    openList.push_back(pPair(
                            fNew, Pair(i, j - 1)));

                    // Update the details of this cell
                    m_cellDetails[i][j - 1].f = fNew;
                    m_cellDetails[i][j - 1].g = gNew;
                    m_cellDetails[i][j - 1].h = hNew;
                    m_cellDetails[i][j - 1].parent_i = i;
                    m_cellDetails[i][j - 1].parent_j = j;
                }
            }
        }
    }

    // When the destination cell is not found and the open
    // list is empty, then we conclude that we failed to
    // reach the destination cell. This may happen when the
    // there is no way to destination cell (due to
    // blockages)
    if (m_foundDest == false)
        printf("Failed to find the Destination Cell\n");

    return;
}

void Router::addPath(QList<QPoint> list) {
    updatePathInfo(list, 0);
}

void Router::addRect(QPoint pos, QPoint size) {
    updateRectInfo(pos, size, 0);
}

void Router::removePath(QList<QPoint> list) {
    updatePathInfo(list, 1);
}

void Router::removeRect(QPoint pos, QPoint size) {
    updateRectInfo(pos, size, 1);
}

QList<QPoint> Router::getPaths() {
    return m_path;
}

bool Router::isSuccessFindPaths() {
    return m_foundDest;
}

void Router::setRowGridInfo(QVector<QVector<int>> rowGridInfo) {
    m_rowGridInfo = rowGridInfo;
}

void Router::setColumnGridInfo(QVector<QVector<int>> rowGridInfo) {
    m_columnGridInfo = rowGridInfo;
}
