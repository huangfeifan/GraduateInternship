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
CMAKE_SOURCE_DIR = E:\huangfeifan-Sort\chip

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = E:\huangfeifan-Sort\chip\cmake-build-debug

# Utility rule file for chip_autogen.

# Include the progress variables for this target.
include CMakeFiles\chip_autogen.dir\progress.make

CMakeFiles\chip_autogen:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=E:\huangfeifan-Sort\chip\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Automatic MOC and UIC for target chip"
	echo >nul && "C:\CLion 2021.1.2\bin\cmake\win\bin\cmake.exe" -E cmake_autogen E:/huangfeifan-Sort/chip/cmake-build-debug/CMakeFiles/chip_autogen.dir/AutogenInfo.json Debug

chip_autogen: CMakeFiles\chip_autogen
chip_autogen: CMakeFiles\chip_autogen.dir\build.make

.PHONY : chip_autogen

# Rule to build all files generated by this target.
CMakeFiles\chip_autogen.dir\build: chip_autogen

.PHONY : CMakeFiles\chip_autogen.dir\build

CMakeFiles\chip_autogen.dir\clean:
	$(CMAKE_COMMAND) -P CMakeFiles\chip_autogen.dir\cmake_clean.cmake
.PHONY : CMakeFiles\chip_autogen.dir\clean

CMakeFiles\chip_autogen.dir\depend:
	$(CMAKE_COMMAND) -E cmake_depends "NMake Makefiles" E:\huangfeifan-Sort\chip E:\huangfeifan-Sort\chip E:\huangfeifan-Sort\chip\cmake-build-debug E:\huangfeifan-Sort\chip\cmake-build-debug E:\huangfeifan-Sort\chip\cmake-build-debug\CMakeFiles\chip_autogen.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles\chip_autogen.dir\depend
