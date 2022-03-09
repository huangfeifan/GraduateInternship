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

QVector<QPoint> SchematicPlacement::getModulePos() {
    return m_modulePos;
}

QVector<QPoint> SchematicPlacement::getLeftPortPos() {
    // 左侧输入port位置
    return m_leftPortPos;
}

QVector<QPoint> SchematicPlacement::getRightPortPos() {
    // 右侧输出port位置
    return m_rightPortPos;
}

QVector<QVector<int>> SchematicPlacement::getModulePortPos() {
    // 每个module的port位置
    return m_modulePortPos;
}

void SchematicPlacement::printInfo() {
    qDebug() << "****************************************************";
    qDebug() << m_sccs << "  SCC";
    for (int i = 0; i < m_sccsInfo.size(); ++i) {
        qDebug() << "Index   " << i;
        qDebug() << m_sccsInfo[i].absolutePos << "  absolutePos";
        qDebug() << m_sccsInfo[i].relativePos << "  relativePos";
        qDebug() << m_sccsInfo[i].graph << "  graph";
        qDebug() << m_sccsInfo[i].sccIndexHash << "  sccIndexHash";
        qDebug() << m_sccsInfo[i].sccSize << "  sccSize";
        qDebug() << m_sccsInfo[i].moduleSize << "  moduleSize";
        qDebug() << "****************************************************";
    }
}

void SchematicPlacement::adjustRelativePos(QVector<QPoint> &relativePos) {
// 最后两列只有一个module 且都只包含一个module 则将最后一列的module放在倒数第二列
    QVector<QList<int>> columnIndexList = QVector<QList<int>>(relativePos.size());
    QVector<QHash<int, int>> isPosOccupied = QVector<QHash<int, int>>(relativePos.size());
    for (int i = 0; i < relativePos.size(); ++i) {
        int x = relativePos[i].x();// column
        int y = relativePos[i].y();// row
        isPosOccupied[x].insert(y, y);
    }

    // 初始化数据
    for (int i = 0; i < relativePos.size(); ++i) {
        QPoint point = relativePos[i];// .x()列 .y()行
        columnIndexList[point.x()].push_back(i);// 存放索引index
    }

    int maxModuleCount = 0;// 按列划分 模块数的最大值
    for (int i = 0; i < columnIndexList.size(); ++i) {
        maxModuleCount = maxModuleCount > columnIndexList[i].size() ? maxModuleCount : columnIndexList[i].size();
    }

    if (maxModuleCount > 1) {
        // 最后两列仅有一个 则将其放在其他列
        int temp = relativePos.size() - 1;
        while (columnIndexList[temp].size() == 0) {
            temp--;
        }
        if (columnIndexList[temp].size() == 1) {// 从最后一列往前面列探索
            int index = columnIndexList[temp][0];
            int column = temp - 1;
            // 调整单个模块位置
            int row = relativePos[index].y();
            while (isPosOccupied[column].contains(row)) {
                row++;
            }
            relativePos[index].setX(column);
            relativePos[index].setY(row);
        }
    }
}

void SchematicPlacement::computeAllModulePos() {
    //qDebug() << "compute All Module Pos------------------------------------";
    //qDebug() << m_modulePos;
    // 相对位置的计算
    for (int i = 0; i < m_modulePos.size(); ++i) {
        QPoint point = m_modulePos[i];
        m_modulePos[i] = QPoint(point.x() / 5, point.y() / 5);
    }
    qDebug() << m_modulePos << " All module Relative Pos";
    // 调整相对位置
    adjustRelativePos(m_modulePos);

    // 保存相对位置
    m_moduleRelativePos = m_modulePos;

    ComputeAbsolutePos pos(m_moduleConnectData, m_size, m_modulePos, 1, 30, 50);
    m_modulePos = pos.getAbsolutePos();
}

void SchematicPlacement::computePosition() {
    m_modulePos = QVector<QPoint>(m_moduleConnectData.size());

    /// 计算模块的绝对位置
    ///     Step1.计算强连通分支间的相对位置
    ///     Step2.计算每个强连通分支内部的相对位置和绝对位置
    ///     Step3.计算强连通分支间的绝对位置
    ///     Step4.计算单独port的绝对位置

    // Not Pass bug     Fix     Time:2022.0228
    computeSccsRelativePosition();// 计算强连通分支间的相对位置

    computeASccPosition();// 计算每个强连通分支内部的相对位置

    //printInfo()
    // bug 疑难杂症 Fix Time:2022.0228
    // 计算SccInfo 需要注意
    computeSccsAbsolutePosition();// 计算强连通分支间的绝对位置

    // 计算每个模块的相对位置
    computeASccInnerPosition();

    // 计算每个模块的绝对位置
    computeAllModulePos();

    // 调整模块的位置 使得模块间距小一些
    adjustModulePos();

    // todo add
    computePortPos();// 计算单独port的绝对位置
}

void SchematicPlacement::computeASccInnerPosition() {
//qDebug() << "------------------------------------------------------compute A SccInner Position";

    /// 计算每个强连通分支内部结点的绝对位置
    // 根据强连通分支间 以及 强连通分支内部的绝对位置来计算出每个module的最终位置
    for (int i = 0; i < m_sccs.size(); ++i) {
        for (int j = 0; j < m_sccs[i].size(); ++j) {
            // 模块index
            int index = m_sccs[i][j];// 原index
            int newIndex = m_sccsInfo[i + 1].sccIndexHash.value(index);
            // bug solved
            QPoint absolutePos = m_sccsInfo[i + 1].absolutePos[newIndex];
            // bug solved
            //qDebug() << m_sccsInfo[0].absolutePos.size() << " " << i;
            QPoint sccIPos = m_sccsInfo[0].absolutePos[i];
            m_modulePos[index] = QPoint(sccIPos.x() + absolutePos.x(), sccIPos.y() + absolutePos.y());
        }
    }
    //qDebug() << m_modulePos << " ModulePos";
    //qDebug() << "------------------------------------------------------compute A SccInner Position End\n\n";
}

void SchematicPlacement::computeASccPosition() {
    /// 计算强连通分支内部的相对位置和绝对位置
    // pass
    for (int i = 0; i < m_sccs.size(); ++i) {
        if (m_sccs[i].size() > 1) {
            /// 该强连通分量不止一个结点 则考虑连通分支内的模块的位置的计算

            int index = i + 1;
            // 计算相对位置并保存
            PlaceAScc placeAScc(m_sccsInfo[index].graph);
            m_sccsInfo[index].relativePos = placeAScc.getRelativePos();
            //qDebug() << m_sccsInfo[index].relativePos << "     relative Pos";

            int width = 0;
            int height = 0;
            QVector<QPoint> size = m_sccsInfo[index].relativePos;
            for (int j = 0; j < size.size(); ++j) {
                width = width > size[j].x() ? width : size[j].x();
                height = height > size[j].y() ? height : size[j].y();
                size[j].setX(size[j].x() * 5);
                size[j].setY(size[j].y() * 5);
            }
            //qDebug() << size;

            //qDebug() << width << " width  height" << height;
            // 计算绝对位置并保存
            m_sccsInfo[index].absolutePos = size;
            // 更新单个强连通分支的大小
            m_sccsInfo[index].sccSize = QPoint((width + 1) * 5, (height + 1) * 5);

/*                // 计算绝对位置并保存
                ComputeAbsolutePos absolutePos(m_sccsInfo[index].graph, size,
                                               m_sccsInfo[index].relativePos, m_grid, 0, 0);
                m_sccsInfo[index].absolutePos = absolutePos.getAbsolutePos();
                // 更新单个强连通分支的大小
                m_sccsInfo[index].sccSize = absolutePos.getSccBlockSize();*/

            //qDebug() << index << "  absolutePos  " << m_sccsInfo[index].absolutePos;
            //qDebug() << index << "  sccSize  " << m_sccsInfo[index].sccSize;
        }
    }
}

void SchematicPlacement::computeSccsAbsolutePosition() {
    /// 计算强连通分支间的绝对位置
    //qDebug() << "\n\n------------------------------------------------------compute Sccs AbsolutePosition";
    // 每个强连通分支的大小
    QVector<QPoint> moduleSize = QVector<QPoint>(m_sccs.size());

    for (int i = 0; i < m_sccs.size(); ++i) {
        if (m_sccs[i].size() == 1) {
            // 为了计算相对位置 以5为基本大小单位
            moduleSize[i] = QPoint(5, 5);
        } else {
            // 一个最大强连通分支的大小
            moduleSize[i] = m_sccsInfo[i + 1].sccSize;
        }
    }
    // pass
    //qDebug() << moduleSize << " ModuleSize----Sccs";
    QVector<QList<int>> graph;//    m_sccsInfo[0].graph
    ComputeAbsolutePos absolutePos(m_sccsInfo[0].graph, moduleSize,
                                   m_sccsInfo[0].relativePos, m_grid, 0, 0);
    m_sccsInfo[0].absolutePos = absolutePos.getAbsolutePos();
    m_sccsInfo[0].sccSize = absolutePos.getSccBlockSize();

    // pass
    //qDebug() << "\n\n  absolutePos  " << m_sccsInfo[0].absolutePos;
    //qDebug() << "  sccSize  " << m_sccsInfo[0].sccSize;

    //qDebug() << m_sccsInfo[0].absolutePos.size() << " ";
    //qDebug() << "------------------------------------------------------compute Sccs AbsolutePosition End\n\n";
}

void SchematicPlacement::initSccsInfo() {
    // 初始化m_sccsConnectData和m_sccsInfo

    m_moduleSccIndex = QVector<int>(m_moduleConnectData.size());// 模块所在的sccIndex
    m_sccsConnectData = QVector<QList<QPoint >>(m_sccs.size() + 1);
    m_sccsInfo = QVector<ASccInfo>(m_sccs.size() + 1);

    QHash<int, int> sccHash;// 强连通分支间的hash
    for (int i = 0; i < m_sccs.size(); ++i) {
        sccHash.insert(m_sccs[i][0], i);// 原index 新index

        QVector<QPoint> moduleSize = QVector<QPoint>(m_sccs[i].size());
        QHash<int, int> sccIndexHash;// 根据原index找在强连通分支的新index
        for (int j = 0; j < m_sccs[i].size(); ++j) {
            // 原index = m_sccs[i][j] 在第i个强连通分支中
            m_moduleSccIndex[m_sccs[i][j]] = i;

            sccIndexHash.insert(m_sccs[i][j], j);// 原来的index 新index
            // 模块大小
            moduleSize[j] = m_size[m_sccs[i][j]];
        }

        m_sccsInfo[i + 1].sccSize = moduleSize[0];// 先初始化一个sccSize 后续会修改
        m_sccsInfo[i + 1].absolutePos = QVector<QPoint>(m_sccs[i].size());
        m_sccsInfo[i + 1].relativePos = QVector<QPoint>(m_sccs[i].size());
        m_sccsInfo[i + 1].sccIndexHash = sccIndexHash;
        // pass
        //qDebug() << sccIndexHash << "---QVector<ASccInfo>";
        m_sccsInfo[i + 1].graph = QVector<QList<int>>(m_sccs[i].size());
        m_sccsInfo[i + 1].moduleSize = moduleSize;
    }

    m_sccsInfo[0].sccSize = QPoint(0, 0);// 先初始化一个sccSize 后续会修改
    m_sccsInfo[0].absolutePos = QVector<QPoint>(m_sccs.size());
    m_sccsInfo[0].relativePos = QVector<QPoint>(m_sccs.size());
    m_sccsInfo[0].sccIndexHash = sccHash;
    //qDebug() << sccHash << "---***";
    m_sccsInfo[0].graph = QVector<QList<int>>(m_sccs.size());
    // 差graph

    // success
    for (int i = 0; i < m_moduleConnectData.size(); ++i) {
        for (int j = 0; j < m_moduleConnectData[i].size(); ++j) {
            int end = m_moduleConnectData[i][j];// endModuleIndex
            int sccIndex = m_moduleSccIndex[i];// 起点i所在的sccIndex
            int endSccIndex = m_moduleSccIndex[end];
            if (sccIndex == endSccIndex) {
                /// 起点i 和 终点 m_moduleConnectData[i][j] 在同一个scc中 则考虑将该connect添加到m_sccConnectData中
                //qDebug() << m_moduleSccIndex[i] << "-----";
                m_sccsConnectData[sccIndex + 1].push_back(QPoint(i, end));
                //m_sccsConnectData[]
            } else {
                m_sccsConnectData[0].push_back(QPoint(m_sccs[sccIndex][0], m_sccs[endSccIndex][0]));
            }
        }
    }

    // pass
    // 根据原来的连接关系 计算新索引下强连通分支间、单个强连通分析的连接关系
    for (int i = 0; i < m_sccsConnectData.size(); ++i) {
        for (int j = 0; j < m_sccsConnectData[i].size(); ++j) {
            // 原 起点m_sccsConnectData[i][j].x() 终点m_sccsConnectData[i][j].y()
            int start = m_sccsInfo[i].sccIndexHash.value(m_sccsConnectData[i][j].x());
            int end = m_sccsInfo[i].sccIndexHash.value(m_sccsConnectData[i][j].y());
            //qDebug() << m_sccsInfo[i].sccIndexHash;
            //qDebug() << " before " << m_sccsConnectData[i][j].x() << m_sccsConnectData[i][j].y() << " after "
            //<< start << " " << end;
            m_sccsInfo[i].graph[start].push_back(end);
        }
    }
}

SchematicPlacement::SchematicPlacement(QList<ConnectData> connectData, QVector<QPoint> moduleSize) {
    m_connectData = connectData;
    m_size = moduleSize;
}

SchematicPlacement::SchematicPlacement(QList<ConnectData> connectData) {
    m_connectData = connectData;
    // 邻接表的计算
    computeModuleConnectData();


}

void SchematicPlacement::computeModuleSize() {
    /// 计算模块大小 根据输入输出个数() 高度统一(参数)

    QVector<QPoint> size = QVector<QPoint>(m_moduleCount);
    QVector<int> inDegree = QVector<int>(m_moduleCount);
    QVector<int> outDegree = QVector<int>(m_moduleCount);
    for (int i = 0; i < m_moduleConnectData.size(); ++i) {
        // 出度
        outDegree[i] = m_moduleConnectData[i].size();
        int end;
        for (int j = 0; j < m_moduleConnectData[i].size(); ++j) {
            end = m_moduleConnectData[i][j];
            inDegree[end]++;
        }
    }
    const int WIDTH = 30;
    int height = 10;// 一个port 20 两个port30 依次类推
    for (int i = 0; i < m_moduleCount; ++i) {
        m_size[i].setX(WIDTH);
        int temp = inDegree[i] > outDegree[i] ? inDegree[i] : outDegree[i];
        m_size[i].setY((temp + 1) * height);
    }
}

QVector<QPoint> SchematicPlacement::getModuleRelativePos() {
    return m_moduleRelativePos;
}

void SchematicPlacement::computeScc() {
    GetGraphSccs tarjanAlgo(m_moduleConnectData);
    m_sccs = tarjanAlgo.getGraphAllScc();
}

void SchematicPlacement::computeModuleConnectData() { // pass
    for (int i = 0; i < m_connectData.size(); ++i) {
        int startModule = m_connectData[i].startModuleIndex;
        int endModule = m_connectData[i].endModuleIndex;

        if (endModule == -1 || startModule == -1) {
            // 输入为单独的port
            // 输入为单独的port
            continue;
        }
        m_moduleConnectData[startModule].push_back(endModule);
    }
}
