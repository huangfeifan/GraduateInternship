from setuptools import setup, Extension  
  
functions_module = Extension(  
    name ='functions',  
    sources = ['functions_wrapper.cpp'],  
    include_dirs = [r'C:\Users\Huangff\Desktop\pybind11-master\pybind11-master\include',  
                    r'D:\Application_Tools\Anaconda\include']  
)  
  
setup(ext_modules = [functions_module])