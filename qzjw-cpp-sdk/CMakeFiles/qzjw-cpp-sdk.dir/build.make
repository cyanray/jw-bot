# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.17

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
CMAKE_COMMAND = /home/uint128/.vs/cmake/bin/cmake

# The command to remove a file.
RM = /home/uint128/.vs/cmake/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /mnt/f/Project/jw-bot

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /mnt/f/Project/jw-bot

# Include any dependencies generated for this target.
include qzjw-cpp-sdk/CMakeFiles/qzjw-cpp-sdk.dir/depend.make

# Include the progress variables for this target.
include qzjw-cpp-sdk/CMakeFiles/qzjw-cpp-sdk.dir/progress.make

# Include the compile flags for this target's objects.
include qzjw-cpp-sdk/CMakeFiles/qzjw-cpp-sdk.dir/flags.make

qzjw-cpp-sdk/CMakeFiles/qzjw-cpp-sdk.dir/src/qzjw.cpp.o: qzjw-cpp-sdk/CMakeFiles/qzjw-cpp-sdk.dir/flags.make
qzjw-cpp-sdk/CMakeFiles/qzjw-cpp-sdk.dir/src/qzjw.cpp.o: qzjw-cpp-sdk/src/qzjw.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/f/Project/jw-bot/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object qzjw-cpp-sdk/CMakeFiles/qzjw-cpp-sdk.dir/src/qzjw.cpp.o"
	cd /mnt/f/Project/jw-bot/qzjw-cpp-sdk && /usr/bin/g++-8  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/qzjw-cpp-sdk.dir/src/qzjw.cpp.o -c /mnt/f/Project/jw-bot/qzjw-cpp-sdk/src/qzjw.cpp

qzjw-cpp-sdk/CMakeFiles/qzjw-cpp-sdk.dir/src/qzjw.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/qzjw-cpp-sdk.dir/src/qzjw.cpp.i"
	cd /mnt/f/Project/jw-bot/qzjw-cpp-sdk && /usr/bin/g++-8 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/f/Project/jw-bot/qzjw-cpp-sdk/src/qzjw.cpp > CMakeFiles/qzjw-cpp-sdk.dir/src/qzjw.cpp.i

qzjw-cpp-sdk/CMakeFiles/qzjw-cpp-sdk.dir/src/qzjw.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/qzjw-cpp-sdk.dir/src/qzjw.cpp.s"
	cd /mnt/f/Project/jw-bot/qzjw-cpp-sdk && /usr/bin/g++-8 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/f/Project/jw-bot/qzjw-cpp-sdk/src/qzjw.cpp -o CMakeFiles/qzjw-cpp-sdk.dir/src/qzjw.cpp.s

# Object files for target qzjw-cpp-sdk
qzjw__cpp__sdk_OBJECTS = \
"CMakeFiles/qzjw-cpp-sdk.dir/src/qzjw.cpp.o"

# External object files for target qzjw-cpp-sdk
qzjw__cpp__sdk_EXTERNAL_OBJECTS =

qzjw-cpp-sdk/libqzjw-cpp-sdk.a: qzjw-cpp-sdk/CMakeFiles/qzjw-cpp-sdk.dir/src/qzjw.cpp.o
qzjw-cpp-sdk/libqzjw-cpp-sdk.a: qzjw-cpp-sdk/CMakeFiles/qzjw-cpp-sdk.dir/build.make
qzjw-cpp-sdk/libqzjw-cpp-sdk.a: qzjw-cpp-sdk/CMakeFiles/qzjw-cpp-sdk.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/mnt/f/Project/jw-bot/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library libqzjw-cpp-sdk.a"
	cd /mnt/f/Project/jw-bot/qzjw-cpp-sdk && $(CMAKE_COMMAND) -P CMakeFiles/qzjw-cpp-sdk.dir/cmake_clean_target.cmake
	cd /mnt/f/Project/jw-bot/qzjw-cpp-sdk && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/qzjw-cpp-sdk.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
qzjw-cpp-sdk/CMakeFiles/qzjw-cpp-sdk.dir/build: qzjw-cpp-sdk/libqzjw-cpp-sdk.a

.PHONY : qzjw-cpp-sdk/CMakeFiles/qzjw-cpp-sdk.dir/build

qzjw-cpp-sdk/CMakeFiles/qzjw-cpp-sdk.dir/clean:
	cd /mnt/f/Project/jw-bot/qzjw-cpp-sdk && $(CMAKE_COMMAND) -P CMakeFiles/qzjw-cpp-sdk.dir/cmake_clean.cmake
.PHONY : qzjw-cpp-sdk/CMakeFiles/qzjw-cpp-sdk.dir/clean

qzjw-cpp-sdk/CMakeFiles/qzjw-cpp-sdk.dir/depend:
	cd /mnt/f/Project/jw-bot && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /mnt/f/Project/jw-bot /mnt/f/Project/jw-bot/qzjw-cpp-sdk /mnt/f/Project/jw-bot /mnt/f/Project/jw-bot/qzjw-cpp-sdk /mnt/f/Project/jw-bot/qzjw-cpp-sdk/CMakeFiles/qzjw-cpp-sdk.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : qzjw-cpp-sdk/CMakeFiles/qzjw-cpp-sdk.dir/depend

