//
// Created by Huangff on 2022/2/10.
//

#include "ComputeAbsolutePos.h"

ComputeAbsolutePos::ComputeAbsolutePos(const QVector<QList<int>> &graph, const QVector<QPoint> &size,
                                       const QVector<QPoint> &relativePos, int rowSpacing, int columnSpacing)
        : m_graph(graph),
          m_moduleSize(size),
          m_relativePos(relativePos),
          rowSpacing(rowSpacing),
          columnSpacing(columnSpacing) {
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

    //qDebug() << "----------------------Compute AbsolutePos---------------------------------Start";
    // 方法1
    solutionOne();

    // 方法2
    //solutionTwo();

    //qDebug() << "----------------------Compute AbsolutePos---------------------------------End";

}
