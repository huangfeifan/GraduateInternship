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
        modulePortNum[i] = QRandomGenerator::global()->bounded(10) + 1;
    }

    /// 随机生成ConnectData
    QList<ConnectData> myConnectData;

    // 记录模块的端口是否被占用，QPair的t1表示模块序号，t2表示端口序号
    QHash<QPair<int, int>, QPair<int, int>> hash;
    int tempNum = 0;
    while (tempNum < connectDataNum) {
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
                tempNum++;
                hash.insert(pair1, pair1);
                hash.insert(pair2, pair2);
            }
        }
    }
    qDebug() << myConnectData.size() << "  --ConnectData Num";

    QVector<QVector<int>> modulePortInfo(moduleNum);
    for (int i = 0; i < modulePortInfo.size(); ++i) {
        modulePortInfo[i] = QVector<int>(modulePortNum[i]);
        for (int j = 0; j < modulePortInfo[i].size(); ++j) {
            modulePortInfo[i][j] = 0;
        }
    }
    for (int i = 0; i < myConnectData.size(); ++i) {
        int portIndex = myConnectData[i].startPortIndex;
        int moduleIndex = myConnectData[i].startModuleIndex;
        if(moduleIndex==-1){
            continue;
        }
        modulePortInfo[moduleIndex][portIndex] = 2;
    }

    QString str;
    for (int i = 0; i < modulePortInfo.size(); ++i) {
        str += "{";
        for (int j = 0; j < modulePortInfo[i].size(); ++j) {
            str += QString::number(modulePortInfo[i][j]);
            str += ", ";
        }
        str += "},";
    }
    qDebug() << str << "ModulePortInfo\n\n";


    m_randomConnectData = myConnectData;
    m_randomModulePortInfo = modulePortInfo;
}

