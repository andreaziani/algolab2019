# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.15

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.15.3/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.15.3/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/andrea/Documents/ETH/AlgorithmsLabs/week10/goldeneye

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/andrea/Documents/ETH/AlgorithmsLabs/week10/goldeneye

# Include any dependencies generated for this target.
include CMakeFiles/emst.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/emst.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/emst.dir/flags.make

CMakeFiles/emst.dir/emst.cpp.o: CMakeFiles/emst.dir/flags.make
CMakeFiles/emst.dir/emst.cpp.o: emst.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/andrea/Documents/ETH/AlgorithmsLabs/week10/goldeneye/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/emst.dir/emst.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/emst.dir/emst.cpp.o -c /Users/andrea/Documents/ETH/AlgorithmsLabs/week10/goldeneye/emst.cpp

CMakeFiles/emst.dir/emst.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/emst.dir/emst.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/andrea/Documents/ETH/AlgorithmsLabs/week10/goldeneye/emst.cpp > CMakeFiles/emst.dir/emst.cpp.i

CMakeFiles/emst.dir/emst.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/emst.dir/emst.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/andrea/Documents/ETH/AlgorithmsLabs/week10/goldeneye/emst.cpp -o CMakeFiles/emst.dir/emst.cpp.s

# Object files for target emst
emst_OBJECTS = \
"CMakeFiles/emst.dir/emst.cpp.o"

# External object files for target emst
emst_EXTERNAL_OBJECTS =

emst: CMakeFiles/emst.dir/emst.cpp.o
emst: CMakeFiles/emst.dir/build.make
emst: /usr/local/lib/libCGAL.13.0.3.dylib
emst: /usr/local/lib/libmpfr.dylib
emst: /usr/local/lib/libgmp.dylib
emst: /usr/local/lib/libboost_thread-mt.dylib
emst: CMakeFiles/emst.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/andrea/Documents/ETH/AlgorithmsLabs/week10/goldeneye/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable emst"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/emst.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/emst.dir/build: emst

.PHONY : CMakeFiles/emst.dir/build

CMakeFiles/emst.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/emst.dir/cmake_clean.cmake
.PHONY : CMakeFiles/emst.dir/clean

CMakeFiles/emst.dir/depend:
	cd /Users/andrea/Documents/ETH/AlgorithmsLabs/week10/goldeneye && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/andrea/Documents/ETH/AlgorithmsLabs/week10/goldeneye /Users/andrea/Documents/ETH/AlgorithmsLabs/week10/goldeneye /Users/andrea/Documents/ETH/AlgorithmsLabs/week10/goldeneye /Users/andrea/Documents/ETH/AlgorithmsLabs/week10/goldeneye /Users/andrea/Documents/ETH/AlgorithmsLabs/week10/goldeneye/CMakeFiles/emst.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/emst.dir/depend

