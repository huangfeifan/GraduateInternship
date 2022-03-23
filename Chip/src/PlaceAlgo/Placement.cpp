//
// Created by Huangff on 2022/2/11.
//

#include "Placement.h"
#include "Data.h"

Placement::Placement(QVector<QList<int>> graphData, QVector<ModuleInfo> moduleInfo) : m_moduleConnectData(
        graphData), m_moduleInfo(moduleInfo) {
    ///// 1.数据预处理
    ///// 2.计算强连通分支间的相对位置
    ///// 3.计算强连通内部的相对位置和绝对位置
    ///// 4.计算每个模块的绝对位置
    ///// 5.计算单独port的绝对位置
    ///// 全剧终
    //
    //// passed
    //preHandleData();
    //
    //// bug 数组越界 Fix Time: 0228  计算模块绝对位置
    //computePosition();  // 2,3,4
    //
    //// 初始化port相关数据
    //initPortRelatedInfo();
    //
    //// 计算port的绝对位置
    //computePortPos();
}

Placement::Placement(QList<ConnectData> connectData, QVector<ModuleInfo> moduleInfo, int leftPortNum, int rightPortNum)
        : m_connectData(connectData),
          m_moduleInfo(moduleInfo), m_leftPortNum(leftPortNum), m_rightPortNum(rightPortNum) {
    // 计算模块大小
    computeModuleSize();

    /// 1.数据预处理
    /// 2.计算强连通分支间的相对位置
    /// 3.计算强连通内部的相对位置和绝对位置
    /// 4.计算每个模块的绝对位置
    /// 5.计算单独port的绝对位置
    /// 全剧终

    // passed
    preHandleData();

    // bug 数组越界 Fix Time: 0228  计算模块绝对位置
    computePosition();  // 2,3,4

    // 初始化port相关数据
    initPortRelatedInfo();

    // 计算port的绝对位置
    computePortPos();
}

Placement::Placement(QList<ConnectData> connectData) : m_connectData(connectData) {
    // 邻接表的计算
    computeModuleConnectData();

}

QVector<QPoint> Placement::getModulePos() {
    return m_modulePos;
}

QVector<QPoint> Placement::getLeftPortPos() {
    // 左侧输入port位置
    return m_leftPortPos;
}

QVector<QPoint> Placement::getRightPortPos() {
    // 右侧输出port位置
    return m_rightPortPos;
}

QVector<QVector<QPoint>> Placement::getModulePortPos() {
    // 每个module的port位置
    return m_modulePortPos;
}

void Placement::printInfo() {
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

void Placement::adjustRelativePos(QVector<QPoint> &relativePos) {
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

void Placement::computeAllModulePos() {
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

    QVector<QPoint> size = QVector<QPoint>(m_moduleInfo.size());
    for (int i = 0; i < m_moduleInfo.size(); ++i) {
        size[i] = m_moduleInfo[i].size;
    }

    ComputeAbsolutePos pos(m_moduleConnectData, size, m_modulePos, 1, 30, 50);
    m_modulePos = pos.getAbsolutePos();
}

void Placement::computePosition() {
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
    computePortPos();// 计算所有port的绝对位置
}

void Placement::computeASccInnerPosition() {
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

void Placement::computeASccPosition() {
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

            //qDebug() << width << " width  weight" << weight;
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

void Placement::computeSccsAbsolutePosition() {
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

void Placement::initSccsInfo() {
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
            moduleSize[j] = m_moduleInfo[m_sccs[i][j]].size;
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


void Placement::computeModuleSize() {
    /// 计算模块大小 根据输入输出个数() 高度统一(参数)

    if (m_moduleInfo.size() && m_moduleInfo[0].size.x() > 0) {
        return;
    }

    const int WIDTH = 30;
    int height = 10;// 一个port 20 两个port30 依次类推
    for (int i = 0; i < m_moduleInfo.size(); ++i) {
        m_moduleInfo[i].size.setX(WIDTH);
        int temp = m_moduleInfo[i].inDegree > m_moduleInfo[i].outDegree ? m_moduleInfo[i].inDegree
                                                                        : m_moduleInfo[i].outDegree;
        m_moduleInfo[i].size.setY((temp + 1) * height);
    }
}

QVector<QPoint> Placement::getModuleRelativePos() {
    return m_moduleRelativePos;
}

void Placement::computeScc() {
    GetGraphSccs tarjanAlgo(m_moduleConnectData);
    m_sccs = tarjanAlgo.getGraphAllScc();
}

void Placement::computeModuleConnectData() { // pass
    for (int i = 0; i < m_connectData.size(); ++i) {
        int startModule = m_connectData[i].startModuleIndex;
        int endModule = m_connectData[i].endModuleIndex;

        if (startModule == -1) {
            // 左侧单独的输入port
            m_leftPortConnectData.push_back(m_connectData[i]);
            continue;
        }

        if (endModule == -1) {
            // 右侧单独的输出port
            m_rightPortConnectData.push_back(m_connectData[i]);
            continue;
        }

        m_moduleConnectData[startModule].push_back(endModule);
    }
}

Placement::Placement(QVector<QPoint> moduleDegree, QList<ConnectData> connectData) : m_connectData(connectData) {
    computeModuleSize();
}

void Placement::adjustModulePos() {
    // 根据俄罗斯方块调整最终位置
}

void Placement::computePortPos() {
    /// 用于计算所有port的位置

    // 计算属于模块的port的位置
    computeModulePortPos();

    // 计算左侧单独的port位置
    computeLeftPortPos();

    // 计算右侧单独的port位置
    computeRightPortPos();

}

void Placement::computeSccsRelativePosition() {
    /// 计算强连通分支间的相对位置
    // 存放在m_sccsInfo[0].relativePos中
    // connect数据
    //qDebug() << m_sccsConnectData[0];// 原连接数据
    //qDebug() << m_sccsInfo[0].graph;// 新索引下的连接数据
    PlaceSccs placeSccs(m_sccsInfo[0].graph);
    // 保存相对位置数据
    m_sccsInfo[0].relativePos = placeSccs.getRelativePos();
}

void Placement::preHandleData() {
    /// 数据的预处理
    //      1.计算强连通分支
    //      2.初始化m_sccsConnectData和m_sccsInfo

    computeScc();
    initSccsInfo();
}

void Placement::computeModulePortPos() {
    for (int i = 0; i < m_connectData.size(); ++i) {
        int startModule = m_connectData[i].startModuleIndex;
        int endModule = m_connectData[i].endModuleIndex;

        if (startModule == -1 || endModule == -1) {
            continue;
        }

        int startPort = m_connectData[i].startPortIndex;
        int endPort = m_connectData[i].endPortIndex;
        // 设置模块的port的权重  port放在左右 则考虑y当作权重 port放在上下 考虑x当作权重
        // Todo extend
        m_modulePort[startModule][startPort].weight = m_moduleRelativePos[endModule].y();
        m_modulePort[endModule][endPort].weight = m_moduleRelativePos[startModule].y();
    }

    // 对模块的port进行排序 四个方向基于权重排序并计算绝对位置
    for (int i = 0; i < m_modulePort.size(); ++i) {
        QList<WeightAndIndex> left;
        QList<WeightAndIndex> bottom;
        QList<WeightAndIndex> right;
        QList<WeightAndIndex> top;
        for (int j = 0; j < m_modulePort[i].size(); ++j) {
            WeightAndIndex temp(m_modulePort[i][j].weight, j);
            switch (m_modulePort[i][j].Direction) {
                // 0,1,2,3 左下右上
                case 0:
                    left.push_back(temp);
                    break;
                case 1:
                    bottom.push_back(temp);
                    break;
                case 2:
                    right.push_back(temp);
                    break;
                case 3:
                    top.push_back(temp);
                    break;
                default:
                    qDebug() << "Error Switch Case!";
            }
        }

        qSort(left.begin(), left.end(), compareWeight);
        qSort(bottom.begin(), bottom.end(), compareWeight);
        qSort(right.begin(), right.end(), compareWeight);
        qSort(top.begin(), top.end(), compareWeight);

        for (int j = 0; j < left.size(); ++j) {
            int index = left[i].index;
            m_modulePort[i][index].orderIndex = j;
            m_modulePort[i][index].point = QPoint(0, j * m_grid + m_grid) + m_modulePos[i];
        }
        for (int j = 0; j < bottom.size(); ++j) {
            int index = bottom[i].index;
            m_modulePort[i][index].orderIndex = j;
            m_modulePort[i][index].point = QPoint(j * m_grid + m_grid, m_moduleInfo[i].size.y()) + m_modulePos[i];
        }

        for (int j = 0; j < right.size(); ++j) {
            int index = right[i].index;
            m_modulePort[i][index].orderIndex = j;
            m_modulePort[i][index].point = QPoint(m_moduleInfo[i].size.x(), j * m_grid + m_grid) + m_modulePos[i];
        }

        for (int j = 0; j < top.size(); ++j) {
            int index = top[i].index;
            m_modulePort[i][index].orderIndex = j;
            m_modulePort[i][index].point = QPoint(0, j * m_grid + m_grid) + m_modulePos[i];
        }
    }

}

void Placement::computeLeftPortPos() {
    QHash<int, int> leftPos;
    for (int i = 0; i < m_leftPortConnectData.size(); ++i) {
        int startPort = m_leftPortConnectData[i].startPortIndex;
        m_leftInputPort[startPort].point = QPoint();
    }
    // place unplaced port
    for (int i = 0; i < m_leftPortNum; ++i) {
        //if (m_leftInputPort[i].)
        // todo add something
    }

}

void Placement::computeRightPortPos() {

}

void Placement::initPortRelatedInfo() {
    m_leftInputPort = QVector<PortInfo>(m_leftPortNum);
    m_rightOutputPort = QVector<PortInfo>(m_rightPortNum);
    m_modulePort = QVector<QVector<PortInfo>>(m_moduleInfo.size());
    for (int i = 0; i < m_moduleInfo.size(); ++i) {
        m_modulePort[i] = QVector<PortInfo>(m_moduleInfo[i].inDegree + m_moduleInfo[i].outDegree);
    }
}
