//
// Created by Huangff on 2022/2/11.
//

#include "Placement.h"
//#include "MyWidget/Data.h"

Placement::Placement(QVector<QList<int>> graphData, QVector<QVector<int>> modulePortInfo) : m_moduleConnectData(
        graphData) {
    ///// 1.数据预处理
    ///// 2.计算sccs的相对位置
    ///// 3.计算sccInner的相对位置和绝对位置
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

Placement::Placement(QList<ConnectData> connectData, const QVector<QVector<int>> &modulePortInfo, int leftPortNum,
                     int rightPortNum) : m_connectData(connectData), m_leftPortNum(leftPortNum),
                                         m_rightPortNum(rightPortNum), m_modulePortInfo(modulePortInfo) {
    // 数据预处理
    preHandleData();

    //计算模块绝对位置
    computePosition();  // 2,3,4

    // 偏移
    adjustAllPos();
}

Placement::Placement(QList<ConnectData> connectData, QVector<QPoint> moduleSize) : m_connectData(connectData),
                                                                                   m_moduleSize(moduleSize) {

}

QVector<QPoint> Placement::getModulePos() {
    //qDebug() << m_moduleRelativePos << "PLACEMENT_MODULE_POS_RELATIVE";
    //qDebug() << m_modulePos << "PLACEMENT_MODULE_POS";
    return m_modulePos;
}

QVector<QPoint> Placement::getLeftInputPortPos() {
    // 左侧输入port位置
    return m_leftPortPos;
}

QVector<QPoint> Placement::getRightOutputPortPos() {
    // 右侧输出port的位置
    return m_rightPortPos;
}

QVector<QVector<QPoint>> Placement::getModulePortPos() {
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
    // 调整相对位置 计算所有模块的绝对位置
    for (int i = 0; i < m_modulePos.size(); ++i) {
        QPoint point = m_modulePos[i];
        m_modulePos[i] = QPoint(point.x() / 5, point.y() / 5);
    }

    // 调整相对位置 error ??
    //adjustRelativePos(m_modulePos);

    // 保存相对位置
    m_moduleRelativePos = m_modulePos;
    //qDebug() << m_moduleRelativePos << " Relative_Pos";

    // 针对计算出的模块相对位置有重合进行微调
    getDistinctRelativePos();
    //qDebug() << m_moduleRelativePos << " After_Adjust_Relative_Pos";
    //qDebug() << m_moduleConnectData << "ConnectData";
    //qDebug() << m_moduleSize << "ModuleSize";
    //qDebug() << m_modulePos << "ModulePos";

    ComputeAbsolutePos pos(m_moduleConnectData, m_moduleSize, m_moduleRelativePos, 20 * GRID, 20 * GRID);// 作为参数
    m_modulePos = pos.getAbsolutePos();
    //qDebug() << m_modulePos << " Module_Pos";

}

void Placement::computePosition() {

    computeSccsRelativePosition();// 计算sccs的相对位置

    computeASccPosition();// 计算sccInner的相对位置

    //printInfo()

    computeSccsAbsolutePosition();// 计算sccs的绝对位置

    computeASccInnerPosition();// 计算强sccInner的绝对位置

    computeAllModulePos();// 调整相对位置 计算所有模块的绝对位置

    //adjustModulePos(); // 调整模块的位置 使得模块间距小一些 // todo implement

    computePortPos();// 计算所有port的绝对位置

}

void Placement::computeASccInnerPosition() {

    /// 计算sccInner的绝对位置
    // 根据sccs 以及 sccInner的绝对位置来计算出每个module的最终位置
    for (int i = 0; i < m_sccs.size(); ++i) {
        for (int j = 0; j < m_sccs[i].size(); ++j) {
            // 模块index
            int index = m_sccs[i][j];// 原index
            int newIndex = m_sccsInfo[i + 1].sccIndexHash.value(index);
            QPoint absolutePos = m_sccsInfo[i + 1].absolutePos[newIndex];
            QPoint sccIPos = m_sccsInfo[0].absolutePos[i];
            m_modulePos[index] = sccIPos + absolutePos;
            //QPoint(sccIPos.x() + absolutePos.x(), sccIPos.y() + absolutePos.y());
        }
    }
    //   qDebug() << m_modulePos << " Module_Pos";
}

void Placement::computeASccPosition() {
    ///  计算sccInner的相对位置
    // pass
    for (int i = 0; i < m_sccs.size(); ++i) {
        if (m_sccs[i].size() > 1) {
            /// 该scc不止一个结点 则考虑sccInner的模块的位置的计算

            int index = i + 1;
            // 计算相对位置并保存
            PlaceAScc placeAScc(m_sccsInfo[index].graph);
            m_sccsInfo[index].relativePos = placeAScc.getRelativePos();
            //qDebug() << m_sccsInfo[index].graph << " GRAPH";
            //qDebug() << m_sccsInfo[index].relativePos << "RELATIVE_POS";

            int width = 0;
            int height = 0;
            QVector<QPoint> size = m_sccsInfo[index].relativePos;
            for (int j = 0; j < size.size(); ++j) {
                width = width > size[j].x() ? width : size[j].x();
                height = height > size[j].y() ? height : size[j].y();
                size[j].setX(size[j].x() * 5);
                size[j].setY(size[j].y() * 5);
            }

            // 计算绝对位置并保存
            m_sccsInfo[index].absolutePos = size;
            // 更新scc的大小
            m_sccsInfo[index].sccSize = QPoint((width + 1) * 5, (height + 1) * 5);
        }
    }
}

void Placement::computeSccsAbsolutePosition() {
    /// 计算sccs的绝对位置
    // 一个scc的大小
    QVector<QPoint> moduleSize = QVector<QPoint>(m_sccs.size());

    for (int i = 0; i < m_sccs.size(); ++i) {
        if (m_sccs[i].size() == 1) {
            // 为了计算相对位置 以5为基本大小单位
            moduleSize[i] = QPoint(5, 5);
        } else {
            // a scc的大小
            moduleSize[i] = m_sccsInfo[i + 1].sccSize;
        }
    }
    QVector<QList<int>> graph;//    m_sccsInfo[0].graph
    //qDebug() << moduleSize << "ModuleSize";
    // Bug -- todo consider
    ComputeAbsolutePos absolutePos(m_sccsInfo[0].graph, moduleSize,
                                   m_sccsInfo[0].relativePos, 0, 0);
    m_sccsInfo[0].absolutePos = absolutePos.getAbsolutePos();
    //qDebug() << m_sccsInfo[0].absolutePos << "Absolute_Pos";
    m_sccsInfo[0].sccSize = absolutePos.getSccBlockSize();

}

void Placement::initSccsInfo() {
    // 初始化m_sccsConnectData和m_sccsInfo

    m_sccsConnectData = QVector<QList<QPoint >>(m_sccs.size() + 1);
    m_sccsInfo = QVector<ASccInfo>(m_sccs.size() + 1);

    QHash<int, int> sccHash;// sccs的hash
    for (int i = 0; i < m_sccs.size(); ++i) {
        sccHash.insert(m_sccs[i][0], i);// 原index 新index

        QVector<QPoint> moduleSize = QVector<QPoint>(m_sccs[i].size());
        QHash<int, int> sccIndexHash;// 根据原index找在sccInner的新index
        for (int j = 0; j < m_sccs[i].size(); ++j) {
            // 原index = m_sccs[i][j] 在第i个scc中
            m_moduleSccIndex[m_sccs[i][j]] = i;

            sccIndexHash.insert(m_sccs[i][j], j);// 原来的index 新index
            // 模块大小
            moduleSize[j] = m_moduleSize[m_sccs[i][j]];
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
                m_sccsConnectData[sccIndex + 1].push_back(QPoint(i, end));
            } else {
                m_sccsConnectData[0].push_back(QPoint(m_sccs[sccIndex][0], m_sccs[endSccIndex][0]));
            }
        }
    }

    // pass
    // 根据原来的连接关系 计算新索引下sccs和sccInner的连接关系
    for (int i = 0; i < m_sccsConnectData.size(); ++i) {
        for (int j = 0; j < m_sccsConnectData[i].size(); ++j) {
            // 原 起点m_sccsConnectData[i][j].x() 终点m_sccsConnectData[i][j].y()
            int start = m_sccsInfo[i].sccIndexHash.value(m_sccsConnectData[i][j].x());
            int end = m_sccsInfo[i].sccIndexHash.value(m_sccsConnectData[i][j].y());
            m_sccsInfo[i].graph[start].push_back(end);
        }
    }
}

void Placement::computeModuleSize() {
    /// 计算模块大小 根据输入输出个数() 高度统一(参数)
    m_moduleSize = QVector<QPoint>(m_moduleCount);
    const int WIDTH = 4 * GRID;// 一个port 2*GRID 两个port 3*GRID 依次类推

    for (int i = 0; i < m_moduleCount; ++i) {
        int leftCount = 0;
        int rightCount = 0;
        int topCount = 0;// 目前只支持两个方向
        int bottomCount = 0;
        // 0表示leftInputPort 1表示rightOutputPort
        for (int j = 0; j < m_modulePortInfo[i].size(); ++j) {
            if (m_modulePortInfo[i][j] == 0) {
                leftCount++;
            } else {
                rightCount++;
            }
        }
        int temp = leftCount > rightCount ? leftCount : rightCount;
        m_moduleSize[i] = QPoint(WIDTH, temp * GRID + GRID);
    }
    //qDebug() << "-----------------";
}

QVector<QPoint> Placement::getModuleRelativePos() {
    return m_moduleRelativePos;
}

void Placement::computeScc() {
    //qDebug() << m_moduleConnectData << "PLACEMENT_MODULE_CONNECT_DATA";
    GetGraphSccs tarjanAlgo(m_moduleConnectData);
    m_sccs = tarjanAlgo.getGraphAllScc();
    //qDebug() << m_sccs << "PLACEMENT_SCC";
}

void Placement::computeModuleConnectData() { // pass
    // 计算邻接表

    for (int i = 0; i < m_connectData.size(); ++i) {
        int startModule = m_connectData[i].startModuleIndex;
        int endModule = m_connectData[i].endModuleIndex;

        // 检测输入数据是否异常
        int startPortIndex = m_connectData[i].startPortIndex;
        int endPortIndex = m_connectData[i].endPortIndex;

        if (startModule == -1) {
            // 左侧单独的输入port
            m_leftPortConnectData.push_back(m_connectData[i]);
            if (startPortIndex < 0 || startPortIndex > m_leftPortNum) {
                qDebug() << "Data Error!";// 输入数据有误
                return;
            }
            continue;
        } else {
            if (startPortIndex < 0 || startModule < 0 ||
                startModule > m_moduleCount || startPortIndex > m_modulePortInfo[startModule].size()) {
                qDebug() << "Data Error!";// 输入数据有误
                return;
            }
        }

        if (endModule == -1) {
            // 右侧单独的输出port
            m_rightPortConnectData.push_back(m_connectData[i]);

            if (endPortIndex < 0 || endPortIndex > m_rightPortNum) {
                qDebug() << "Data Error!";// 输入数据有误
                return;
            }
            continue;
        } else {
            if (endPortIndex < 0 || endModule < 0 ||
                endModule > m_moduleCount || endPortIndex > m_modulePortInfo[endModule].size()) {
                qDebug() << "Data Error!";// 输入数据有误
                return;
            }
        }

        m_moduleConnectData[startModule].push_back(endModule);
        //
    }   //qDebug() << m_moduleConnectData;// pass

    //qDebug() << "adjacencyList" << m_moduleConnectData;
}

Placement::Placement(QList<ConnectData> connectData) : m_connectData(connectData) {
    computeModuleSize();
}

void Placement::adjustModulePos() {
    // 根据俄罗斯方块调整最终位置
}

void Placement::computePortPos() {
    /// 计算所有port的位置

    // 计算左侧和右侧port的X坐标
    computeXPos();

    // 计算属于模块的port的位置
    computeModulePortPos();

    // 计算左侧单独的port位置 error
    computeLeftPortPos();

    // 计算右侧单独的port位置
    computeRightPortPos();

}

void Placement::computeSccsRelativePosition() {
    /// 计算sccs的相对位置

    PlaceSccs placeSccs(m_sccsInfo[0].graph);
    // 保存相对位置数据
    m_sccsInfo[0].relativePos = placeSccs.getRelativePos();
    //qDebug() << m_sccsInfo[0].relativePos << "SCCs_Relative_Pos";
}

void Placement::preHandleData() {
    /// 数据的预处理

    initVector();// 初始化数组

    if (m_moduleSize.size() == 0) {
        // 计算模块大小  --有时候需要
        computeModuleSize();
    }

    computeModuleConnectData();    // 邻接表的计算

    computeScc();// 根据邻接表计算scc

    initSccsInfo();// 初始化scc的相关数据

}

void Placement::computeModulePortPos() {

    // 计算模块port的权重
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
    for (int i = 0; i < m_moduleCount; ++i) {
        QList<WeightAndIndex> left;
        QList<WeightAndIndex> bottom;
        QList<WeightAndIndex> right;
        QList<WeightAndIndex> top;
        // todo consider
        for (int j = 0; j < m_modulePort[i].size(); ++j) {

            WeightAndIndex temp;
            temp.weight = m_modulePort[i][j].weight;
            temp.index = j;

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
                    qDebug() << "Error Switch Case! Data Error!";
            }
        }

        qSort(left.begin(), left.end(), compareWeight);
        qSort(bottom.begin(), bottom.end(), compareWeight);
        qSort(right.begin(), right.end(), compareWeight);
        qSort(top.begin(), top.end(), compareWeight);

        // 根据排序结果 计算摆放位置  模块的port往相应方向扇出一个GRID距离
        const int fanoutDistant = 1;
        for (int j = 0; j < left.size(); ++j) {// 从上往下摆放port
            int index = left[j].index;
            //qDebug() << index << "  --index";
            m_modulePort[i][index].orderIndex = j;
            //qDebug() << index << " " << j;
            m_modulePortPos[i][index] = QPoint(-GRID*fanoutDistant, j * GRID + GRID) + m_modulePos[i];
        }
        for (int j = 0; j < bottom.size(); ++j) {
            int index = bottom[j].index;
            m_modulePort[i][index].orderIndex = j;
            m_modulePortPos[i][index] = QPoint(j * GRID + GRID, m_moduleSize[i].y() + GRID*fanoutDistant) + m_modulePos[i];
        }

        for (int j = 0; j < right.size(); ++j) {// 从上往下摆放port
            int index = right[j].index;
            m_modulePort[i][index].orderIndex = j;
            m_modulePortPos[i][index] = QPoint(m_moduleSize[i].x() + GRID*fanoutDistant, j * GRID + GRID) + m_modulePos[i];
        }

        for (int j = 0; j < top.size(); ++j) {
            int index = top[j].index;
            m_modulePort[i][index].orderIndex = j;
            m_modulePortPos[i][index] = QPoint(j * GRID + GRID, -GRID*fanoutDistant) + m_modulePos[i];
        }
    }
}

void Placement::computeLeftPortPos() {

    // 有连接关系的左侧port的索引放到hash中
    QHash<int, int> hash;//<int--portIndex,int--connectIndex>
    for (int i = 0; i < m_leftPortConnectData.size(); ++i) {
        int startPort = m_leftPortConnectData[i].startPortIndex;
        if (hash.contains(startPort)) {
            continue;
        }
        hash.insert(startPort, i);
    }

    // 对有连接关系的port和无连接关系的port分别处理
    QHash<int, int> pos;
    int maxY = m_maxY;
    for (int i = 0; i < m_leftPortNum; ++i) {
        if (hash.contains(i)) {// hash表中是否含有key
            int index = hash.value(i);// 根据index 找到连接关系

            int moduleIndex = m_leftPortConnectData[index].endModuleIndex;
            int modulePortIndex = m_leftPortConnectData[index].endPortIndex;
            int height = m_modulePortPos[moduleIndex][modulePortIndex].y();
            while (pos.contains(height)) {
                height += GRID;
            }
            pos.insert(height, height);
            //qDebug() << m_leftPortXGap << " in hash " << height << " " << index;
            m_leftPortPos[i] = QPoint(m_leftPortXGap, height);
        } else {
            // 单独的port最下面从下往上摆放
            while (pos.contains(maxY)) {
                maxY -= GRID;
            }
            pos.insert(maxY, maxY);
            //qDebug() << m_leftPortXGap << " " << maxY << " " << i;
            m_leftPortPos[i] = QPoint(m_leftPortXGap, maxY);
        }
    }
}

void Placement::computeRightPortPos() {

    QHash<int, int> hash;//<int--portIndex,int--connectIndex>
    for (int i = 0; i < m_rightPortConnectData.size(); ++i) {
        int endPort = m_rightPortConnectData[i].endPortIndex;
        if (hash.contains(endPort)) {
            continue;
        }
        hash.insert(endPort, i);
    }

    QHash<int, int> pos;
    int maxY = m_maxY;
    for (int i = 0; i < m_rightPortNum; ++i) {
        if (hash.contains(i)) {
            int index = hash.value(i);

            int moduleIndex = m_rightPortConnectData[index].startModuleIndex;
            int modulePortIndex = m_rightPortConnectData[index].startPortIndex;
            int height = m_modulePortPos[moduleIndex][modulePortIndex].y();
            while (pos.contains(height)) {
                height += GRID;
            }
            m_rightPortPos[i] = QPoint(m_rightPortXGap, height);
        } else {
            // 单独的port最下面从下往上摆放
            while (pos.contains(maxY)) {
                maxY -= GRID;
            }
            pos.insert(maxY, maxY);
            m_rightPortPos[i] = QPoint(m_rightPortXGap, maxY);
        }
    }
}

int Placement::getSchematicRowCount() {
    return m_graphRowCount;
}

int Placement::getSchematicColumnCount() {
    return m_graphColumnCount;
}

QVector<QPoint> Placement::getModuleSize() {
    return m_moduleSize;
}

void Placement::initVector() {
    // 初始化数组长度
    m_moduleCount = m_modulePortInfo.size();

    /// 中间数据和输出数据的初始化
    // 初始化模块的port数据
    m_moduleSccIndex = QVector<int>(m_moduleCount);// 模块所在的sccIndex
    m_moduleConnectData = QVector<QList<int>>(m_moduleCount);
    m_modulePort = QVector<QVector<PortInfo>>(m_moduleCount);
    m_modulePortPos = QVector<QVector<QPoint>>(m_moduleCount);
    m_moduleDirectionAndOrder = QVector<QVector<QPoint>>(m_moduleCount);

    m_moduleLeftPortNum = QVector<int>(m_moduleCount);// 模块i左侧port个数
    m_moduleRightPortNum = QVector<int>(m_moduleCount);
    m_moduleTopPortNum = QVector<int>(m_moduleCount);
    m_moduleBottomPortNum = QVector<int>(m_moduleCount);

    for (int i = 0; i < m_moduleCount; ++i) {
        m_modulePort[i] = QVector<PortInfo>(m_modulePortInfo[i].size());
        m_modulePortPos[i] = QVector<QPoint>(m_modulePortInfo[i].size());
        m_moduleDirectionAndOrder[i] = QVector<QPoint>(m_modulePortInfo[i].size());

        // 根据portInfo 修改modulePort的数据
        for (int j = 0; j < m_modulePortInfo[i].size(); ++j) {
            m_modulePort[i][j].Direction = m_modulePortInfo[i][j];
            switch (m_modulePort[i][j].Direction) {
                case 0:// 左
                    m_moduleLeftPortNum[i]++;
                    break;
                case 1:// 下
                    m_moduleBottomPortNum[i]++;
                    break;
                case 2:// 右
                    m_moduleRightPortNum[i]++;
                    break;
                case 3:// 上
                    m_moduleTopPortNum[i]++;
                    break;
                default:
                    break;
            }
        }
    }
//    qDebug() << m_moduleLeftPortNum << "MODULE_LEFT_PORT_NUM";

    // 初始化 左侧输入port  右侧输出port
    m_leftInputPort = QVector<PortInfo>(m_leftPortNum);
    m_rightOutputPort = QVector<PortInfo>(m_rightPortNum);

    // 输出数据
    m_moduleRelativePos = QVector<QPoint>(m_moduleCount);
    m_modulePos = QVector<QPoint>(m_moduleCount);
    m_leftPortPos = QVector<QPoint>(m_leftPortNum);
    m_rightPortPos = QVector<QPoint>(m_rightPortNum);
}

void Placement::adjustAllPos() {
    // 调整坐标

    // 增加 一组参数 往外延伸10%
    // 平移
    double xScaleFactor = 0.2;
    double yScaleFactor = 0.6;
    int minY = INT_MAX;
    int maxY = INT_MIN;
    for (int i = 0; i < m_leftPortNum; ++i) {
        int posY = m_leftPortPos[i].y();
        minY = posY > minY ? minY : posY;
        maxY = posY > maxY ? posY : maxY;
    }

    for (int i = 0; i < m_rightPortNum; ++i) {
        int posY = m_rightPortPos[i].y();
        minY = posY > minY ? minY : posY;
        maxY = posY > maxY ? posY : maxY;
    }
    minY = minY > m_minY ? m_minY : minY;
    maxY = maxY > m_maxY ? maxY : m_maxY;

    int minX = m_leftPortXGap;// minX
    int maxX = m_rightPortXGap;// maxX

    // 设置网格大小
    int height = (maxY - minY);
    int width = (maxX - minX);

    const int gap = 20;
    m_graphRowCount = height * (1 + yScaleFactor) / GRID + gap;
    m_graphColumnCount = width * (1 + xScaleFactor) / GRID + gap;

    // 对三个PortInfo数组做平移并更新输出结果
    int yOffset = abs(minY) * (1 + yScaleFactor / 2) / GRID + gap / 2;
    yOffset *= GRID;
    int xOffset = abs(minX) * (1 + xScaleFactor / 2) / GRID + gap / 2;
    xOffset *= GRID;
    QPoint point(xOffset, yOffset);

    // 每个module的port位置     // 所有坐标数据都是基于网格
    for (int i = 0; i < m_moduleCount; ++i) {
        for (int j = 0; j < m_modulePort[i].size(); ++j) {
            m_modulePortPos[i][j] += point;
            m_modulePortPos[i][j] /= GRID;

            // 存储modulePortDirectionAndOrder
            m_moduleDirectionAndOrder[i][j] = QPoint(m_modulePort[i][j].Direction, m_modulePort[i][j].orderIndex);
        }
        m_modulePos[i] += point;

        m_modulePos[i] /= GRID;
        m_moduleSize[i] /= GRID;
    }

    // leftPort
    for (int i = 0; i < m_leftPortNum; ++i) {
        m_leftPortPos[i] += point;
        m_leftPortPos[i] /= GRID;
    }

    // rightPort
    for (int i = 0; i < m_rightPortNum; ++i) {
        m_rightPortPos[i] += point;
        m_rightPortPos[i] /= GRID;
    }
}

void Placement::computeXPos() {
    // 计算左右侧port的x坐标
    int minX = INT_MAX;
    int maxX = INT_MIN;
    m_maxY = INT_MIN;// 所有模块的竖直方向最大值
    m_minY = INT_MAX;// 所有模块的竖直方向的最小值
    for (int i = 0; i < m_moduleCount; ++i) {
        int posX = m_modulePos[i].x();
        int width = m_moduleSize[i].x() + posX;
        minX = posX > minX ? minX : posX;
        maxX = width > maxX ? width : maxX;

        int posY = m_modulePos[i].y();
        int height = m_moduleSize[i].y() + posY;
        m_minY = m_minY > posY ? posY : m_minY;
        m_maxY = m_maxY > height ? m_maxY : height;
    }
    m_leftPortXGap = minX - m_leftPortXGap;
    m_rightPortXGap += maxX;
}

QVector<QVector<QPoint>> Placement::getModulePortDirectionAndOrder() {
    return m_moduleDirectionAndOrder;
}

QVector<int> Placement::getModuleLeftPortNum() {
    return m_moduleLeftPortNum;
}

QVector<int> Placement::getModuleRightPortNum() {
    return m_moduleRightPortNum;
}

QVector<int> Placement::getModuleTopPortNum() {
    return m_moduleTopPortNum;
}

QVector<int> Placement::getModuleBottomPortNum() {
    return m_moduleBottomPortNum;
}

void Placement::getDistinctRelativePos() {
    QVector<QPoint> relativePos = m_moduleRelativePos;// 模块的相对位置
    QVector<QHash<int, int>> isPosOccupied(m_moduleRelativePos.size());// 位置是否被占据
    for (int i = 0; i < relativePos.size(); ++i) {
        int x = relativePos[i].x();
        int y = relativePos[i].y();
        if (isPosOccupied[x].contains(y)) {
            y++;
            while (isPosOccupied[x].contains(y)) {
                y++;
            }
            m_moduleRelativePos[i] = QPoint(x, y);
        }
        isPosOccupied[x].insert(y, y);
    }
}
