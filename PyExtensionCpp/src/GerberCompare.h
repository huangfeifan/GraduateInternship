#pragma once

#include <iostream>
#include "clipping.h"

using namespace ClipperLib;
using namespace std;

//定义用于表示像素点坐标的类Point
class Point {
public:
    int x;
    int y;

    Point(int x, int y) {
        this->x = x;
        this->y = y;
    }
};

class doublePoint {
public:
    double x;
    double y;

    doublePoint(double x, double y) {
        this->x = x;
        this->y = y;
    }
};

struct PolygonSeg {
    bool isLine = true;
    double startX = 0;
    double startY = 0;
    bool isClockWise = false;
    double centerX = 0;
    double centerY = 0;
};

char const *greet() {
    return "Welcome to pybind11 !";
}

class RegionCompare {
public:
    RegionCompare();

    vector <vector<PolygonSeg>> m_wukRegions;
    vector <vector<PolygonSeg>> m_apdRegions;

    // python 实际调用的函数
    //int countRegionPixel(double precision);
    //int getWukRegionsPixel(double precision);

    bool setApdRegionNum(int num);

    bool setApdRegionISize(int index, int segNum);

    bool
    setApdRegionISegJ(int indexI, int indexJ, bool isLine, double startX, double startY, bool isClockWise,
                      double centerX, double centerY);

    bool setWukRegionNum(int num);    // 设置vector包含的region个数
    bool setWukRegionISize(int index, int segNum);

    bool
    setWukRegionISegJ(int indexI, int indexJ, bool isLine, double startX, double startY, bool isClockWise,
                      double centerX, double centerY);

    //
    void regionXor(Paths apdRegions, Paths wukRegions, Paths &result);

    bool regionUnion(Paths paths,Paths &result);


    // todo modify
    //bool transferRegionToPolygon(const std::vector <PolygonSeg> region, const double base, std::vector <IntPoint> &polygon);

    //void preHandlePaths(double base, Paths &wuk, Paths &apd, std::vector <std::vector<doublePoint>> wukDouble,
    //                    std::vector <std::vector<doublePoint>> apdDouble);

    //int polygonScan(double precision, Path vertices);


    bool
    convertRegionsToPaths(std::vector <std::vector<PolygonSeg>> wukRegions,
                          std::vector <std::vector<PolygonSeg>> apdRegions, Paths &wuk,
                          Paths &apd, double base);
};


RegionCompare::RegionCompare(void) {
    //std::cout << "function created !" << std::endl;
}

bool RegionCompare::setWukRegionNum(int num) {
    if (num <= 0)
        return false;
    for (int i = 0; i < num; ++i) {
        vector <PolygonSeg> region;
        m_wukRegions.push_back(region);
    }
    return true;
}

bool RegionCompare::setApdRegionNum(int num) {
    if (num <= 0)
        return false;
    for (int i = 0; i < num; ++i) {
        vector <PolygonSeg> region;
        m_apdRegions.push_back(region);
    }
    // 类的成员变量修改后再下次调用时依然存在
    //std::cout << m_wukRegions.size() << std::endl;
    return true;
}

bool RegionCompare::setWukRegionISize(int index, int segNum) {
    if (index < m_wukRegions.size() && index >= 0) {
        for (int i = 0; i < segNum; ++i) {
            PolygonSeg seg;
            m_wukRegions[index].push_back(seg);
        }
        return true;
    }
    return false;
}

bool RegionCompare::setApdRegionISize(int index, int segNum) {
    //std::cout << m_apdRegions.size() << std::endl;

    if (index < m_apdRegions.size() && index >= 0) {
        for (int i = 0; i < segNum; ++i) {
            PolygonSeg seg;
            m_apdRegions[index].push_back(seg);
        }
        return true;
    }
    return false;
}

bool RegionCompare::
setApdRegionISegJ(int indexI, int indexJ, bool isLine, double startX, double startY, bool isClockWise,
                  double centerX, double centerY) {
    // success
    if (indexI >= 0 && indexI < m_apdRegions.size()) {
        if (indexJ >= 0 && indexJ < m_apdRegions[indexI].size()) {
            //index有效
            //PolygonSeg seg;
            m_apdRegions[indexI][indexJ].isLine = isLine;
            m_apdRegions[indexI][indexJ].startX = startX;
            m_apdRegions[indexI][indexJ].startY = startY;
            m_apdRegions[indexI][indexJ].isClockWise = isClockWise;
            m_apdRegions[indexI][indexJ].centerX = centerX;
            m_apdRegions[indexI][indexJ].centerY = centerY;

            return true;
        }
    }
    return false;
}

bool RegionCompare::
setWukRegionISegJ(int indexI, int indexJ, bool isLine, double startX, double startY, bool isClockWise,
                  double centerX, double centerY) {
    if (indexI >= 0 && indexI < m_wukRegions.size()) {
        if (indexJ >= 0 && indexJ < m_wukRegions[indexI].size()) {
            //index有效
            m_wukRegions[indexI][indexJ].isLine = isLine;
            m_wukRegions[indexI][indexJ].startX = startX;
            m_wukRegions[indexI][indexJ].startY = startY;
            m_wukRegions[indexI][indexJ].isClockWise = isClockWise;
            m_wukRegions[indexI][indexJ].centerX = centerX;
            m_wukRegions[indexI][indexJ].centerY = centerY;

            return true;
        }
    }
    return false;
}



bool RegionCompare::regionUnion(Paths paths,Paths &result) {
    // 存放返回结果paths

    // clipper库的使用
    Clipper clipper;

    // 将其添加到clipper中
    for (int i = 0; i < paths.size(); ++i) {
        clipper.AddPath(paths[i], PolyType::ptSubject, true);
    }

    // 执行union操作
    clipper.Execute(ClipType::ctUnion, result, PolyFillType::pftEvenOdd, PolyFillType::pftEvenOdd);

    return true;
}

void RegionCompare::regionXor(Paths apdPaths, Paths wukPaths, Paths &result) {
    Paths paths;// 存放返回结果

    // clipper库的使用
    Clipper clipper;

    // 分别将其添加到clip subject中
    for (int i = 0; i < apdPaths.size(); ++i) {
        clipper.AddPath(apdPaths[i], PolyType::ptClip, true);
    }

    for (int i = 0; i < wukPaths.size(); ++i) {
        clipper.AddPath(wukPaths[i], PolyType::ptSubject, true);
    }

    // 执行Xor操作
    clipper.Execute(ClipType::ctXor, result, PolyFillType::pftEvenOdd, PolyFillType::pftEvenOdd);
}

bool RegionCompare::convertRegionsToPaths(std::vector <std::vector<PolygonSeg>> wukRegions,
                                      std::vector <std::vector<PolygonSeg>> apdRegions,
                                      Paths &wuk, Paths &apd, double base) {

    /**
     * 预处理region的数据(1.根据精度缩放多边形 2.移动多边形--扫描线算法处理都是正数) 将其变为path 存放到对应的数组中
     *
     */
    //wukRegions--> wukDouble--> wuk
    //apdRegions--> apdDouble--> apd

    // 存放region转换后的path--double版本
    vector <vector<doublePoint>> wukDouble, apdDouble;
    for (int i = 0; i < wukRegions.size(); ++i) {
        vector <doublePoint> temp;
        for (int j = 0; j < wukRegions[i].size(); ++j) {
            temp.push_back(doublePoint(wukRegions[i][j].startX, wukRegions[i][j].startY));
        }
        wukDouble.push_back(temp);
    }
    for (int i = 0; i < apdRegions.size(); ++i) {
        vector <doublePoint> temp;
        for (int j = 0; j < apdRegions[i].size(); ++j) {
            temp.push_back(doublePoint(apdRegions[i][j].startX, apdRegions[i][j].startY));
        }
        apdDouble.push_back(temp);
    }

    //preHandlePaths(base, wuk, apd, wukDouble, apdDouble);

    if (wuk.size() == 0 || apd.size() == 0) {
        return false;
    }
    return true;

}

void testVector(std::vector <std::vector<PolygonSeg>> wukRegions,
                std::vector <std::vector<PolygonSeg>> apdRegions,
                double precision) {
    std::cout << " testVector !" << std::endl;
    std::cout << precision << std::endl;

    for (int i = 0; i < wukRegions.size(); ++i) {
        for (int j = 0; j < wukRegions[i].size(); ++j) {
            std::cout << wukRegions[i][j].startX << std::endl;
        }
    }
}


///---------------------------------------------------------------------------------------------------------------------



