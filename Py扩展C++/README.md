# Py扩展C++

## 1.函数类

该函数类存放python需要调用的接口函数，该函数类只能放在一个头文件里，也就是说头文件包含函数的声明和实现。

## 2.包装函数类

用pybind11库写一个函数类的包装，如果函数类使用到STL库，则添加stl.h

```c++
//functions_wrapper.cpp   包装类名字 只有cpp文件
#include "pybind11.h"
#include "stl.h"
#include "functions.h" //函数类的头文件

namespace py = pybind11;

PYBIND11_MODULE(functions, m) {
    m.doc() = "Simple Class";
    m.def("greet", greet, "Welcome");
    // c++接口 函数类名Functions
    py::class_<Functions>(m, "Functions")
            .def(py::init())
        //接口add
            .def("add", &Functions::add)
			//接口setApdRegionNum python中使用funuuuu(函数别名，建议和C++函数同名)调用该函数
    	    .def("funuuuu", &Functions::setApdRegionNum)
            .def("setWukRegionNum", &Functions::setWukRegionNum)

            .def("setApdRegionISize", &Functions::setApdRegionISize)
            .def("setWukRegionISize", &Functions::setWukRegionISize)

            .def("setApdRegionISegJ", &Functions::setApdRegionISegJ)
            .def("setWukRegionISegJ", &Functions::setWukRegionISegJ)

            .def("getWukRegionsPixel", &Functions::getWukRegionsPixel)
            .def("countXorRegionPixel", &Functions::countRegionPixel);


}
```

## 3.添加pybind11库

将pybind/include文件内所有放到functions.h同级目录，将Anaconda/libs的python39.lib放到functions.h同级目录

![image-20211130153616356](.\Picture\image-20211130153616356.png)

![image-20211130153827511](.\Picture\image-20211130153827511.png)

## 4.创建setup.py

用于编译C++生成python库

```python
#setup.py
from setuptools import setup, Extension  
  
functions_module = Extension(  
    name ='functions',  # 编译后库的名字
    sources = ['./functions_wrapper.cpp'],#编译的文件路径
    #包含pybind库相关文件
    #包含python.h及其相关文件
    include_dirs = [r'C:/Users/Huangff/Desktop/pybind11-master/pybind11-master/include',  
                    r'D:/Application_Tools/Anaconda/include',]
                    #r'D:/Master degree/CLionProjects/Gerber_compare_dll/include']
)  
  
setup(ext_modules = [functions_module])

```
## 5.编译生成Pyd

在当前文件夹打开命令行

```shell
// 生成python库
python setup.py build_ext --inplace
// 修改库文件名字
rename functions.cp310-win_amd64.pyd functions.pyd
```

## 6.移动库文件

将生成后的funtions.pyd移动到py库中(为了避免文件冲突,可以修改库的名字)

## 7.调用库文件

通过import functions，实现C++函数类的功能

```python
import functions #如果修改了库文件名字 注意修改此处

if __name__ == '__main__':

	# print(functions.greet())
    f = functions.Functions()
    # print(f.add(1.0, 2.0))

    precision = 0.1
    #wukRegion[0-0]
    f.setWukRegionNum(1)  # 设置wukRegion个数
    f.setApdRegionNum(1)  # 设置apdRegion个数

    #wukRegion[0][0-9]
    f.setApdRegionISize(0, 4)  # 设置wukRegion第一个元素seg个数

    # indexI indexJ isLine startX startY isClockWise centerX centerY
    # 8个参数一个不能少 不需要那么多 后续参数可以随意填写 建议写0或者False
    f.setApdRegionISegJ(0, 0, True, 0.1, 0.1, True, 0.3, 0.4)#设置wukRegion第一个元素seg个数

    f.countXorRegionPixel(0.001)
    f.getWukRegionsPixel(0.001)
```

## 8.注意事项

### 文件夹结构：

1. function.h -- C++扩展函数类

2. python39.lib

3. pybind11/include内所有文件

4. functions_wrapper.cpp

5. setup.py

### 参数的传递

pybind11支持直接传递普通类型（int double float bool string list ）

```c++
struct A{
    int m_count=0;
    bool m_isLine=false;
    double m_aver=1.0;
}
//传递python传递vector<A>的方法
//python使用下面代码进行参数的传递
f.setVectorLen(10)
f.setItemI(0,0,True,1.0)
f.setItemI(1,0,False,1.0)
class Functions{
    vector<A> m_aList;
    bool setVectorLen(int len);
    bool setItemI(int index,int count,bool isLine,double aver);
}
```



函数类使用扫描线算法结合Clipping库计算Region像素点个数
