

import ctypes
import functions

def print_hi(name):
    # Use a breakpoint in the code line below to debug your script.
    print(f'Hi, {name}')  # Press Ctrl+F8 to toggle the breakpoint.


# Press the green button in the gutter to run the script.
if __name__ == '__main__':

    #Cfun = ctypes.WinDLL('./Gerber_compare_dll.dll')  ##动态链接库所在目录
    #Cfun.hello()
    # ctypes 不支持传递union和structure 而是通过传递union structure的指针
    a = 10
    b = 22
    #Cfun.testAdd(a, b)

    # print(functions.greet())
    f = functions.Functions()
    # print(f.add(1.0, 2.0))

    precision = 0.1
    #wukRegion[0-0]
    f.setWukRegionNum(1)  # 设置wukRegion个数
    f.setApdRegionNum(1)  # 设置apdRegion个数

    #wukRegion[0][0-9]
    f.setApdRegionISize(0, 4)  # 设置wukRegion第一个元素seg个数

    # regionIndex rowIndex isLine startX startY isClockWise centerX centerY
    # 8个参数一个不能少 不需要那么多 后续参数可以随意填写 建议写0或者False
    f.setApdRegionISegJ(0, 0, True, 0.1, 0.1, True, 0.3, 0.4)  # 设置wukRegion第一个元素seg个数
    f.setApdRegionISegJ(0, 1, True, 0.4, 0.1, True, 0.3, 0.4)  # 设置wukRegion第一个元素seg个数
    f.setApdRegionISegJ(0, 2, True, 0.4, 0.4, True, 0.3, 0.4)  # 设置wukRegion第一个元素seg个数
    f.setApdRegionISegJ(0, 3, True, 0.1, 0.4, True, 0.3, 0.4)  # 设置wukRegion第一个元素seg个数
    f.countXorRegionPixel(0.001)
    f.getWukRegionsPixel(0.001)
    # f.testVector(apdRegions, apdRegions, precision)

    ### https://blog.csdn.net/BigBoySunshine/article/details/89566012


import ctypes

Cfun = ctypes.WinDLL('./Ccode.dll/x64/Release/Ccode_dll.dll')  ##动态链接库所在目录
Num1 = ctypes.c_int(10)         ##创建第一个输入参数，将参数类型指定为c_int即C语言中的int类型
Num2 = ctypes.c_int(30)
Cfun.add.restype=ctypes.c_int   ##将C函数的返回值类型定位c_int即C语言中的int类型，如果不先声明类型在使用非int变量时，返回值会不对
Sum=Cfun.add(Num1,Num2)
print("Sum is %d"%Sum)