//
// Created by Huangff on 2022/2/11.
//

#include "SchematicPlacement.h"
#include "Data.h"

SchematicPlacement::SchematicPlacement(QVector<QList<int>> graphData, QVector<QPoint> moduleSize) : m_moduleConnectData(
        graphData), m_size(moduleSize) {
    // passed
    preHandleData();

    // bug 数组越界 Fix
    computePosition();
}

SchematicPlacement::~SchematicPlacement() {

}
