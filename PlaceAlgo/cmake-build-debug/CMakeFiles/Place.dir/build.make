# CMAKE generated file: DO NOT EDIT!
# Generated by "NMake Makefiles" Generator, CMake Version 3.19

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


.SUFFIXES: .hpux_make_needs_suffix_list


# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE
NULL=nul
!ENDIF
SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\CLion 2021.1.2\bin\cmake\win\bin\cmake.exe"

# The command to remove a file.
RM = "C:\CLion 2021.1.2\bin\cmake\win\bin\cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = E:\huangfeifan-Sort\PlaceAlgo

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = E:\huangfeifan-Sort\PlaceAlgo\cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles\Place.dir\depend.make

# Include the progress variables for this target.
include CMakeFiles\Place.dir\progress.make

# Include the compile flags for this target's objects.
include CMakeFiles\Place.dir\flags.make

CMakeFiles\Place.dir\Place_autogen\mocs_compilation.cpp.obj: CMakeFiles\Place.dir\flags.make
CMakeFiles\Place.dir\Place_autogen\mocs_compilation.cpp.obj: Place_autogen\mocs_compilation.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=E:\huangfeifan-Sort\PlaceAlgo\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/Place.dir/Place_autogen/mocs_compilation.cpp.obj"
	C:\PROGRA~2\MICROS~2\2017\COMMUN~1\VC\Tools\MSVC\1416~1.270\bin\Hostx86\x64\cl.exe @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /FoCMakeFiles\Place.dir\Place_autogen\mocs_compilation.cpp.obj /FdCMakeFiles\Place.dir\ /FS -c E:\huangfeifan-Sort\PlaceAlgo\cmake-build-debug\Place_autogen\mocs_compilation.cpp
<<

CMakeFiles\Place.dir\Place_autogen\mocs_compilation.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Place.dir/Place_autogen/mocs_compilation.cpp.i"
	C:\PROGRA~2\MICROS~2\2017\COMMUN~1\VC\Tools\MSVC\1416~1.270\bin\Hostx86\x64\cl.exe > CMakeFiles\Place.dir\Place_autogen\mocs_compilation.cpp.i @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E E:\huangfeifan-Sort\PlaceAlgo\cmake-build-debug\Place_autogen\mocs_compilation.cpp
<<

CMakeFiles\Place.dir\Place_autogen\mocs_compilation.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Place.dir/Place_autogen/mocs_compilation.cpp.s"
	C:\PROGRA~2\MICROS~2\2017\COMMUN~1\VC\Tools\MSVC\1416~1.270\bin\Hostx86\x64\cl.exe @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /FoNUL /FAs /FaCMakeFiles\Place.dir\Place_autogen\mocs_compilation.cpp.s /c E:\huangfeifan-Sort\PlaceAlgo\cmake-build-debug\Place_autogen\mocs_compilation.cpp
<<

CMakeFiles\Place.dir\src\CompCompute.cpp.obj: CMakeFiles\Place.dir\flags.make
CMakeFiles\Place.dir\src\CompCompute.cpp.obj: ..\src\CompCompute.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=E:\huangfeifan-Sort\PlaceAlgo\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/Place.dir/src/CompCompute.cpp.obj"
	C:\PROGRA~2\MICROS~2\2017\COMMUN~1\VC\Tools\MSVC\1416~1.270\bin\Hostx86\x64\cl.exe @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /FoCMakeFiles\Place.dir\src\CompCompute.cpp.obj /FdCMakeFiles\Place.dir\ /FS -c E:\huangfeifan-Sort\PlaceAlgo\src\CompCompute.cpp
<<

CMakeFiles\Place.dir\src\CompCompute.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Place.dir/src/CompCompute.cpp.i"
	C:\PROGRA~2\MICROS~2\2017\COMMUN~1\VC\Tools\MSVC\1416~1.270\bin\Hostx86\x64\cl.exe > CMakeFiles\Place.dir\src\CompCompute.cpp.i @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E E:\huangfeifan-Sort\PlaceAlgo\src\CompCompute.cpp
<<

CMakeFiles\Place.dir\src\CompCompute.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Place.dir/src/CompCompute.cpp.s"
	C:\PROGRA~2\MICROS~2\2017\COMMUN~1\VC\Tools\MSVC\1416~1.270\bin\Hostx86\x64\cl.exe @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /FoNUL /FAs /FaCMakeFiles\Place.dir\src\CompCompute.cpp.s /c E:\huangfeifan-Sort\PlaceAlgo\src\CompCompute.cpp
<<

CMakeFiles\Place.dir\src\GetLayout.cpp.obj: CMakeFiles\Place.dir\flags.make
CMakeFiles\Place.dir\src\GetLayout.cpp.obj: ..\src\GetLayout.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=E:\huangfeifan-Sort\PlaceAlgo\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/Place.dir/src/GetLayout.cpp.obj"
	C:\PROGRA~2\MICROS~2\2017\COMMUN~1\VC\Tools\MSVC\1416~1.270\bin\Hostx86\x64\cl.exe @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /FoCMakeFiles\Place.dir\src\GetLayout.cpp.obj /FdCMakeFiles\Place.dir\ /FS -c E:\huangfeifan-Sort\PlaceAlgo\src\GetLayout.cpp
<<

CMakeFiles\Place.dir\src\GetLayout.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Place.dir/src/GetLayout.cpp.i"
	C:\PROGRA~2\MICROS~2\2017\COMMUN~1\VC\Tools\MSVC\1416~1.270\bin\Hostx86\x64\cl.exe > CMakeFiles\Place.dir\src\GetLayout.cpp.i @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E E:\huangfeifan-Sort\PlaceAlgo\src\GetLayout.cpp
<<

CMakeFiles\Place.dir\src\GetLayout.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Place.dir/src/GetLayout.cpp.s"
	C:\PROGRA~2\MICROS~2\2017\COMMUN~1\VC\Tools\MSVC\1416~1.270\bin\Hostx86\x64\cl.exe @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /FoNUL /FAs /FaCMakeFiles\Place.dir\src\GetLayout.cpp.s /c E:\huangfeifan-Sort\PlaceAlgo\src\GetLayout.cpp
<<

CMakeFiles\Place.dir\src\main.cpp.obj: CMakeFiles\Place.dir\flags.make
CMakeFiles\Place.dir\src\main.cpp.obj: ..\src\main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=E:\huangfeifan-Sort\PlaceAlgo\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/Place.dir/src/main.cpp.obj"
	C:\PROGRA~2\MICROS~2\2017\COMMUN~1\VC\Tools\MSVC\1416~1.270\bin\Hostx86\x64\cl.exe @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /FoCMakeFiles\Place.dir\src\main.cpp.obj /FdCMakeFiles\Place.dir\ /FS -c E:\huangfeifan-Sort\PlaceAlgo\src\main.cpp
<<

CMakeFiles\Place.dir\src\main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Place.dir/src/main.cpp.i"
	C:\PROGRA~2\MICROS~2\2017\COMMUN~1\VC\Tools\MSVC\1416~1.270\bin\Hostx86\x64\cl.exe > CMakeFiles\Place.dir\src\main.cpp.i @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E E:\huangfeifan-Sort\PlaceAlgo\src\main.cpp
<<

CMakeFiles\Place.dir\src\main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Place.dir/src/main.cpp.s"
	C:\PROGRA~2\MICROS~2\2017\COMMUN~1\VC\Tools\MSVC\1416~1.270\bin\Hostx86\x64\cl.exe @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /FoNUL /FAs /FaCMakeFiles\Place.dir\src\main.cpp.s /c E:\huangfeifan-Sort\PlaceAlgo\src\main.cpp
<<

# Object files for target Place
Place_OBJECTS = \
"CMakeFiles\Place.dir\Place_autogen\mocs_compilation.cpp.obj" \
"CMakeFiles\Place.dir\src\CompCompute.cpp.obj" \
"CMakeFiles\Place.dir\src\GetLayout.cpp.obj" \
"CMakeFiles\Place.dir\src\main.cpp.obj"

# External object files for target Place
Place_EXTERNAL_OBJECTS =

Place.exe: CMakeFiles\Place.dir\Place_autogen\mocs_compilation.cpp.obj
Place.exe: CMakeFiles\Place.dir\src\CompCompute.cpp.obj
Place.exe: CMakeFiles\Place.dir\src\GetLayout.cpp.obj
Place.exe: CMakeFiles\Place.dir\src\main.cpp.obj
Place.exe: CMakeFiles\Place.dir\build.make
Place.exe: D:\Qt5.12.6\5.12.6\msvc2017_64\lib\Qt5Widgetsd.lib
Place.exe: D:\Qt5.12.6\5.12.6\msvc2017_64\lib\Qt5Guid.lib
Place.exe: D:\Qt5.12.6\5.12.6\msvc2017_64\lib\Qt5Cored.lib
Place.exe: CMakeFiles\Place.dir\objects1.rsp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=E:\huangfeifan-Sort\PlaceAlgo\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Linking CXX executable Place.exe"
	"C:\CLion 2021.1.2\bin\cmake\win\bin\cmake.exe" -E vs_link_exe --intdir=CMakeFiles\Place.dir --rc=C:\PROGRA~2\WI3CF2~1\10\bin\100177~1.0\x86\rc.exe --mt=C:\PROGRA~2\WI3CF2~1\10\bin\100177~1.0\x86\mt.exe --manifests -- C:\PROGRA~2\MICROS~2\2017\COMMUN~1\VC\Tools\MSVC\1416~1.270\bin\Hostx86\x64\link.exe /nologo @CMakeFiles\Place.dir\objects1.rsp @<<
 /out:Place.exe /implib:Place.lib /pdb:E:\huangfeifan-Sort\PlaceAlgo\cmake-build-debug\Place.pdb /version:0.0 /machine:x64 /debug /INCREMENTAL /subsystem:console  D:\Qt5.12.6\5.12.6\msvc2017_64\lib\Qt5Widgetsd.lib D:\Qt5.12.6\5.12.6\msvc2017_64\lib\Qt5Guid.lib D:\Qt5.12.6\5.12.6\msvc2017_64\lib\Qt5Cored.lib kernel32.lib user32.lib gdi32.lib winspool.lib shell32.lib ole32.lib oleaut32.lib uuid.lib comdlg32.lib advapi32.lib 
<<
	echo >nul && "C:\CLion 2021.1.2\bin\cmake\win\bin\cmake.exe" -E make_directory E:/huangfeifan-Sort/PlaceAlgo/cmake-build-debug/plugins/platforms/
	echo >nul && "C:\CLion 2021.1.2\bin\cmake\win\bin\cmake.exe" -E copy D:/Qt5.12.6/5.12.6/msvc2017_64/plugins/platforms/qwindowsd.dll E:/huangfeifan-Sort/PlaceAlgo/cmake-build-debug/plugins/platforms/
	echo >nul && "C:\CLion 2021.1.2\bin\cmake\win\bin\cmake.exe" -E copy D:/Qt5.12.6/5.12.6/msvc2017_64/bin/Qt5Cored.dll E:/huangfeifan-Sort/PlaceAlgo/cmake-build-debug
	echo >nul && "C:\CLion 2021.1.2\bin\cmake\win\bin\cmake.exe" -E copy D:/Qt5.12.6/5.12.6/msvc2017_64/bin/Qt5Guid.dll E:/huangfeifan-Sort/PlaceAlgo/cmake-build-debug
	echo >nul && "C:\CLion 2021.1.2\bin\cmake\win\bin\cmake.exe" -E copy D:/Qt5.12.6/5.12.6/msvc2017_64/bin/Qt5Widgetsd.dll E:/huangfeifan-Sort/PlaceAlgo/cmake-build-debug

# Rule to build all files generated by this target.
CMakeFiles\Place.dir\build: Place.exe

.PHONY : CMakeFiles\Place.dir\build

CMakeFiles\Place.dir\clean:
	$(CMAKE_COMMAND) -P CMakeFiles\Place.dir\cmake_clean.cmake
.PHONY : CMakeFiles\Place.dir\clean

CMakeFiles\Place.dir\depend:
	$(CMAKE_COMMAND) -E cmake_depends "NMake Makefiles" E:\huangfeifan-Sort\PlaceAlgo E:\huangfeifan-Sort\PlaceAlgo E:\huangfeifan-Sort\PlaceAlgo\cmake-build-debug E:\huangfeifan-Sort\PlaceAlgo\cmake-build-debug E:\huangfeifan-Sort\PlaceAlgo\cmake-build-debug\CMakeFiles\Place.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles\Place.dir\depend

