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
include CMakeFiles/VT100.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/VT100.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/VT100.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/VT100.dir/flags.make

CMakeFiles/VT100.dir/fifo.c.obj: CMakeFiles/VT100.dir/flags.make
CMakeFiles/VT100.dir/fifo.c.obj: CMakeFiles/VT100.dir/includes_C.rsp
CMakeFiles/VT100.dir/fifo.c.obj: E:/code/VT100/fifo.c
CMakeFiles/VT100.dir/fifo.c.obj: CMakeFiles/VT100.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=E:\code\VT100\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/VT100.dir/fifo.c.obj"
	C:\tool\mingw32\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/VT100.dir/fifo.c.obj -MF CMakeFiles\VT100.dir\fifo.c.obj.d -o CMakeFiles\VT100.dir\fifo.c.obj -c E:\code\VT100\fifo.c

CMakeFiles/VT100.dir/fifo.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/VT100.dir/fifo.c.i"
	C:\tool\mingw32\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E E:\code\VT100\fifo.c > CMakeFiles\VT100.dir\fifo.c.i

CMakeFiles/VT100.dir/fifo.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/VT100.dir/fifo.c.s"
	C:\tool\mingw32\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S E:\code\VT100\fifo.c -o CMakeFiles\VT100.dir\fifo.c.s

CMakeFiles/VT100.dir/main.cpp.obj: CMakeFiles/VT100.dir/flags.make
CMakeFiles/VT100.dir/main.cpp.obj: CMakeFiles/VT100.dir/includes_CXX.rsp
CMakeFiles/VT100.dir/main.cpp.obj: E:/code/VT100/main.cpp
CMakeFiles/VT100.dir/main.cpp.obj: CMakeFiles/VT100.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=E:\code\VT100\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/VT100.dir/main.cpp.obj"
	C:\tool\mingw32\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/VT100.dir/main.cpp.obj -MF CMakeFiles\VT100.dir\main.cpp.obj.d -o CMakeFiles\VT100.dir\main.cpp.obj -c E:\code\VT100\main.cpp

CMakeFiles/VT100.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/VT100.dir/main.cpp.i"
	C:\tool\mingw32\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E E:\code\VT100\main.cpp > CMakeFiles\VT100.dir\main.cpp.i

CMakeFiles/VT100.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/VT100.dir/main.cpp.s"
	C:\tool\mingw32\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S E:\code\VT100\main.cpp -o CMakeFiles\VT100.dir\main.cpp.s

CMakeFiles/VT100.dir/shell.cpp.obj: CMakeFiles/VT100.dir/flags.make
CMakeFiles/VT100.dir/shell.cpp.obj: CMakeFiles/VT100.dir/includes_CXX.rsp
CMakeFiles/VT100.dir/shell.cpp.obj: E:/code/VT100/shell.cpp
CMakeFiles/VT100.dir/shell.cpp.obj: CMakeFiles/VT100.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=E:\code\VT100\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/VT100.dir/shell.cpp.obj"
	C:\tool\mingw32\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/VT100.dir/shell.cpp.obj -MF CMakeFiles\VT100.dir\shell.cpp.obj.d -o CMakeFiles\VT100.dir\shell.cpp.obj -c E:\code\VT100\shell.cpp

CMakeFiles/VT100.dir/shell.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/VT100.dir/shell.cpp.i"
	C:\tool\mingw32\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E E:\code\VT100\shell.cpp > CMakeFiles\VT100.dir\shell.cpp.i

CMakeFiles/VT100.dir/shell.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/VT100.dir/shell.cpp.s"
	C:\tool\mingw32\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S E:\code\VT100\shell.cpp -o CMakeFiles\VT100.dir\shell.cpp.s

CMakeFiles/VT100.dir/term.cpp.obj: CMakeFiles/VT100.dir/flags.make
CMakeFiles/VT100.dir/term.cpp.obj: CMakeFiles/VT100.dir/includes_CXX.rsp
CMakeFiles/VT100.dir/term.cpp.obj: E:/code/VT100/term.cpp
CMakeFiles/VT100.dir/term.cpp.obj: CMakeFiles/VT100.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=E:\code\VT100\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/VT100.dir/term.cpp.obj"
	C:\tool\mingw32\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/VT100.dir/term.cpp.obj -MF CMakeFiles\VT100.dir\term.cpp.obj.d -o CMakeFiles\VT100.dir\term.cpp.obj -c E:\code\VT100\term.cpp

CMakeFiles/VT100.dir/term.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/VT100.dir/term.cpp.i"
	C:\tool\mingw32\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E E:\code\VT100\term.cpp > CMakeFiles\VT100.dir\term.cpp.i

CMakeFiles/VT100.dir/term.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/VT100.dir/term.cpp.s"
	C:\tool\mingw32\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S E:\code\VT100\term.cpp -o CMakeFiles\VT100.dir\term.cpp.s

# Object files for target VT100
VT100_OBJECTS = \
"CMakeFiles/VT100.dir/fifo.c.obj" \
"CMakeFiles/VT100.dir/main.cpp.obj" \
"CMakeFiles/VT100.dir/shell.cpp.obj" \
"CMakeFiles/VT100.dir/term.cpp.obj"

# External object files for target VT100
VT100_EXTERNAL_OBJECTS =

VT100.exe: CMakeFiles/VT100.dir/fifo.c.obj
VT100.exe: CMakeFiles/VT100.dir/main.cpp.obj
VT100.exe: CMakeFiles/VT100.dir/shell.cpp.obj
VT100.exe: CMakeFiles/VT100.dir/term.cpp.obj
VT100.exe: CMakeFiles/VT100.dir/build.make
VT100.exe: CMakeFiles/VT100.dir/linkLibs.rsp
VT100.exe: CMakeFiles/VT100.dir/objects1.rsp
VT100.exe: CMakeFiles/VT100.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=E:\code\VT100\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Linking CXX executable VT100.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\VT100.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/VT100.dir/build: VT100.exe
.PHONY : CMakeFiles/VT100.dir/build

CMakeFiles/VT100.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\VT100.dir\cmake_clean.cmake
.PHONY : CMakeFiles/VT100.dir/clean

CMakeFiles/VT100.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" E:\code\VT100 E:\code\VT100 E:\code\VT100\build E:\code\VT100\build E:\code\VT100\build\CMakeFiles\VT100.dir\DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/VT100.dir/depend

