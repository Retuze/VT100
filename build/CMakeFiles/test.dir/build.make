# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.30

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

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

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = C:\tool\mingw32\bin\cmake.exe

# The command to remove a file.
RM = C:\tool\mingw32\bin\cmake.exe -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = E:\code\VT100

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = E:\code\VT100\build

# Include any dependencies generated for this target.
include CMakeFiles/test.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/test.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/test.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/test.dir/flags.make

CMakeFiles/test.dir/fifo.c.obj: CMakeFiles/test.dir/flags.make
CMakeFiles/test.dir/fifo.c.obj: CMakeFiles/test.dir/includes_C.rsp
CMakeFiles/test.dir/fifo.c.obj: E:/code/VT100/fifo.c
CMakeFiles/test.dir/fifo.c.obj: CMakeFiles/test.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=E:\code\VT100\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/test.dir/fifo.c.obj"
	C:\tool\mingw32\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/test.dir/fifo.c.obj -MF CMakeFiles\test.dir\fifo.c.obj.d -o CMakeFiles\test.dir\fifo.c.obj -c E:\code\VT100\fifo.c

CMakeFiles/test.dir/fifo.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/test.dir/fifo.c.i"
	C:\tool\mingw32\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E E:\code\VT100\fifo.c > CMakeFiles\test.dir\fifo.c.i

CMakeFiles/test.dir/fifo.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/test.dir/fifo.c.s"
	C:\tool\mingw32\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S E:\code\VT100\fifo.c -o CMakeFiles\test.dir\fifo.c.s

CMakeFiles/test.dir/main.cpp.obj: CMakeFiles/test.dir/flags.make
CMakeFiles/test.dir/main.cpp.obj: CMakeFiles/test.dir/includes_CXX.rsp
CMakeFiles/test.dir/main.cpp.obj: E:/code/VT100/main.cpp
CMakeFiles/test.dir/main.cpp.obj: CMakeFiles/test.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=E:\code\VT100\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/test.dir/main.cpp.obj"
	C:\tool\mingw32\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/test.dir/main.cpp.obj -MF CMakeFiles\test.dir\main.cpp.obj.d -o CMakeFiles\test.dir\main.cpp.obj -c E:\code\VT100\main.cpp

CMakeFiles/test.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/test.dir/main.cpp.i"
	C:\tool\mingw32\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E E:\code\VT100\main.cpp > CMakeFiles\test.dir\main.cpp.i

CMakeFiles/test.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/test.dir/main.cpp.s"
	C:\tool\mingw32\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S E:\code\VT100\main.cpp -o CMakeFiles\test.dir\main.cpp.s

CMakeFiles/test.dir/shell.cpp.obj: CMakeFiles/test.dir/flags.make
CMakeFiles/test.dir/shell.cpp.obj: CMakeFiles/test.dir/includes_CXX.rsp
CMakeFiles/test.dir/shell.cpp.obj: E:/code/VT100/shell.cpp
CMakeFiles/test.dir/shell.cpp.obj: CMakeFiles/test.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=E:\code\VT100\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/test.dir/shell.cpp.obj"
	C:\tool\mingw32\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/test.dir/shell.cpp.obj -MF CMakeFiles\test.dir\shell.cpp.obj.d -o CMakeFiles\test.dir\shell.cpp.obj -c E:\code\VT100\shell.cpp

CMakeFiles/test.dir/shell.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/test.dir/shell.cpp.i"
	C:\tool\mingw32\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E E:\code\VT100\shell.cpp > CMakeFiles\test.dir\shell.cpp.i

CMakeFiles/test.dir/shell.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/test.dir/shell.cpp.s"
	C:\tool\mingw32\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S E:\code\VT100\shell.cpp -o CMakeFiles\test.dir\shell.cpp.s

CMakeFiles/test.dir/term.cpp.obj: CMakeFiles/test.dir/flags.make
CMakeFiles/test.dir/term.cpp.obj: CMakeFiles/test.dir/includes_CXX.rsp
CMakeFiles/test.dir/term.cpp.obj: E:/code/VT100/term.cpp
CMakeFiles/test.dir/term.cpp.obj: CMakeFiles/test.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=E:\code\VT100\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/test.dir/term.cpp.obj"
	C:\tool\mingw32\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/test.dir/term.cpp.obj -MF CMakeFiles\test.dir\term.cpp.obj.d -o CMakeFiles\test.dir\term.cpp.obj -c E:\code\VT100\term.cpp

CMakeFiles/test.dir/term.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/test.dir/term.cpp.i"
	C:\tool\mingw32\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E E:\code\VT100\term.cpp > CMakeFiles\test.dir\term.cpp.i

CMakeFiles/test.dir/term.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/test.dir/term.cpp.s"
	C:\tool\mingw32\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S E:\code\VT100\term.cpp -o CMakeFiles\test.dir\term.cpp.s

# Object files for target test
test_OBJECTS = \
"CMakeFiles/test.dir/fifo.c.obj" \
"CMakeFiles/test.dir/main.cpp.obj" \
"CMakeFiles/test.dir/shell.cpp.obj" \
"CMakeFiles/test.dir/term.cpp.obj"

# External object files for target test
test_EXTERNAL_OBJECTS =

test.exe: CMakeFiles/test.dir/fifo.c.obj
test.exe: CMakeFiles/test.dir/main.cpp.obj
test.exe: CMakeFiles/test.dir/shell.cpp.obj
test.exe: CMakeFiles/test.dir/term.cpp.obj
test.exe: CMakeFiles/test.dir/build.make
test.exe: CMakeFiles/test.dir/linkLibs.rsp
test.exe: CMakeFiles/test.dir/objects1.rsp
test.exe: CMakeFiles/test.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=E:\code\VT100\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Linking CXX executable test.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\test.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/test.dir/build: test.exe
.PHONY : CMakeFiles/test.dir/build

CMakeFiles/test.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\test.dir\cmake_clean.cmake
.PHONY : CMakeFiles/test.dir/clean

CMakeFiles/test.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" E:\code\VT100 E:\code\VT100 E:\code\VT100\build E:\code\VT100\build E:\code\VT100\build\CMakeFiles\test.dir\DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/test.dir/depend

