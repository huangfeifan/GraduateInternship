//
// Created by Nora on 2022-3-25.
//

#pragma once

#include "PlaceAlgo/MyStruct.h"

const int GRID = 10;

const int LEFT_PORT_NUM = 6;// 左侧输入port个数

const int RIGHT_PORT_NUM = 3;// 右侧输出port个数

// 连接关系
const static QList<ConnectData> Fake_ConnectData = {
        // startModuleIndex, endModule, startPort ,endPort

        /*
        // example 1
        {-1, 0, 0, 0},
        {-1, 3, 1, 0},
        {-1, 0, 2, 1},
        {-1, 2, 3, 1},
        {0,  3, 2, 1},
        {0,  2, 3, 0},

        */

        // example 2
        {-1, 0,  3,  2},
        {-1, 0,  4,  3},
        {-1, 0,  5,  4},

        {0,  -1, 8,  0},
        {0,  -1, 9,  1},
        {0,  -1, 10, 2},
        {0,  2,  11, 2},
        {0,  2,  12, 4},
        {0,  1,  13, 3},

        {2,  0,  8,  7},
        {2,  3,  9,  0},
        {2,  3,  10, 1},
        {2,  3,  11, 2},

        {3,  2,  3,  6},
        {3,  2,  4,  7},


};

// 模块信息  0表示leftInputPort 2表示rightOutputPort
const static QVector<QVector<int>> Fake_ModulePortInfo = {

        /*
        // example 1

        {0, 0, 2, 2},// module_0
        {},// module_1
        {2, 2},//module_2
        {0, 0},//module_3
        */


        // example 2
        {0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2},// module_0
        {0, 0, 0, 0, 2, 2},// module_1
        {0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2},//module_2
        {0, 0, 0, 2, 2},//module_3
};






