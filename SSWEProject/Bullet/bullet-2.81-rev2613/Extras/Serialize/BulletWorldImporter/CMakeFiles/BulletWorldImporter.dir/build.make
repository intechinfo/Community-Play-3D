# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/parallels/Desktop/Soganatsu/SSWEProject/Bullet/bullet-2.81-rev2613

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/parallels/Desktop/Soganatsu/SSWEProject/Bullet/bullet-2.81-rev2613

# Include any dependencies generated for this target.
include Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/depend.make

# Include the progress variables for this target.
include Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/progress.make

# Include the compile flags for this target's objects.
include Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/flags.make

Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/btBulletWorldImporter.o: Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/flags.make
Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/btBulletWorldImporter.o: Extras/Serialize/BulletWorldImporter/btBulletWorldImporter.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/parallels/Desktop/Soganatsu/SSWEProject/Bullet/bullet-2.81-rev2613/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/btBulletWorldImporter.o"
	cd /home/parallels/Desktop/Soganatsu/SSWEProject/Bullet/bullet-2.81-rev2613/Extras/Serialize/BulletWorldImporter && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/BulletWorldImporter.dir/btBulletWorldImporter.o -c /home/parallels/Desktop/Soganatsu/SSWEProject/Bullet/bullet-2.81-rev2613/Extras/Serialize/BulletWorldImporter/btBulletWorldImporter.cpp

Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/btBulletWorldImporter.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/BulletWorldImporter.dir/btBulletWorldImporter.i"
	cd /home/parallels/Desktop/Soganatsu/SSWEProject/Bullet/bullet-2.81-rev2613/Extras/Serialize/BulletWorldImporter && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/parallels/Desktop/Soganatsu/SSWEProject/Bullet/bullet-2.81-rev2613/Extras/Serialize/BulletWorldImporter/btBulletWorldImporter.cpp > CMakeFiles/BulletWorldImporter.dir/btBulletWorldImporter.i

Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/btBulletWorldImporter.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/BulletWorldImporter.dir/btBulletWorldImporter.s"
	cd /home/parallels/Desktop/Soganatsu/SSWEProject/Bullet/bullet-2.81-rev2613/Extras/Serialize/BulletWorldImporter && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/parallels/Desktop/Soganatsu/SSWEProject/Bullet/bullet-2.81-rev2613/Extras/Serialize/BulletWorldImporter/btBulletWorldImporter.cpp -o CMakeFiles/BulletWorldImporter.dir/btBulletWorldImporter.s

Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/btBulletWorldImporter.o.requires:
.PHONY : Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/btBulletWorldImporter.o.requires

Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/btBulletWorldImporter.o.provides: Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/btBulletWorldImporter.o.requires
	$(MAKE) -f Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/build.make Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/btBulletWorldImporter.o.provides.build
.PHONY : Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/btBulletWorldImporter.o.provides

Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/btBulletWorldImporter.o.provides.build: Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/btBulletWorldImporter.o

Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/btWorldImporter.o: Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/flags.make
Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/btWorldImporter.o: Extras/Serialize/BulletWorldImporter/btWorldImporter.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/parallels/Desktop/Soganatsu/SSWEProject/Bullet/bullet-2.81-rev2613/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/btWorldImporter.o"
	cd /home/parallels/Desktop/Soganatsu/SSWEProject/Bullet/bullet-2.81-rev2613/Extras/Serialize/BulletWorldImporter && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/BulletWorldImporter.dir/btWorldImporter.o -c /home/parallels/Desktop/Soganatsu/SSWEProject/Bullet/bullet-2.81-rev2613/Extras/Serialize/BulletWorldImporter/btWorldImporter.cpp

Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/btWorldImporter.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/BulletWorldImporter.dir/btWorldImporter.i"
	cd /home/parallels/Desktop/Soganatsu/SSWEProject/Bullet/bullet-2.81-rev2613/Extras/Serialize/BulletWorldImporter && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/parallels/Desktop/Soganatsu/SSWEProject/Bullet/bullet-2.81-rev2613/Extras/Serialize/BulletWorldImporter/btWorldImporter.cpp > CMakeFiles/BulletWorldImporter.dir/btWorldImporter.i

Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/btWorldImporter.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/BulletWorldImporter.dir/btWorldImporter.s"
	cd /home/parallels/Desktop/Soganatsu/SSWEProject/Bullet/bullet-2.81-rev2613/Extras/Serialize/BulletWorldImporter && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/parallels/Desktop/Soganatsu/SSWEProject/Bullet/bullet-2.81-rev2613/Extras/Serialize/BulletWorldImporter/btWorldImporter.cpp -o CMakeFiles/BulletWorldImporter.dir/btWorldImporter.s

Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/btWorldImporter.o.requires:
.PHONY : Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/btWorldImporter.o.requires

Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/btWorldImporter.o.provides: Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/btWorldImporter.o.requires
	$(MAKE) -f Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/build.make Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/btWorldImporter.o.provides.build
.PHONY : Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/btWorldImporter.o.provides

Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/btWorldImporter.o.provides.build: Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/btWorldImporter.o

# Object files for target BulletWorldImporter
BulletWorldImporter_OBJECTS = \
"CMakeFiles/BulletWorldImporter.dir/btBulletWorldImporter.o" \
"CMakeFiles/BulletWorldImporter.dir/btWorldImporter.o"

# External object files for target BulletWorldImporter
BulletWorldImporter_EXTERNAL_OBJECTS =

Extras/Serialize/BulletWorldImporter/libBulletWorldImporter.a: Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/btBulletWorldImporter.o
Extras/Serialize/BulletWorldImporter/libBulletWorldImporter.a: Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/btWorldImporter.o
Extras/Serialize/BulletWorldImporter/libBulletWorldImporter.a: Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/build.make
Extras/Serialize/BulletWorldImporter/libBulletWorldImporter.a: Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX static library libBulletWorldImporter.a"
	cd /home/parallels/Desktop/Soganatsu/SSWEProject/Bullet/bullet-2.81-rev2613/Extras/Serialize/BulletWorldImporter && $(CMAKE_COMMAND) -P CMakeFiles/BulletWorldImporter.dir/cmake_clean_target.cmake
	cd /home/parallels/Desktop/Soganatsu/SSWEProject/Bullet/bullet-2.81-rev2613/Extras/Serialize/BulletWorldImporter && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/BulletWorldImporter.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/build: Extras/Serialize/BulletWorldImporter/libBulletWorldImporter.a
.PHONY : Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/build

Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/requires: Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/btBulletWorldImporter.o.requires
Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/requires: Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/btWorldImporter.o.requires
.PHONY : Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/requires

Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/clean:
	cd /home/parallels/Desktop/Soganatsu/SSWEProject/Bullet/bullet-2.81-rev2613/Extras/Serialize/BulletWorldImporter && $(CMAKE_COMMAND) -P CMakeFiles/BulletWorldImporter.dir/cmake_clean.cmake
.PHONY : Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/clean

Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/depend:
	cd /home/parallels/Desktop/Soganatsu/SSWEProject/Bullet/bullet-2.81-rev2613 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/parallels/Desktop/Soganatsu/SSWEProject/Bullet/bullet-2.81-rev2613 /home/parallels/Desktop/Soganatsu/SSWEProject/Bullet/bullet-2.81-rev2613/Extras/Serialize/BulletWorldImporter /home/parallels/Desktop/Soganatsu/SSWEProject/Bullet/bullet-2.81-rev2613 /home/parallels/Desktop/Soganatsu/SSWEProject/Bullet/bullet-2.81-rev2613/Extras/Serialize/BulletWorldImporter /home/parallels/Desktop/Soganatsu/SSWEProject/Bullet/bullet-2.81-rev2613/Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/depend

