//
// Created by Huangff on 2022/2/10.
//

#pragma once

#include <QVector>
#include <QDebug>

class ComputeAbsolutePos {
public:
    /// 计算模块的绝对位置   参数: 邻接表 模块大小 相对位置 网格大小
    ComputeAbsolutePos(const QVector<QList<int>> &graph, const QVector<QPoint> &size,
                       const QVector<QPoint> &relativePos, int grid);

public:
    // width = *.x()   height = *.y()

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
        qDebug() << m_rowHeight << "  m_rowHeight";
        qDebug() << m_columnWidth << "  m_columnWidth";
    }

    /// 计算每行每列的间距
    void computeColumnAndRowSpacing() {
        /// 计算每行每列的间距
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

        qDebug() << m_rowSpacing << "   m_rowSpacing";
        qDebug() << m_columnSpacing << "    m_columnSpacing";
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

        qDebug() << m_rowPosition << "  m_rowPosition";
        qDebug() << m_columnPosition << "   m_columnPosition";

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
        qDebug() << m_absolutePos << "     m_absolutePos";
    }

    /// 计算所有模块占据的版图大小
    void computeBlockSize() {
        /// 计算所有模块占据的版图大小 或者遍历行宽列高 + 行列间距
        int column = m_columnPosition.size() - 1;
        m_size.setX(m_columnPosition[column] + m_columnSpacing[column + 1] + m_columnWidth[column]);
        int row = m_rowPosition.size() - 1;
        m_size.setY(m_rowPosition[row] + m_rowSpacing[row + 1] + m_rowHeight[row]);

        qDebug() << m_size << "     m_size";
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

    QPoint m_size;// 所有模块占据矩形大小

    int m_grid = 10;//网格大小设定
};



