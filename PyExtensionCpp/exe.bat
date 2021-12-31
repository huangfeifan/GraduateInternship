echo
rd /S /Q D:\Master degree\CLionProjects\Gerber_compare_pyd\build
del /S /Q "E:\huangfeifan-Sort\PyExtensionCpp\functions.pyd"
python setup.py build_ext --inplace
rename functions.cp310-win_amd64.pyd functions.pyd
copy functions.pyd C:\ProgramData\Anaconda3\DLLs
