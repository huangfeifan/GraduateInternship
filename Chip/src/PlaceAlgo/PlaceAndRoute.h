//
// Created by Huangff on 2022/3/23.
//

#pragma once

#include <QPoint>

class PlaceAndRoute {

public:

    PlaceAndRoute();

    ~PlaceAndRoute() = default;

    // convert data to fit interface
    void dataConvert();

    // compute placing result
    void placing();

    // compute routing result
    void routing();

    // get module pos
    QVector<QPoint> getModulePos();

    // get module port pos
    void getPortPos();

    // get alone left input port pos
    QVector<QPoint> getLeftPortPos();

    // get alone right output port pos
    QVector<QPoint> getRightPortPos();

    // get all paths
    void getPaths();


private:


};
