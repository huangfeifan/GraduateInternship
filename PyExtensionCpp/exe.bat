echo 
rd /S /Q E:\huangfeifan-Sort\PyExtensionCpp\build
rd ./functions.cp310-win_amd64.pyd
rd ./functions.pyd
python setup.py build_ext --inplace
rename functions.cp310-win_amd64.pyd functions.pyd
