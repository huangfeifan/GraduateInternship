//
// Created by Huangff on 2022/3/6.
//

#pragma once

#include <QVector>

class ComputeRelativePos {
public:
    ComputeRelativePos() {

    };

    QVector<QPoint> getRelativePos() {
        return m_relativePos;
    }

private:
    QVector<QPoint> m_relativePos;

};



