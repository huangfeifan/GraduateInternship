//
// Created by Huangff on 2021/11/29.
//

#include "pybind11.h"
#include "stl.h"
#include "GerberCompare.h"

namespace py = pybind11;

PYBIND11_MODULE(HuaseenGerberCompare, m) {
    m.doc() = "Simple Class";
    m.def("greet", greet, "Welcome");
    // c++接口
    py::class_<RegionCompare>(m, "Functions")  // 引号括起来的 "Functions" 是 python接口使用的类名
            .def(py::init())  //不要落下
            .def("setApdRegionNum", &RegionCompare::setApdRegionNum) // 引号括起来的 "setApdRegionNum" 是 python接口函数名
            .def("setWukRegionNum", &RegionCompare::setWukRegionNum)

            .def("setApdRegionISize", &RegionCompare::setApdRegionISize)
            .def("setWukRegionISize", &RegionCompare::setWukRegionISize)

            .def("setApdRegionISegJ", &RegionCompare::setApdRegionISegJ)
            .def("setWukRegionISegJ", &RegionCompare::setWukRegionISegJ)

            //.def("getWukRegionsPixel", &RegionCompare::getWukRegionsPixel)
            //.def("countXorRegionPixel", &RegionCompare::countRegionPixel)

            ;  // 接口函数结束需要分号


}