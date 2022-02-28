//
// Created by Huangff on 2022/2/10.
//

#pragma once

#include <QVector>
#include <QDebug>

class ComputeAbsolutePos {
public:
    /// 计算模块的绝对位置
    ComputeAbsolutePos(const QVector<QList<int>> &graph, const QVector<QPoint> &size,
                        const QVector<QPoint> &relativePos, int grid);

public:
    // width=x   height=y

    QVector<QPoint> getAbsolutePos() {
        return m_absolutePos;
    }

    // 返回所有模块占据的矩形大小
    QPoint getSccBlockSize();

private:
    void initColumnRowInfo();/// init GridIWidth and GridJHeight
    void computeColumnAndRow();/// 计算行宽列高
    void computeColumnAndRowSpacing();/// 计算每行每列的间距
    void computeColumnAndRowPosition();/// 计算每行每列的位置
    void computePosition();/// 计算模块的绝对位置
    void computeBlockSize();/// 计算所有模块占据的版图大小

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
