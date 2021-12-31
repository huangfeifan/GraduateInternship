//
// Created by Huangff on 2021/11/29.
//

#include "pybind11.h"
#include "stl.h"
#include "functions.h"

namespace py = pybind11;

PYBIND11_MODULE(functions, m) {
    m.doc() = "Simple Class";
    m.def("greet", greet, "Welcome");
    // c++接口
    py::class_<Functions>(m, "Functions")  // 引号括起来的 "Functions" 是 python接口使用的类名
            .def(py::init())  //不要落下
            .def("setApdRegionNum", &Functions::setApdRegionNum) // 引号括起来的 "setApdRegionNum" 是 python接口函数名
            .def("setWukRegionNum", &Functions::setWukRegionNum)

            .def("setApdRegionISize", &Functions::setApdRegionISize)
            .def("setWukRegionISize", &Functions::setWukRegionISize)

            .def("setApdRegionISegJ", &Functions::setApdRegionISegJ)
            .def("setWukRegionISegJ", &Functions::setWukRegionISegJ)

            .def("getWukRegionsPixel", &Functions::getWukRegionsPixel)
            .def("countXorRegionPixel", &Functions::countRegionPixel);  // 接口函数结束需要分号


}