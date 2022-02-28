from setuptools import setup, Extension

HuaseenGerberCompare_module = Extension(
    name ='HuaseenGerberCompare',  # 编译后库的名字  需要和接口函数的类名相同
    sources = ['./src/main.cpp'],#编译的文件路径
    #包含pybind库相关文件
    #包含python.h及其相关文件
    include_dirs = [r'./includeAnaconda',  
                    r'./includePybind11',]
                    #r'D:/Master degree/CLionProjects/Gerber_compare_dll/include']
)  
  
setup(ext_modules = [HuaseenGerberCompare_module])