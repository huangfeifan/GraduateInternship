//
// Created by Nora on 2022-3-30.
//

#pragma once

#include <QTime>
#include <QDebug>
#include <QRandomGenerator>
#include <QHash>
#include <QPair>

#include "PlaceAlgo/MyStruct.h"
#include "PlaceAlgo/FakeData.h"


class RandomData {

public:

    RandomData() {
        //已知:
        // 模块个数 左侧port个数 右侧port个数 需要生成的连接关系个数
        // Attention 注意更新后面会用到的数据
        //QVector<QList<int>> moduleInfo
        //QHash<int,int> leftPortOccupy
        //QHash<int,int> rightPortOccupy




    }

    ~RandomData() = default;

    RandomData(int moduleNum, int leftPortNum, int rightPortNum, int connectDataNum);

    QList<ConnectData> m_randomConnectData;
    QVector<QVector<int>> m_randomModulePortInfo;
    int m_leftNum;
    int m_rightNum;
};



