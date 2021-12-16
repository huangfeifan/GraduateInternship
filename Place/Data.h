//
// Created by Huangff on 2021/11/6.
//

#ifndef PLACE_DATA_H
#define PLACE_DATA_H

#include <QString>
#include <QVector>

#define COMPONENTNUM  7 //component num
#define MAXCOLOUM 4
#define MAXROW 4

struct NameMapIndex {
    QString name;//component name
    int index;//component index
};

struct line {
    int sender = 0;
    int receiver = 0;
    int lineNum = 0;
};

// 单根线连接信息 从sender到receiver 是不是反馈线
struct ConnectionData {
    // 从 sender 出发到 receiver 有 output 条路径
    // isFeedbackLine 标记反馈线
    int sender = -1;
    int senderPort = -1;
    bool isFeedbackLine = false;
    int receiver = -1;
    int receiverPort = -1;
};

// component的基本信息 name index topologyIndex inDegree outDegree
struct ComponentInfo {
    // component 名字
    QString componentName = "";

    // componentIndex
    int componentIndex = 0;

    //topology index 拓扑序列
    int topologyIndex = 0;

    // 入度
    int inDegree = 0;
    // 出度
    int outDegree = 0;
};

static QVector<NameMapIndex> NameVectorMapIndex = {
        // 需要注意此处不能出现componentIndex为0的情况
        {"A", 1},
        {"B", 2},
        {"C", 3},
        {"D", 4},
        {"E", 5},

        {"F", 6},
        {"G", 7},

/*        {"H", 8},
        {"I", 9},
        {"J", 10},

        {"K", 11},
        {"L", 12},*/

};

static QList<ConnectionData> FakeData = {

        {1, 1, false, 3, 1},
        {1, 2, false, 4, 2},
        {1, 3, false, 5, 3},

        {3, 1, false, 2, 1},

        {4, 1, false, 2, 1},


        {5, 1, false, 2, 1},



        // --pass topologySort
        // test_case 4 --with loop 1-->2-->3-->4-->5-->1
/*        {1, 1, false, 2, 1},
        {2, 1, false, 3, 1},
        {3, 1, false, 4, 1},
        {4, 1, false, 5, 1},
        {5, 1, false, 1, 1},*/


        // test_case 3  --pass topologySort
/*        {1, 1, false, 3, 1},
        {2, 1, false, 3, 1},
        {3, 1, false, 4, 1},
        {3, 1, false, 5, 1},
        {4, 1, false, 5, 1},
        {5, 1, false, 1, 1},
        // temp add -- two rows
        {5, 1, false, 2, 1},
        {1, 1, false, 2, 1},*/


        //testCase_2  --pass computingPriority
/*        {1, 1, false, 2, 1},
        {1, 2, false, 2, 2},
        {1, 3, false, 4, 3},
        {1, 3, false, 5, 3},
        {1, 3, false, 5, 3},
        {1, 3, false, 5, 3},

        {2, 4, false, 3, 1},
        {2, 5, false, 3, 2},
        {2, 6, false, 3, 3},
        {2, 6, false, 3, 3},
        {2, 4, false, 4, 1},
        {2, 4, false, 4, 1},

        // 加上该边则有环
        {3, 1, false, 1, 1},

        {4, 1, false, 3, 1},
        {4, 1, false, 3, 1},
        {4, 1, false, 3, 1},

        {5, 1, false, 3, 1},
        {5, 1, false, 3, 1},
        {5, 1, false, 3, 1},
        {5, 1, false, 3, 1},
        {5, 1, false, 3, 1},*/


        //testCase_1 pass_topologySort
/*
        {1, 1, false, 2, 1},
        {1, 2, false, 2, 2},
        {1, 3, false, 2, 3},

        {2, 4, false, 3, 1},
        {2, 5, false, 3, 2},
        {2, 6, false, 3, 3},
        {2, 4, false, 1, 1},

        {3, 1, false, 4, 1},

        {4, 1, false, 2, 1},
        {4, 1, false, 5, 1},

        {5, 1, false, 4, 1},
*/

};

#endif //PLACE_DATA_H
