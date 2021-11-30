//
// Created by Huangff on 2021/11/29.
//
// 需要注意 因为只包含头文件,所以实现得放在头文件

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

//定义用于边表ET和活动边表AET的通用类Edge
class Edge {
public:
    int ymax;
    float x;
    float dx;
    Edge *next;
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

class Functions {
public:
    Functions();

    vector<vector<PolygonSeg>> m_wukRegions;
    vector<vector<PolygonSeg>> m_apdRegions;
    int m_wukRegionPixel = 0;
    double m_precision = 1.0;

    double add(double in1, double in2);

    bool setPrecision(double precision) {
        m_precision = precision;
    };

    // fail
    void testVector(std::vector<std::vector<PolygonSeg>> wukRegions, std::vector<std::vector<PolygonSeg>> apdRegions,
                    double precision);

    // python 实际调用的函数
    int countRegionPixel();

    // 设置vector包含的region个数
    bool setWukRegionNum(int num) {
        if (num <= 0)
            return false;
        for (int i = 0; i < num; ++i) {
            vector<PolygonSeg> region;
            m_wukRegions.push_back(region);
        }
        return true;
    }

    bool setApdRegionNum(int num) {
        if (num <= 0)
            return false;
        for (int i = 0; i < num; ++i) {
            vector<PolygonSeg> region;
            m_apdRegions.push_back(region);
        }
        // 类的成员变量修改后再下次调用时依然存在
        std::cout << m_wukRegions.size() << std::endl;
        return true;
    }

    bool setWukRegionISize(int index, int segNum) {
        if (index < m_wukRegions.size() && index >= 0) {
            for (int i = 0; i < segNum; ++i) {
                PolygonSeg seg;
                m_wukRegions[index].push_back(seg);
            }
            return true;
        }
        return false;
    }

    bool setApdRegionISize(int index, int segNum) {
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

    bool
    setApdRegionISegJ(int indexI, int indexJ, bool isLine, double startX, double startY, bool isClockWise,
                      double centerX, double centerY) {
        // success
/*        std::cout << "------" << std::endl;
        std::cout << isLine << std::endl;
        std::cout << startX << std::endl;
        std::cout << startY << std::endl;
        std::cout << isClockWise << std::endl;
        std::cout << centerX << std::endl;
        std::cout << centerY << std::endl;
        std::cout << "------" << std::endl;*/

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

/*                std::cout << "------" << std::endl;
                std::cout << m_apdRegions[indexI][indexJ].isLine << std::endl;
                std::cout << m_apdRegions[indexI][indexJ].startX << std::endl;
                std::cout << m_apdRegions[indexI][indexJ].startY << std::endl;
                std::cout << m_apdRegions[indexI][indexJ].isClockWise << std::endl;
                std::cout << m_apdRegions[indexI][indexJ].centerX << std::endl;
                std::cout << m_apdRegions[indexI][indexJ].centerY << std::endl;
                std::cout << "------" << std::endl;*/
                return true;
            }
        }
        return false;
    }

    bool
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

    int getWukRegionsPixel() {
        return m_wukRegionPixel;
    }


    // todo modify
    bool
    transferRegionToPolygon(const std::vector<PolygonSeg> region, const double base, std::vector<IntPoint> &polygon);

    bool regionUnion(Path region1, Path region2, Paths &paths);

    void preHandlePaths(double base, Paths &wuk, Paths &apd, std::vector<std::vector<doublePoint>> wukDouble,
                        std::vector<std::vector<doublePoint>> apdDouble);

    int polygonScan(double precision, Path vertices);

    void regionXor(Paths apdRegions, Paths wukRegions, Paths &result);

    bool
    convertRegionsToPaths(std::vector<std::vector<PolygonSeg>> wukRegions,
                          std::vector<std::vector<PolygonSeg>> apdRegions, Paths &wuk,
                          Paths &apd, double base);
};


Functions::Functions(void) {
    std::cout << "function created !" << std::endl;
}

double Functions::add(double in1, double in2) {
    return in1 + in2;
}

void
Functions::testVector(std::vector<std::vector<PolygonSeg>> wukRegions, std::vector<std::vector<PolygonSeg>> apdRegions,
                      double precision) {
    std::cout << " testVector !" << std::endl;
    std::cout << precision << std::endl;

    for (int i = 0; i < wukRegions.size(); ++i) {
        for (int j = 0; j < wukRegions[i].size(); ++j) {
            std::cout << wukRegions[i][j].startX << std::endl;
        }
    }
}


int Functions::countRegionPixel() {
    /**
     * input 两个regionList                   wukRegion apdRegion
     *                                            |         |
     *  直线拟合弧线 精度换算 之后 得到两个数组    wuk       apd
     *   wuk数组存放wuk匹配不通过的Paths
     *   apd数组存放wuk匹配不通过的Paths
     *
     *   precision精度建议取值 0.1 0.01 0.001 0.0001 0.000001
     */

    //double base = 1;//

    std::vector<std::vector<PolygonSeg>> wukRegions = m_wukRegions;
    std::vector<std::vector<PolygonSeg>> apdRegions = m_apdRegions;

    double precision = m_precision;

    // 在某个精度下 计算region的像素点个数
    int count = 0;

    Paths wuk, apd;

    convertRegionsToPaths(wukRegions, apdRegions, wuk, apd, precision);

    // 异或操作
    Paths xorResult;
    regionXor(apd, wuk, xorResult);

    // 根据异或得到的多边形组计算其包含的像素点个数
    for (int i = 0; i < xorResult.size(); ++i) {
        count += polygonScan(precision, xorResult[i]);
    }

    // 计算转换后的wuk的pixel个数
    for (int i = 0; i < wuk.size(); ++i) {
        m_wukRegionPixel += polygonScan(precision, wuk[i]);
    }

    return count;

}

bool Functions::transferRegionToPolygon(const std::vector<PolygonSeg> region, const double base,
                                        std::vector<IntPoint> &polygon) {
    // 精度转换
    // 用一系列直线拟合圆弧

    if (base <= 0) {
        return false;
    }
    //vector<IntPoint> polygon;

    for (int i = 0; i < region.size(); ++i) {
        if (region[i].isLine) {
            // 直线则直接起点放到列表中 否则进行转换
            // 注意: 只添加起点

            IntPoint temp(region[i].startX / base, region[i].startY / base);
            polygon.push_back(temp);

            //region[i].endX /= base;
            //region[i].endY /= base;

        } else {

        }
    }

    return true;
}

bool Functions::regionUnion(Path region1, Path region2, Paths &paths) {
    // 存放返回结果paths

    // clipper库的使用
    Clipper clipper;

    // 将其添加到clipper中

    clipper.AddPath(region2, PolyType::ptClip, true);
    clipper.AddPath(region1, PolyType::ptClip, true);

    // 执行union操作
    clipper.Execute(ClipType::ctUnion, paths, PolyFillType::pftEvenOdd, PolyFillType::pftEvenOdd);

    return true;
}

/**
 * 根据precision 计算多边形内像素点个数
 * @param precision
 * @param vertices
 * @return 像素点个数
 */
void Functions::preHandlePaths(double base, Paths &wuk, Paths &apd, std::vector<std::vector<doublePoint>> wukDouble,
                               std::vector<std::vector<doublePoint>> apdDouble) {
/**
     * 精度--网格大小   base = 0.1 表示以0.1为一个单位
     *      1.根据精度调整数值
     *      2.移动区域到第一象限
     *      3.将wukDouble/apdDouble-->wuk/apd
     */

    // 根据精度调整数值
    if (base > 0) {
        int precision = 1.0 / base;

        for (int i = 0; i < wukDouble.size(); ++i) {
            for (int j = 0; j < wukDouble[i].size(); ++j) {
                wukDouble[i][j].x *= precision;
                wukDouble[i][j].y *= precision;
            }
        }

        for (int i = 0; i < apdDouble.size(); ++i) {
            for (int j = 0; j < apdDouble[i].size(); ++j) {
                apdDouble[i][j].x *= precision;
                apdDouble[i][j].y *= precision;
            }
        }
    }

    // 移动区域到第一象限
    double minY = 5;
    // 计算最低点的y坐标
    for (int i = 0; i < wukDouble.size(); ++i) {
        for (int j = 0; j < wukDouble[i].size(); ++j) {
            minY = (minY > wukDouble[i][j].y) ? wukDouble[i][j].y : minY;
        }
    }

    for (int i = 0; i < apdDouble.size(); ++i) {
        for (int j = 0; j < apdDouble[i].size(); ++j) {
            minY = (minY > apdDouble[i][j].y) ? apdDouble[i][j].y : minY;
        }
    }

    double minX = 5;
    // 计算最低点的x坐标
    for (int i = 0; i < wukDouble.size(); ++i) {
        for (int j = 0; j < wukDouble[i].size(); ++j) {
            minX = (minY > wukDouble[i][j].x) ? wukDouble[i][j].x : minY;
        }
    }

    for (int i = 0; i < apdDouble.size(); ++i) {
        for (int j = 0; j < apdDouble[i].size(); ++j) {
            minX = (minY > apdDouble[i][j].x) ? apdDouble[i][j].x : minY;
        }
    }

    minX -= 1;
    minY -= 1;

    minY = minY >= 0 ? 0 : minY;
    minX = minX >= 0 ? 0 : minX;

    if (minY < 0 || minX < 0) {
        // 向上移动多边形
        for (int i = 0; i < wukDouble.size(); ++i) {
            for (int j = 0; j < wukDouble[i].size(); ++j) {
                wukDouble[i][j].x -= minX;
                wukDouble[i][j].y -= minY;
            }
        }

        for (int i = 0; i < apdDouble.size(); ++i) {
            for (int j = 0; j < apdDouble[i].size(); ++j) {
                apdDouble[i][j].x -= minX;
                apdDouble[i][j].y -= minY;
            }
        }
    }

    // 3.将wukDouble/apdDouble-->wuk/apd
    for (int i = 0; i < wukDouble.size(); ++i) {
        Path path;
        for (int j = 0; j < wukDouble[i].size(); ++j) {
            path.push_back(IntPoint(wukDouble[i][j].x, wukDouble[i][j].y));
        }
        wuk.push_back(path);
    }

    for (int i = 0; i < apdDouble.size(); ++i) {
        Path path;
        for (int j = 0; j < apdDouble[i].size(); ++j) {
            path.push_back(IntPoint(apdDouble[i][j].x, apdDouble[i][j].y));
        }
        apd.push_back(path);
    }

}

int Functions::polygonScan(double precision, Path vertices) {
    std::vector<Point> m_pointsList;
    int count = 0;

    //边表
    std::vector<Edge *> ET;
    //活动边表
    Edge *AET;

    //计算最高点的y坐标
    int maxY = 0;
    for (int i = 0; i < vertices.size(); i++) {
        if (vertices[i].Y > maxY) {
            maxY = vertices[i].Y;
        }
    }
    maxY += 1;

    //初始化ET和AET
    //Edge *pET[windowHeight];
    std::vector<Edge *> pET;
    for (int i = 0; i < maxY; i++) {
        Edge *temp = new Edge();
        temp->next = nullptr;
        pET.push_back(temp);
        //pET[i] = new Edge();
        //pET[i]->next = nullptr;
    }
    AET = new Edge();
    AET->next = nullptr;

    //建立边表ET
    for (int i = 0; i < vertices.size(); i++) {
        //取出当前点1前后相邻的共4个点，点1与点2的连线作为本次循环处理的边，另外两个点点0和点3用于计算奇点
        int x0 = vertices[(i - 1 + vertices.size()) % vertices.size()].X;
        int x1 = vertices[i].X;
        int x2 = vertices[(i + 1) % vertices.size()].X;
        int x3 = vertices[(i + 2) % vertices.size()].X;
        int y0 = vertices[(i - 1 + vertices.size()) % vertices.size()].Y;
        int y1 = vertices[i].Y;
        int y2 = vertices[(i + 1) % vertices.size()].Y;
        int y3 = vertices[(i + 2) % vertices.size()].Y;
        //水平线直接舍弃
        if (y1 == y2)
            continue;
        //分别计算下端点y坐标、上端点y坐标、下端点x坐标和斜率倒数
        int ymin = y1 > y2 ? y2 : y1;
        int ymax = y1 > y2 ? y1 : y2;
        float x = y1 > y2 ? x2 : x1;
        float dx = (x1 - x2) * 1.0f / (y1 - y2);
        //奇点特殊处理，若点2->1->0的y坐标单调递减则y1为奇点，若点1->2->3的y坐标单调递减则y2为奇点
        if (((y1 < y2) && (y1 > y0)) || ((y2 < y1) && (y2 > y3))) {
            ymin++;
            x += dx;
        }
        //创建新边，插入边表ET
        Edge *p = new Edge();
        p->ymax = ymax;
        p->x = x;
        p->dx = dx;
        p->next = pET[ymin]->next;
        pET[ymin]->next = p;
    }

    //扫描线从下往上扫描，y坐标每次加1
    for (int i = 0; i < maxY; i++) {
        //取出ET中当前扫描行的所有边并按x的递增顺序（若x相等则按dx的递增顺序）插入AET
        while (pET[i]->next) {
            //取出ET中当前扫描行表头位置的边
            Edge *pInsert = pET[i]->next;
            Edge *p = AET;
            //在AET中搜索合适的插入位置
            while (p->next) {
                if (pInsert->x > p->next->x) {
                    p = p->next;
                    continue;
                }
                if (pInsert->x == p->next->x && pInsert->dx > p->next->dx) {
                    p = p->next;
                    continue;
                }
                //找到位置
                break;
            }
            //将pInsert从ET中删除，并插入AET的当前位置
            pET[i]->next = pInsert->next;
            pInsert->next = p->next;
            p->next = pInsert;
        }

        //AET中的边两两配对并填色
        Edge *p = AET;
        while (p->next && p->next->next) {
/*            for (int x = p->next->x; x < p->next->next->x; x++) {
                //glVertex2i(x, i);
                // 区域包含的像素点坐标
                Point point(x, i);
                m_pointsList.push_back(point);

                //   count++;
            }*/
            count += p->next->next->x - p->next->x + 1;
            p = p->next->next;
        }

        //删除AET中满足y=ymax的边
        p = AET;
        while (p->next) {
            if (p->next->ymax == i) {
                Edge *pDelete = p->next;
                p->next = pDelete->next;
                pDelete->next = nullptr;
                delete pDelete;
            } else {
                p = p->next;
            }
        }

        //更新AET中边的x值，进入下一循环
        p = AET;
        while (p->next) {
            p->next->x += p->next->dx;
            p = p->next;
        }

    }

    // 防止内存泄漏
    for (int i = 0; i < pET.size(); ++i) {
        auto temp = pET[i];
        delete temp;
    }
    delete AET;

    //qDebug() << count << " <--count   size--> " << m_pointsList.size();
    return count > m_pointsList.size() ? count : m_pointsList.size();
}

void Functions::regionXor(Paths apdRegions, Paths wukRegions, Paths &result) {
    Paths paths;// 存放返回结果

    // clipper库的使用
    Clipper clipper;

    // 分别将其添加到clip subject中
    for (int i = 0; i < apdRegions.size(); ++i) {
        clipper.AddPath(apdRegions[i], PolyType::ptClip, true);
    }

    for (int i = 0; i < wukRegions.size(); ++i) {
        clipper.AddPath(wukRegions[i], PolyType::ptSubject, true);
    }

    // 执行Xor操作
    clipper.Execute(ClipType::ctXor, result, PolyFillType::pftEvenOdd, PolyFillType::pftEvenOdd);
}

bool Functions::convertRegionsToPaths(std::vector<std::vector<PolygonSeg>> wukRegions,
                                      std::vector<std::vector<PolygonSeg>> apdRegions,
                                      Paths &wuk, Paths &apd, double base) {
    return false;
}
