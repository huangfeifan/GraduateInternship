echo
rd /S /Q E:\huangfeifan-Sort\PyExtensionCpp\build
del /S /Q E:\huangfeifan-Sort\PyExtensionCpp\build
del /S /Q "E:\huangfeifan-Sort\PyExtensionCpp\functions.pyd"
del /S /Q "E:\huangfeifan-Sort\PyExtensionCpp\HuaseenGerberCompare.pyd"
python setup.py build_ext --inplace
rename HuaseenGerberCompare.cp310-win_amd64.pyd HuaseenGerberCompare.pyd
copy HuaseenGerberCompare.pyd D:\Application_Tools\Anaconda\DLLs
