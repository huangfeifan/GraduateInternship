//
// Created by Huangff on 2022/2/10.
//

#include "ComputeAbsolutePos.h"

void ComputeAbsolutePos::computeBlockSize() {
    /// 计算所有模块占据的版图大小
    int column = m_columnPosition.size() - 1;
    m_size.setX(m_columnPosition[column] + m_columnSpacing[column + 1]);
    int row = m_rowPosition.size() - 1;
    m_size.setY(m_rowPosition[row] + m_rowSpacing[row + 1]);

    qDebug() << m_size << "     m_size";
}

void ComputeAbsolutePos::computePosition() {
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

void ComputeAbsolutePos::computeColumnAndRowPosition() {
    /// 计算每行每列的位置
    m_columnPosition[0] = m_columnSpacing[0];
    for (int i = 1; i < m_columnPosition.size(); ++i) {
        m_columnPosition[i] = m_columnSpacing[i] + m_columnPosition[i - 1];
    }

    m_rowPosition[0] = m_rowSpacing[0];
    for (int i = 1; i < m_rowPosition.size(); ++i) {
        m_rowPosition[i] = m_rowSpacing[i] + m_rowPosition[i - 1];
    }
}

void ComputeAbsolutePos::computeColumnAndRowSpacing() {
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

    qDebug() << m_rowHeight << "    m_rowHeight";
    qDebug() << m_rowSpacing << "   m_rowSpacing";
    qDebug() << m_rowPosition << "  m_rowPosition";

    qDebug() << m_columnWidth << "  m_columnWidth";
    qDebug() << m_columnSpacing << "    m_columnSpacing";
    qDebug() << m_columnPosition << "   m_columnPosition";
}

void ComputeAbsolutePos::computeColumnAndRow() {
    /// 计算行宽列高
    for (int i = 0; i < m_relativePos.size(); ++i) {
        int column = m_relativePos[i].x();// 列
        int row = m_relativePos[i].y();// 行
        m_columnWidth[column] =
                m_columnWidth[column] > m_moduleSize[i].x() ? m_columnWidth[column] : m_moduleSize[i].x();
        qDebug() << m_rowHeight[row] << " " << m_moduleSize[i].y();
        m_rowHeight[row] = m_rowHeight[row] > m_moduleSize[i].y() ? m_rowHeight[row] : m_moduleSize[i].y();
        qDebug() << m_rowHeight[row];
    }
    qDebug() << m_rowHeight << "  m_rowHeight";
    qDebug() << m_columnWidth << "  m_columnWidth";
}

void ComputeAbsolutePos::initColumnRowInfo() {
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

QPoint ComputeAbsolutePos::getSccBlockSize() {
    return m_size;
}

ComputeAbsolutePos::ComputeAbsolutePos(const QVector<QList<int>> &graph, const QVector<QPoint> &size,
                                         const QVector<QPoint> &relativePos, int grid) : m_graph(graph),
                                                                                         m_moduleSize(size),
                                                                                         m_relativePos(relativePos),
                                                                                         m_grid(grid) {
    /**
     * 目的：确定每个元件的布局最终位置
     *  已有各个模块的大小 模块的相对位置
     *
     *  难点：如何根据模块间的连接信息得到模块的最终位置
     *
     *  问题分解
     *      1.类似行列定位的思路 首先确定每一列的宽度 其次确定列与列之间的间距(根据通道的概念 预留出足够的空间) 最后得到每一列的坐标
     *      2.按照列的方式 依然根据通道的概念 预留出足够的空间 得到每一行的坐标
     *      3.对通道数据的存储 辅助线探索判断后续的走线  线探索需要处理的事情  Todo : need consider
     *
     *  如何计算列与列的通道数     粗略计算  todo modify
     *  如何计算行与行的通道数     粗略计算
     */

    qDebug() << relativePos << "  relativePos";


    // 初始化数据
    initColumnRowInfo();

    // 计算行宽列高
    computeColumnAndRow();

    // 计算每行每列的间距
    computeColumnAndRowSpacing();

    // 计算每行每列的位置
    computeColumnAndRowPosition();

    // 计算模块的绝对位置
    computePosition();

    // 计算所有模块占据的版图大小
    computeBlockSize();
}
