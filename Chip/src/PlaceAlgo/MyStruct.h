//
// Created by Huangff on 2022/3/8.
//
#pragma once

#include <QString>
#include <QPoint>
#include <QList>
#include <QHash>
#include <QVector>


/// 用于对权重的排序
struct HeightAndIndex {
    int height;
    int index;
};

/// 注意事项 可能不能很好地与接口兼容 需要增加一个转换函数 实现接口数据到算法数据的转换
struct ConnectData {
    // 算法的需要的数据 用于存储所有数据
    int startModuleIndex = 0;// 连线起点索引
    int endModuleIndex = 0;// 连线终点索引
    int startPortIndex = 0;// 起点端口索引
    int endPortIndex = 0;// 终点端口索引
    // moduleIndex = -1 表示单独的port
};

struct PortInfo {
    /// 注意事项
    ///     计算module上的port时  height是行相对位置
    ///
    ///     计算单独的port时  height是行绝对位置
    ///         由module上的port输出到单独的port 则height=0
    ///         由单独的port输出到module上的port 则height=0
    int moduleIndex = 0;
    int portIndex = 0;
    int height = 0;// 如果port是起点,则终点高度为height;如果port是终点,则起点高度为height
    bool isLeftInput = false;// true表示是左侧单独的输入port;false则表示右侧单独的输出port
};

struct ASccInfo {
    /// 单个强连通分支内部结点的所有相关信息
    QPoint sccSize;// 一个强连通分支包含结点所占据的大小
    QVector<QPoint> absolutePos;// 强连通分支内部结点的绝对位置
    QVector<QPoint> relativePos;// 强连通分支内部结点的相对位置
    QHash<int, int> sccIndexHash;// 根据原index找到现在的index   <key--int--before,value--int--after--now>
    QVector<QList<int>> graph;// 连接数据--邻接表
    QVector<QPoint> moduleSize;// 模块的大小
};

struct ModuleSize {
    int width = 0;
    int height = 0;
};

struct IndexDegree {
    int index;
    int degree;
};

struct Bar {
    int barLevel;
    QString diameter;
    double planAmount;
};

