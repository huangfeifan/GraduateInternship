#pragma once

#include <stdio.h>
#include <iostream>

#include "clipper.hpp"



#define DLLEXPORT extern "C" __declspec(dllexport)




// python 实际调用的函数
DLLEXPORT int __stdcall countRegionPixel(vector<vector<PolygonSeg>> wukRegions, vector<vector<PolygonSeg>> apdRegions,
                                         double precision);


// todo modify
bool
transferRegionToPolygon(const vector<PolygonSeg> region, const double base, vector<IntPoint> &polygon);

bool regionUnion(Path region1, Path region2, Paths &paths);

void preHandlePaths(double base, Paths &wuk, Paths &apd, vector<vector<doublePoint>> wukDouble,
                    vector<vector<doublePoint>> apdDouble);

int polygonScan(double precision, Path vertices);

void regionXor(Paths apdRegions, Paths wukRegions, Paths &result);

bool
convertRegionsToPaths(vector<vector<PolygonSeg>> wukRegions, vector<vector<PolygonSeg>> apdRegions, Paths &wuk,
                      Paths &apd, double base);


