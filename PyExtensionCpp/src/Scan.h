//
// Created by Huangff on 2022/2/25.
//

#pragma once

//定义用于边表ET和活动边表AET的通用类Edge
class Edge {
public:
    int ymax;
    float x;
    float dx;
    Edge *next;
};

int getWukRegionsPixel(double precision) {
    std::vector <std::vector<PolygonSeg>> wukRegions = m_wukRegions;
    std::vector <std::vector<PolygonSeg>> apdRegions = m_apdRegions;

    //double precision = m_precision;

    // 在某个精度下 计算region的像素点个数
    int count = 0;

    Paths wuk, apd;

    convertRegionsToPaths(wukRegions, apdRegions, wuk, apd, precision);

    // 计算转换后的wuk的pixel个数
    std::cout << apd.size() <<" wuk_size "<< endl;
    for (int i = 0; i < apd.size(); ++i) {

        //count += polygonScan(precision, apd[i]);


    }
    std::cout << count <<" wukRegion_size_count "<< endl;

    return count;
}

int countRegionPixel(double precision) {
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

    std::vector <std::vector<PolygonSeg>> wukRegions = m_wukRegions;
    std::vector <std::vector<PolygonSeg>> apdRegions = m_apdRegions;

/*    std::cout << wukRegions.size() << endl;
    std::cout << apdRegions.size() << endl;
    std::cout << "----------------" << endl;
    for (int i = 0; i < apdRegions.size(); ++i) {
        for (int j = 0; j < apdRegions[i].size(); ++j) {
            cout<< apdRegions[i][j].startX<<endl;
            cout<< apdRegions[i][j].startY<<endl;
            cout<< apdRegions[i][j].isLine<<endl;
            cout<< apdRegions[i][j].isClockWise<<endl;
        }
    }
    std::cout << "----------------" << endl;*/

    //double precision = m_precision;

    // 在某个精度下 计算region的像素点个数
    int count = 0;

    Paths wuk, apd;

    convertRegionsToPaths(wukRegions, apdRegions, wuk, apd, precision);

    // 异或操作
    Paths xorResult;
    regionXor(apd, wuk, xorResult);

    // 根据异或得到的多边形组计算其包含的像素点个数
    std::cout << xorResult.size() <<" xorResult_size "<< endl;
    for (int i = 0; i < xorResult.size(); ++i) {
        count += polygonScan(precision, xorResult[i]);
    }
    std::cout << count <<" xorResult_size_count "<< endl;
    return count;

}

bool transferRegionToPolygon(const std::vector <PolygonSeg> region, const double base,
                             std::vector <IntPoint> &polygon) {
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

void preHandlePaths(double base, Paths &wuk, Paths &apd, std::vector <std::vector<doublePoint>> wukDouble,
                    std::vector <std::vector<doublePoint>> apdDouble) {

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

int polygonScan(double precision, Path vertices) {
    std::vector <Point> m_pointsList;
    int count = 0;

    //边表
    std::vector < Edge * > ET;
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
    std::vector < Edge * > pET;
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
            for (int x = p->next->x; x < p->next->next->x; x++) {
                //glVertex2i(x, i);
                // 区域包含的像素点坐标
                Point point(x, i);
                m_pointsList.push_back(point);

                //   count++;
            }
            count += p->next->next->x - p->next->x ;
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
