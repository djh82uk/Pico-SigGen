# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/pi/Pico_SigGen

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/pi/Pico_SigGen/build

# Include any dependencies generated for this target.
include pico-sdk/src/rp2_common/boot_stage2/CMakeFiles/bs2_default.dir/depend.make

# Include the progress variables for this target.
include pico-sdk/src/rp2_common/boot_stage2/CMakeFiles/bs2_default.dir/progress.make

# Include the compile flags for this target's objects.
include pico-sdk/src/rp2_common/boot_stage2/CMakeFiles/bs2_default.dir/flags.make

pico-sdk/src/rp2_common/boot_stage2/CMakeFiles/bs2_default.dir/compile_time_choice.S.obj: pico-sdk/src/rp2_common/boot_stage2/CMakeFiles/bs2_default.dir/flags.make
pico-sdk/src/rp2_common/boot_stage2/CMakeFiles/bs2_default.dir/compile_time_choice.S.obj: /home/pi/pico/pico-sdk/src/rp2_common/boot_stage2/compile_time_choice.S
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pi/Pico_SigGen/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building ASM object pico-sdk/src/rp2_common/boot_stage2/CMakeFiles/bs2_default.dir/compile_time_choice.S.obj"
	cd /home/pi/Pico_SigGen/build/pico-sdk/src/rp2_common/boot_stage2 && /usr/bin/arm-none-eabi-gcc $(ASM_DEFINES) $(ASM_INCLUDES) $(ASM_FLAGS) -o CMakeFiles/bs2_default.dir/compile_time_choice.S.obj   -c /home/pi/pico/pico-sdk/src/rp2_common/boot_stage2/compile_time_choice.S

# Object files for target bs2_default
bs2_default_OBJECTS = \
"CMakeFiles/bs2_default.dir/compile_time_choice.S.obj"

# External object files for target bs2_default
bs2_default_EXTERNAL_OBJECTS =

pico-sdk/src/rp2_common/boot_stage2/bs2_default.elf: pico-sdk/src/rp2_common/boot_stage2/CMakeFiles/bs2_default.dir/compile_time_choice.S.obj
pico-sdk/src/rp2_common/boot_stage2/bs2_default.elf: pico-sdk/src/rp2_common/boot_stage2/CMakeFiles/bs2_default.dir/build.make
pico-sdk/src/rp2_common/boot_stage2/bs2_default.elf: /home/pi/pico/pico-sdk/src/rp2_common/boot_stage2/boot_stage2.ld
pico-sdk/src/rp2_common/boot_stage2/bs2_default.elf: pico-sdk/src/rp2_common/boot_stage2/CMakeFiles/bs2_default.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/pi/Pico_SigGen/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking ASM executable bs2_default.elf"
	cd /home/pi/Pico_SigGen/build/pico-sdk/src/rp2_common/boot_stage2 && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/bs2_default.dir/link.txt --verbose=$(VERBOSE)
	cd /home/pi/Pico_SigGen/build/pico-sdk/src/rp2_common/boot_stage2 && /usr/bin/arm-none-eabi-objdump -h bs2_default.elf >bs2_default.dis
	cd /home/pi/Pico_SigGen/build/pico-sdk/src/rp2_common/boot_stage2 && /usr/bin/arm-none-eabi-objdump -d bs2_default.elf >>bs2_default.dis

# Rule to build all files generated by this target.
pico-sdk/src/rp2_common/boot_stage2/CMakeFiles/bs2_default.dir/build: pico-sdk/src/rp2_common/boot_stage2/bs2_default.elf

.PHONY : pico-sdk/src/rp2_common/boot_stage2/CMakeFiles/bs2_default.dir/build

pico-sdk/src/rp2_common/boot_stage2/CMakeFiles/bs2_default.dir/clean:
	cd /home/pi/Pico_SigGen/build/pico-sdk/src/rp2_common/boot_stage2 && $(CMAKE_COMMAND) -P CMakeFiles/bs2_default.dir/cmake_clean.cmake
.PHONY : pico-sdk/src/rp2_common/boot_stage2/CMakeFiles/bs2_default.dir/clean

pico-sdk/src/rp2_common/boot_stage2/CMakeFiles/bs2_default.dir/depend:
	cd /home/pi/Pico_SigGen/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/pi/Pico_SigGen /home/pi/pico/pico-sdk/src/rp2_common/boot_stage2 /home/pi/Pico_SigGen/build /home/pi/Pico_SigGen/build/pico-sdk/src/rp2_common/boot_stage2 /home/pi/Pico_SigGen/build/pico-sdk/src/rp2_common/boot_stage2/CMakeFiles/bs2_default.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : pico-sdk/src/rp2_common/boot_stage2/CMakeFiles/bs2_default.dir/depend

