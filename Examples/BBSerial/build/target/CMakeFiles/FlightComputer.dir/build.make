# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.14

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
CMAKE_COMMAND = /snap/clion/73/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /snap/clion/73/bin/cmake/linux/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/liam/Development/comp4-software/Examples/BBSerial

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/liam/Development/comp4-software/Examples/BBSerial/build/target

# Include any dependencies generated for this target.
include CMakeFiles/FlightComputer.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/FlightComputer.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/FlightComputer.dir/flags.make

CMakeFiles/FlightComputer.dir/main.cpp.o: CMakeFiles/FlightComputer.dir/flags.make
CMakeFiles/FlightComputer.dir/main.cpp.o: ../../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/liam/Development/comp4-software/Examples/BBSerial/build/target/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/FlightComputer.dir/main.cpp.o"
	/usr/bin/arm-linux-gnueabihf-g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/FlightComputer.dir/main.cpp.o -c /home/liam/Development/comp4-software/Examples/BBSerial/main.cpp

CMakeFiles/FlightComputer.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/FlightComputer.dir/main.cpp.i"
	/usr/bin/arm-linux-gnueabihf-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/liam/Development/comp4-software/Examples/BBSerial/main.cpp > CMakeFiles/FlightComputer.dir/main.cpp.i

CMakeFiles/FlightComputer.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/FlightComputer.dir/main.cpp.s"
	/usr/bin/arm-linux-gnueabihf-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/liam/Development/comp4-software/Examples/BBSerial/main.cpp -o CMakeFiles/FlightComputer.dir/main.cpp.s

# Object files for target FlightComputer
FlightComputer_OBJECTS = \
"CMakeFiles/FlightComputer.dir/main.cpp.o"

# External object files for target FlightComputer
FlightComputer_EXTERNAL_OBJECTS =

FlightComputer: CMakeFiles/FlightComputer.dir/main.cpp.o
FlightComputer: CMakeFiles/FlightComputer.dir/build.make
FlightComputer: CMakeFiles/FlightComputer.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/liam/Development/comp4-software/Examples/BBSerial/build/target/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable FlightComputer"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/FlightComputer.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/FlightComputer.dir/build: FlightComputer

.PHONY : CMakeFiles/FlightComputer.dir/build

CMakeFiles/FlightComputer.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/FlightComputer.dir/cmake_clean.cmake
.PHONY : CMakeFiles/FlightComputer.dir/clean

CMakeFiles/FlightComputer.dir/depend:
	cd /home/liam/Development/comp4-software/Examples/BBSerial/build/target && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/liam/Development/comp4-software/Examples/BBSerial /home/liam/Development/comp4-software/Examples/BBSerial /home/liam/Development/comp4-software/Examples/BBSerial/build/target /home/liam/Development/comp4-software/Examples/BBSerial/build/target /home/liam/Development/comp4-software/Examples/BBSerial/build/target/CMakeFiles/FlightComputer.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/FlightComputer.dir/depend

