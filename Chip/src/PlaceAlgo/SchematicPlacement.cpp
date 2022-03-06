//
// Created by Huangff on 2022/2/11.
//

#include "SchematicPlacement.h"
#include "Data.h"

SchematicPlacement::SchematicPlacement(QVector<QList<int>> graphData, QVector<QPoint> moduleSize) : m_moduleConnectData(
        graphData), m_size(moduleSize) {
    /// 1.数据预处理
    /// 2.计算强连通分支间的相对位置
    /// 3.计算强连通内部的相对位置和绝对位置
    /// 4.计算强连通间的绝对位置
    /// 5.计算每个模块的绝对位置
    /// 6.计算单独port的绝对位置
    /// 全剧终

    // passed
    preHandleData();

    // bug 数组越界 Fix Time: 0228
    computePosition();
}

/*SchematicPlacement::~SchematicPlacement() {

}*/
