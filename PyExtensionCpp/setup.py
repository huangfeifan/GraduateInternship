from setuptools import setup, Extension  
  
functions_module = Extension(  
    name ='functions',  # 编译后库的名字
    sources = ['./src/functions_wrapper.cpp'],#编译的文件路径
    #包含pybind库相关文件
    #包含python.h及其相关文件
    include_dirs = [r'./includeAnaconda',  
                    r'./includePybind11',]
                    #r'D:/Master degree/CLionProjects/Gerber_compare_dll/include']
)  
  
setup(ext_modules = [functions_module])