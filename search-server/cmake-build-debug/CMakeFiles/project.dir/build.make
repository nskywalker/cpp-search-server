# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.25

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
CMAKE_COMMAND = "C:\Program Files\JetBrains\CLion 2023.1.3\bin\cmake\win\x64\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\JetBrains\CLion 2023.1.3\bin\cmake\win\x64\bin\cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:\Users\googl\cpp-search-server\search-server

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\Users\googl\cpp-search-server\search-server\cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/project.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/project.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/project.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/project.dir/flags.make

CMakeFiles/project.dir/document.cpp.obj: CMakeFiles/project.dir/flags.make
CMakeFiles/project.dir/document.cpp.obj: CMakeFiles/project.dir/includes_CXX.rsp
CMakeFiles/project.dir/document.cpp.obj: C:/Users/googl/cpp-search-server/search-server/document.cpp
CMakeFiles/project.dir/document.cpp.obj: CMakeFiles/project.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\googl\cpp-search-server\search-server\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/project.dir/document.cpp.obj"
	C:\dev\mingw64\bin\c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/project.dir/document.cpp.obj -MF CMakeFiles\project.dir\document.cpp.obj.d -o CMakeFiles\project.dir\document.cpp.obj -c C:\Users\googl\cpp-search-server\search-server\document.cpp

CMakeFiles/project.dir/document.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/project.dir/document.cpp.i"
	C:\dev\mingw64\bin\c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\googl\cpp-search-server\search-server\document.cpp > CMakeFiles\project.dir\document.cpp.i

CMakeFiles/project.dir/document.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/project.dir/document.cpp.s"
	C:\dev\mingw64\bin\c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\googl\cpp-search-server\search-server\document.cpp -o CMakeFiles\project.dir\document.cpp.s

CMakeFiles/project.dir/main.cpp.obj: CMakeFiles/project.dir/flags.make
CMakeFiles/project.dir/main.cpp.obj: CMakeFiles/project.dir/includes_CXX.rsp
CMakeFiles/project.dir/main.cpp.obj: C:/Users/googl/cpp-search-server/search-server/main.cpp
CMakeFiles/project.dir/main.cpp.obj: CMakeFiles/project.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\googl\cpp-search-server\search-server\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/project.dir/main.cpp.obj"
	C:\dev\mingw64\bin\c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/project.dir/main.cpp.obj -MF CMakeFiles\project.dir\main.cpp.obj.d -o CMakeFiles\project.dir\main.cpp.obj -c C:\Users\googl\cpp-search-server\search-server\main.cpp

CMakeFiles/project.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/project.dir/main.cpp.i"
	C:\dev\mingw64\bin\c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\googl\cpp-search-server\search-server\main.cpp > CMakeFiles\project.dir\main.cpp.i

CMakeFiles/project.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/project.dir/main.cpp.s"
	C:\dev\mingw64\bin\c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\googl\cpp-search-server\search-server\main.cpp -o CMakeFiles\project.dir\main.cpp.s

CMakeFiles/project.dir/read_input_functions.cpp.obj: CMakeFiles/project.dir/flags.make
CMakeFiles/project.dir/read_input_functions.cpp.obj: CMakeFiles/project.dir/includes_CXX.rsp
CMakeFiles/project.dir/read_input_functions.cpp.obj: C:/Users/googl/cpp-search-server/search-server/read_input_functions.cpp
CMakeFiles/project.dir/read_input_functions.cpp.obj: CMakeFiles/project.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\googl\cpp-search-server\search-server\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/project.dir/read_input_functions.cpp.obj"
	C:\dev\mingw64\bin\c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/project.dir/read_input_functions.cpp.obj -MF CMakeFiles\project.dir\read_input_functions.cpp.obj.d -o CMakeFiles\project.dir\read_input_functions.cpp.obj -c C:\Users\googl\cpp-search-server\search-server\read_input_functions.cpp

CMakeFiles/project.dir/read_input_functions.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/project.dir/read_input_functions.cpp.i"
	C:\dev\mingw64\bin\c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\googl\cpp-search-server\search-server\read_input_functions.cpp > CMakeFiles\project.dir\read_input_functions.cpp.i

CMakeFiles/project.dir/read_input_functions.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/project.dir/read_input_functions.cpp.s"
	C:\dev\mingw64\bin\c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\googl\cpp-search-server\search-server\read_input_functions.cpp -o CMakeFiles\project.dir\read_input_functions.cpp.s

CMakeFiles/project.dir/request_queue.cpp.obj: CMakeFiles/project.dir/flags.make
CMakeFiles/project.dir/request_queue.cpp.obj: CMakeFiles/project.dir/includes_CXX.rsp
CMakeFiles/project.dir/request_queue.cpp.obj: C:/Users/googl/cpp-search-server/search-server/request_queue.cpp
CMakeFiles/project.dir/request_queue.cpp.obj: CMakeFiles/project.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\googl\cpp-search-server\search-server\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/project.dir/request_queue.cpp.obj"
	C:\dev\mingw64\bin\c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/project.dir/request_queue.cpp.obj -MF CMakeFiles\project.dir\request_queue.cpp.obj.d -o CMakeFiles\project.dir\request_queue.cpp.obj -c C:\Users\googl\cpp-search-server\search-server\request_queue.cpp

CMakeFiles/project.dir/request_queue.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/project.dir/request_queue.cpp.i"
	C:\dev\mingw64\bin\c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\googl\cpp-search-server\search-server\request_queue.cpp > CMakeFiles\project.dir\request_queue.cpp.i

CMakeFiles/project.dir/request_queue.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/project.dir/request_queue.cpp.s"
	C:\dev\mingw64\bin\c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\googl\cpp-search-server\search-server\request_queue.cpp -o CMakeFiles\project.dir\request_queue.cpp.s

CMakeFiles/project.dir/search_server.cpp.obj: CMakeFiles/project.dir/flags.make
CMakeFiles/project.dir/search_server.cpp.obj: CMakeFiles/project.dir/includes_CXX.rsp
CMakeFiles/project.dir/search_server.cpp.obj: C:/Users/googl/cpp-search-server/search-server/search_server.cpp
CMakeFiles/project.dir/search_server.cpp.obj: CMakeFiles/project.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\googl\cpp-search-server\search-server\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/project.dir/search_server.cpp.obj"
	C:\dev\mingw64\bin\c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/project.dir/search_server.cpp.obj -MF CMakeFiles\project.dir\search_server.cpp.obj.d -o CMakeFiles\project.dir\search_server.cpp.obj -c C:\Users\googl\cpp-search-server\search-server\search_server.cpp

CMakeFiles/project.dir/search_server.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/project.dir/search_server.cpp.i"
	C:\dev\mingw64\bin\c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\googl\cpp-search-server\search-server\search_server.cpp > CMakeFiles\project.dir\search_server.cpp.i

CMakeFiles/project.dir/search_server.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/project.dir/search_server.cpp.s"
	C:\dev\mingw64\bin\c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\googl\cpp-search-server\search-server\search_server.cpp -o CMakeFiles\project.dir\search_server.cpp.s

CMakeFiles/project.dir/string_processing.cpp.obj: CMakeFiles/project.dir/flags.make
CMakeFiles/project.dir/string_processing.cpp.obj: CMakeFiles/project.dir/includes_CXX.rsp
CMakeFiles/project.dir/string_processing.cpp.obj: C:/Users/googl/cpp-search-server/search-server/string_processing.cpp
CMakeFiles/project.dir/string_processing.cpp.obj: CMakeFiles/project.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\googl\cpp-search-server\search-server\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/project.dir/string_processing.cpp.obj"
	C:\dev\mingw64\bin\c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/project.dir/string_processing.cpp.obj -MF CMakeFiles\project.dir\string_processing.cpp.obj.d -o CMakeFiles\project.dir\string_processing.cpp.obj -c C:\Users\googl\cpp-search-server\search-server\string_processing.cpp

CMakeFiles/project.dir/string_processing.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/project.dir/string_processing.cpp.i"
	C:\dev\mingw64\bin\c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\googl\cpp-search-server\search-server\string_processing.cpp > CMakeFiles\project.dir\string_processing.cpp.i

CMakeFiles/project.dir/string_processing.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/project.dir/string_processing.cpp.s"
	C:\dev\mingw64\bin\c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\googl\cpp-search-server\search-server\string_processing.cpp -o CMakeFiles\project.dir\string_processing.cpp.s

CMakeFiles/project.dir/process_queries.cpp.obj: CMakeFiles/project.dir/flags.make
CMakeFiles/project.dir/process_queries.cpp.obj: CMakeFiles/project.dir/includes_CXX.rsp
CMakeFiles/project.dir/process_queries.cpp.obj: C:/Users/googl/cpp-search-server/search-server/process_queries.cpp
CMakeFiles/project.dir/process_queries.cpp.obj: CMakeFiles/project.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\googl\cpp-search-server\search-server\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/project.dir/process_queries.cpp.obj"
	C:\dev\mingw64\bin\c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/project.dir/process_queries.cpp.obj -MF CMakeFiles\project.dir\process_queries.cpp.obj.d -o CMakeFiles\project.dir\process_queries.cpp.obj -c C:\Users\googl\cpp-search-server\search-server\process_queries.cpp

CMakeFiles/project.dir/process_queries.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/project.dir/process_queries.cpp.i"
	C:\dev\mingw64\bin\c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\googl\cpp-search-server\search-server\process_queries.cpp > CMakeFiles\project.dir\process_queries.cpp.i

CMakeFiles/project.dir/process_queries.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/project.dir/process_queries.cpp.s"
	C:\dev\mingw64\bin\c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\googl\cpp-search-server\search-server\process_queries.cpp -o CMakeFiles\project.dir\process_queries.cpp.s

# Object files for target project
project_OBJECTS = \
"CMakeFiles/project.dir/document.cpp.obj" \
"CMakeFiles/project.dir/main.cpp.obj" \
"CMakeFiles/project.dir/read_input_functions.cpp.obj" \
"CMakeFiles/project.dir/request_queue.cpp.obj" \
"CMakeFiles/project.dir/search_server.cpp.obj" \
"CMakeFiles/project.dir/string_processing.cpp.obj" \
"CMakeFiles/project.dir/process_queries.cpp.obj"

# External object files for target project
project_EXTERNAL_OBJECTS =

project.exe: CMakeFiles/project.dir/document.cpp.obj
project.exe: CMakeFiles/project.dir/main.cpp.obj
project.exe: CMakeFiles/project.dir/read_input_functions.cpp.obj
project.exe: CMakeFiles/project.dir/request_queue.cpp.obj
project.exe: CMakeFiles/project.dir/search_server.cpp.obj
project.exe: CMakeFiles/project.dir/string_processing.cpp.obj
project.exe: CMakeFiles/project.dir/process_queries.cpp.obj
project.exe: CMakeFiles/project.dir/build.make
project.exe: CMakeFiles/project.dir/linkLibs.rsp
project.exe: CMakeFiles/project.dir/objects1
project.exe: CMakeFiles/project.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=C:\Users\googl\cpp-search-server\search-server\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Linking CXX executable project.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\project.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/project.dir/build: project.exe
.PHONY : CMakeFiles/project.dir/build

CMakeFiles/project.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\project.dir\cmake_clean.cmake
.PHONY : CMakeFiles/project.dir/clean

CMakeFiles/project.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\Users\googl\cpp-search-server\search-server C:\Users\googl\cpp-search-server\search-server C:\Users\googl\cpp-search-server\search-server\cmake-build-debug C:\Users\googl\cpp-search-server\search-server\cmake-build-debug C:\Users\googl\cpp-search-server\search-server\cmake-build-debug\CMakeFiles\project.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/project.dir/depend
