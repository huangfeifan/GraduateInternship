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

class RandomData {

public:

    RandomData() = default;

    ~RandomData() = default;

    RandomData(int moduleNum, int leftPortNum, int rightPortNum, int connectDataNum);

    QList<ConnectData> m_randomConnectData;
    QVector<QVector<int>> m_randomModulePortInfo;
    int m_leftNum;
    int m_rightNum;
};



