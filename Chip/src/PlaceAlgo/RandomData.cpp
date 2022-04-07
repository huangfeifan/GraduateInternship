//
// Created by Nora on 2022-3-30.
//

#include "PlaceAlgo/RandomData.h"

RandomData::RandomData(int moduleNum, int leftPortNum, int rightPortNum, int connectDataNum) : m_leftNum(leftPortNum),
                                                                                               m_rightNum(
                                                                                                       rightPortNum) {

    /// 随机生成模块的port数目
    //int maxPortNum = 10;
    QVector<int> modulePortNum(moduleNum);
    for (int i = 0; i < moduleNum; ++i) {
        modulePortNum[i] = QRandomGenerator::global()->bounded(moduleNum) + 1;
    }
    qDebug() << modulePortNum << "--modulePortNum";

    /// 随机生成modulePortInfo
    QVector<QVector<int>> modulePortInfo(moduleNum);
    for (int i = 0; i < modulePortInfo.size(); ++i) {
        // 初始化，全为右边的port
        modulePortInfo[i] = QVector<int>(modulePortNum[i], 2);
        // 随机生成模块左边的port的数目
        int moduleLeftPortNum = QRandomGenerator::global()->bounded(modulePortInfo[i].size() + 1);
        for (int j = 0; j < moduleLeftPortNum; ++j) {
            modulePortInfo[i][j] = 0;
        }
        //qDebug() << moduleLeftPortNum << "  --module" << i << "leftPortNum";
        //qDebug() << modulePortInfo[i] << "  --module" << i << "PortInfo";
    }
    qDebug() << modulePortInfo << "--modulePortInfo";

    /// 随机生成ConnectData
    QList<ConnectData> myConnectData;

    // 记录模块的端口是否被占用，QPair的t1表示模块序号，t2表示端口序号
    QHash<QPair<int, int>, QPair<int, int>> hash;
    for (int i = 0; i < connectDataNum; ++i) {
        // 生成起始模块序号
        int startModuleIndex = QRandomGenerator::global()->bounded(moduleNum + 1);
        if (startModuleIndex == moduleNum) {
            startModuleIndex = -1;
        }
        // 生成终止模块序号
        int endModuleIndex = QRandomGenerator::global()->bounded(moduleNum + 1);
        if (endModuleIndex == moduleNum) {
            endModuleIndex = -1;
        }

        // 生成端口序号
        if (startModuleIndex != endModuleIndex) {
            int startPortIndex;
            int endPortIndex;

            if (startModuleIndex == -1) {
                startPortIndex = QRandomGenerator::global()->bounded(leftPortNum);
            }
            if (startModuleIndex != -1) {
                startPortIndex = QRandomGenerator::global()->bounded(modulePortNum[startModuleIndex]);
            }
            if (endModuleIndex == -1) {
                endPortIndex = QRandomGenerator::global()->bounded(rightPortNum);
            }
            if (endModuleIndex != -1) {
                endPortIndex = QRandomGenerator::global()->bounded(modulePortNum[endModuleIndex]);
            }


            QPair<int, int> pair1(startModuleIndex, startPortIndex);
            QPair<int, int> pair2(endModuleIndex, endPortIndex);

            if (!hash.contains(pair1) && !hash.contains(pair2)) {
                ConnectData temp = {startModuleIndex, endModuleIndex, startPortIndex, endPortIndex};
                myConnectData.push_back(temp);
                qDebug() << "{" << startModuleIndex << "," << endModuleIndex << "," << startPortIndex << ","
                         << endPortIndex << "},";
                hash.insert(pair1, pair1);
                hash.insert(pair2, pair2);
            }
        }
    }

    m_randomConnectData = myConnectData;
    m_randomModulePortInfo = modulePortInfo;
}