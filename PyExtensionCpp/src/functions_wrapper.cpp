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
    py::class_<Functions>(m, "Functions")
            .def(py::init())
            .def("setApdRegionNum", &Functions::setApdRegionNum)
            .def("setWukRegionNum", &Functions::setWukRegionNum)

            .def("setApdRegionISize", &Functions::setApdRegionISize)
            .def("setWukRegionISize", &Functions::setWukRegionISize)

            .def("setApdRegionISegJ", &Functions::setApdRegionISegJ)
            .def("setWukRegionISegJ", &Functions::setWukRegionISegJ)

            .def("getWukRegionsPixel", &Functions::getWukRegionsPixel)
            .def("countXorRegionPixel", &Functions::countRegionPixel);


}