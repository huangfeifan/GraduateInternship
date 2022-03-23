//
// Created by Huangff on 2022/2/10.
//

#pragma once

#include <QVector>
#include <QRect>
#include <QDebug>

class ComputeAbsolutePos {
public:
    /// 计算模块的绝对位置   参数: 邻接表 模块大小 相对位置 网格大小
    ComputeAbsolutePos(const QVector<QList<int>> &graph, const QVector<QPoint> &size,
                       const QVector<QPoint> &relativePos, int grid, int rowSpacing, int columnSpacing);

public:
    // width = *.x()   weight = *.y()

    QVector<QPoint> getAbsolutePos() {
        return m_absolutePos;
    }

    // 返回所有模块占据的矩形大小
    QPoint getSccBlockSize() {
        return m_size;
    }

private:

    /// init GridIWidth and GridJHeight
    void initColumnRowInfo() {
        /// 初始化行列相关数据
        m_absolutePos = QVector<QPoint>(m_relativePos.size());

        int rowMin = 0, rowMax = 0, columnMin = 0, columnMax = 0;
        for (int i = 0; i < m_relativePos.size(); ++i) {
            int column = m_relativePos[i].x();
            int row = m_relativePos[i].y();
            rowMax = rowMax > row ? rowMax : row;
            rowMin = rowMin < row ? rowMin : row;

            columnMax = columnMax > column ? columnMax : column;
            columnMin = columnMin < column ? columnMin : column;
        }

        // 初始化数组长度 共计temp列
        int temp = columnMax - columnMin + 1;
        m_columnWidth = QVector<int>(temp);
        m_columnPosition = QVector<int>(temp);
        // 列间距共计temp+1个
        m_columnSpacing = QVector<int>(temp + 1);

        temp = rowMax - rowMin + 1;
        m_rowHeight = QVector<int>(temp);
        m_rowPosition = QVector<int>(temp);
        // 列间距共计temp+1个
        m_rowSpacing = QVector<int>(temp + 1);

    }

    /// 计算行宽列高
    void computeColumnAndRow() {
        /// 计算行宽列高
        for (int i = 0; i < m_relativePos.size(); ++i) {
            int column = m_relativePos[i].x();// 列
            int row = m_relativePos[i].y();// 行
            m_columnWidth[column] =
                    m_columnWidth[column] > m_moduleSize[i].x() ? m_columnWidth[column] : m_moduleSize[i].x();
            //qDebug() << m_rowHeight[row] << " " << m_moduleSize[i].y();
            m_rowHeight[row] = m_rowHeight[row] > m_moduleSize[i].y() ? m_rowHeight[row] : m_moduleSize[i].y();
            //qDebug() << m_rowHeight[row];
        }
        //qDebug() << m_rowHeight << "  m_rowHeight";
        //qDebug() << m_columnWidth << "  m_columnWidth";
    }

    /// 计算每行每列的间距
    void computeColumnAndRowSpacing() {
        /// 计算每行每列的间距

        m_graph.clear();// todo delete 2022.0306

        for (int i = 0; i < m_graph.size(); ++i) {
            for (int j = 0; j < m_graph[i].size(); ++j) {
                /// i 起点    终点 graph[i][j]
                int startRow, startColumn, endColumn, endRow;
                startRow = m_relativePos[i].y();
                startColumn = m_relativePos[i].x();
                endRow = m_relativePos[m_graph[i][j]].y();
                endColumn = m_relativePos[m_graph[i][j]].x();

                /// TODO modify 时间复杂度较大 是否存在改进的可能

                // 修改列间距
                if (startColumn < endColumn) {/// 起点在终点左侧
                    for (int k = startColumn + 1; k <= endColumn; ++k) {
                        m_columnSpacing[k] += m_grid;
                    }
                } else {/// 起点在终点左侧或同一列
                    for (int k = endColumn + 1; k <= startColumn; ++k) {
                        m_columnSpacing[k] += m_grid;
                    }
                }

                // 修改行间距
                if (startRow < endRow) {/// 起点在终点左侧或同一列
                    for (int k = startRow + 1; k <= endRow; ++k) {
                        m_rowSpacing[k] += m_grid;
                    }
                } else {/// 起点在终点左侧或同一列
                    for (int k = endRow + 1; k <= startRow; ++k) {
                        m_rowSpacing[k] += m_grid;
                    }
                }
            }
        }

        for (int i = 1; i < m_rowSpacing.size() - 1; ++i) {
            m_rowSpacing[i] += rowSpacing;
        }

        for (int i = 1; i < m_columnSpacing.size() - 1; ++i) {
            m_columnSpacing[i] += columnSpacing;
        }

        //qDebug() << m_rowSpacing << "   m_rowSpacing";
        //qDebug() << m_columnSpacing << "    m_columnSpacing";
    }

    /// 计算每行每列的位置
    void computeColumnAndRowPosition() {
        /// 计算每行每列的位置 Todo modify 可能存在某些大模块导致行列太宽太高 需要换种方式计算绝对位置
        // 当前行 = 上一行位置+上一行高度+当前行与上一行间距 (列同理)
        m_columnPosition[0] = m_columnSpacing[0];
        for (int i = 1; i < m_columnPosition.size(); ++i) {
            m_columnPosition[i] = m_columnSpacing[i] + m_columnPosition[i - 1] + m_columnWidth[i - 1];
        }

        m_rowPosition[0] = m_rowSpacing[0];
        for (int i = 1; i < m_rowPosition.size(); ++i) {
            m_rowPosition[i] = m_rowSpacing[i] + m_rowPosition[i - 1] + m_rowHeight[i - 1];
        }

        //qDebug() << m_rowPosition << "  m_rowPosition";
        //qDebug() << m_columnPosition << "   m_columnPosition";

    }

    /// 计算模块的绝对位置
    void computePosition() {
        /// 计算模块的绝对位置
        for (int i = 0; i < m_relativePos.size(); ++i) {
            int row, column;
            row = m_relativePos[i].y();
            column = m_relativePos[i].x();
            // 左边距
            int leftXSpacing = (m_columnWidth[column] - m_moduleSize[i].x()) / 2;
            // 上边距
            int upYSpacing = (m_rowHeight[row] - m_moduleSize[i].x()) / 2;

            int xPos = leftXSpacing + m_columnPosition[column];
            int yPos = upYSpacing + m_rowPosition[row];
            m_absolutePos[i].setX(xPos);
            m_absolutePos[i].setY(yPos);
        }
        //qDebug() << m_absolutePos << "  before   m_absolutePos";

        // 以 (0,0) 为左上角顶点
        int minX = m_absolutePos[m_absolutePos.size() - 1].x();
        int minY = m_absolutePos[m_absolutePos.size() - 1].y();
        for (int i = 0; i < m_absolutePos.size(); ++i) {
            minX = minX > m_absolutePos[i].x() ? m_absolutePos[i].x() : minX;
            minY = minY > m_absolutePos[i].y() ? m_absolutePos[i].y() : minY;
        }
        QPoint point(minX, minY);
        for (int i = 0; i < m_absolutePos.size(); ++i) {
            m_absolutePos[i] -= point;
        }
        //qDebug() << m_absolutePos << "  after   m_absolutePos";
    }

    /// 计算所有模块占据的版图大小
    void computeBlockSize() {
        /// 计算所有模块占据的版图大小 或者遍历行宽列高 + 行列间距
        int column = m_columnPosition.size() - 1;
        m_size.setX(m_columnPosition[column] + m_columnSpacing[column + 1] + m_columnWidth[column]);
        int row = m_rowPosition.size() - 1;
        m_size.setY(m_rowPosition[row] + m_rowSpacing[row + 1] + m_rowHeight[row]);

        //qDebug() << m_size << "     m_size";
    }

    void solutionOne() {
        //qDebug() << m_relativePos << "  relativePos";
        //qDebug() << m_moduleSize << "  size";
        // 初始化数据
        initColumnRowInfo();
        //qDebug() << "-----------";
        // 计算行宽列高
        computeColumnAndRow();
        //qDebug() << "-----------";
        // 计算每行每列的间距
        computeColumnAndRowSpacing();
        //qDebug() << "-----------";
        // 计算每行每列的位置
        computeColumnAndRowPosition();
        //qDebug() << "-----------";
        // 计算模块的绝对位置
        computePosition();
        //qDebug() << "-----------";
        // 计算所有模块占据的版图大小
        computeBlockSize();
        //qDebug() << "-----------";
    }

    /// 不采用规则网格的方式来确定模块的最终位置
    void solutionTwo() {
        /* 数组哈希表存储 相对位置的信息
         * 计算哪一列模块最多
         * 先摆放模块最多的列 行间距(默认间距+待摆放和上一个出度)
         * 摆放其他列 基于最多列的模块位置 若不存在 则找到最近的相对行位置所在绝对位置   列间距
         * 调整位置 以(0,0)为基准
         * */

        // 通过模块的相对位置以及模块上下左右的端口数量来确定spacing
        // todo consider
        //  从上到下从左到右摆放
        //  难点在于spacing的确定 估算: 两倍(当前待摆放模块和上一个模块的出度之和)+默认spacing --不一定合理
        //  估算的优缺点: 优点: 有效降低了运算复杂度(原:KN K列数 N结点个数 现在:N)
        //               缺点: 估算方式的合理性???
        //      列spacing估算方式  基本列间距加上当前模块和左侧模块出度之和

        qDebug() << "Solution Two-----------------------------------------------------";
        QVector<bool> m_isPlaced = QVector<bool>(m_relativePos.size());
        //qDebug() << m_isPlaced << " m_isPlaced";// pass
        //qDebug() << m_relativePos << " m_relativePos";

        // 计算 列数
        int rowMin = 0, rowMax = 0, columnMin = 0, columnMax = 0;
        for (int i = 0; i < m_relativePos.size(); ++i) {
            int column = m_relativePos[i].x();
            int row = m_relativePos[i].y();
            rowMax = rowMax > row ? rowMax : row;
            rowMin = rowMin < row ? rowMin : row;

            columnMax = columnMax > column ? columnMax : column;
            columnMin = columnMin < column ? columnMin : column;
        }

        // Pass
        // 初始化数组长度 共计columnCount列
        int columnCount = columnMax - columnMin + 1;
        //qDebug() << columnCount << "  columnCount";

        // 行列信息
        QVector<QHash<int, int>> m_hash = QVector<QHash<int, int>>(columnCount);
        // 初始化数据
        for (int i = 0; i < m_moduleSize.size(); ++i) {
            QPoint point = m_relativePos[i];// .x()列 .y()行
            m_hash[point.x()].insert(point.y(), i);// key--row   value--index
        }
        //qDebug() << m_hash << "     m_hash";// Pass
        //qDebug() << m_relativePos << "     m_relativePos";// 强连通分支内部的相对位置 或者 强连通分之间的相对位置

        /// 计算哪一列包含的模块最多
        int moduleMaxColumnIndex = 0;
        int temp = -1;
        for (int i = 0; i < m_hash.size(); ++i) {
            if (temp < m_hash[i].size()) {
                temp = m_hash[i].size();
                moduleMaxColumnIndex = i;
            }
        }
        //qDebug() << moduleMaxColumnIndex << "    moduleMaxColumnIndex";// pass

        // 为了避免模块的重叠
        QVector<QRect> m_rect = QVector<QRect>(m_relativePos.size());


        /// 摆放包含模块数最多的列
        // 选择从上到下 找到行数最小的module 摆放后 再摆放下面的
        temp = columnSpacing;// 存放最小行数
        QHash<int, int>::const_iterator iter = m_hash[moduleMaxColumnIndex].constBegin();// key--row   value--index
        while (iter != m_hash[moduleMaxColumnIndex].constEnd()) {
            temp = temp > iter.key() ? iter.key() : temp;
            ++iter;
        }
        //qDebug() << temp << " row----Min";// pass

        // 摆放第一个模块
        int index = m_hash[moduleMaxColumnIndex].value(temp);
        qDebug() << index << " index---";// pass
        QRect rect = QRect(0, 0, m_moduleSize[index].x(), m_moduleSize[index].y());
        m_absolutePos[index] = QPoint(0, 0);
        m_isPlaced[index] = true;

        // 接着摆放其他模块
        //      计算待摆放模块
        QList<int> tempList;
        while (tempList.size() > m_hash[moduleMaxColumnIndex].size()) {// key--row   value--index
            temp++;
            if (m_hash[moduleMaxColumnIndex].contains(temp)) {
                //tempList.insert(m_hash[moduleMaxColumnIndex].va)
            }
        }

        //  摆放

        /// 摆放moduleMaxColumnIndex左侧的模块

        /// 摆放moduleMaxColumnIndex右侧的模块

        qDebug() << "Solution Two-----------------------------------------------------Finished";

    }


private:

    QVector<QList<int>> m_graph;// 邻接表
    QVector<QPoint> m_absolutePos;// 模块绝对位置
    QVector<QPoint> m_relativePos;// 模块相对位置
    QVector<QPoint> m_moduleSize;// 模块大小
    QVector<int> m_columnWidth;// 列宽
    QVector<int> m_rowHeight;// 行高
    QVector<int> m_columnPosition;// 列所在位置
    QVector<int> m_rowPosition;// 行所在位置

    QVector<int> m_columnSpacing;// 列间距
    QVector<int> m_rowSpacing;// 行间距

    QPoint m_size;// 预估所有模块占据总大小

    int m_grid = 10;//网格大小设定

    int rowSpacing = 0;// 间距
    int columnSpacing = 0;// 间距
};

