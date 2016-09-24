# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.0

# Default target executed when no arguments are given to make.
default_target: all
.PHONY : default_target

# Allow only one "make -f Makefile2" at a time, but pass parallelism.
.NOTPARALLEL:
.PHONY : .NOTPARALLEL

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
CMAKE_SOURCE_DIR = /home/faerytea/ghprojs/proxy_serv

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/faerytea/ghprojs/proxy_serv

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "No interactive CMake dialog available..."
	/usr/bin/cmake -E echo No\ interactive\ CMake\ dialog\ available.
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache
.PHONY : edit_cache/fast

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake to regenerate build system..."
	/usr/bin/cmake -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache
.PHONY : rebuild_cache/fast

# The main all target
all: cmake_check_build_system
	$(CMAKE_COMMAND) -E cmake_progress_start /home/faerytea/ghprojs/proxy_serv/CMakeFiles /home/faerytea/ghprojs/proxy_serv/CMakeFiles/progress.marks
	$(MAKE) -f CMakeFiles/Makefile2 all
	$(CMAKE_COMMAND) -E cmake_progress_start /home/faerytea/ghprojs/proxy_serv/CMakeFiles 0
.PHONY : all

# The main clean target
clean:
	$(MAKE) -f CMakeFiles/Makefile2 clean
.PHONY : clean

# The main clean target
clean/fast: clean
.PHONY : clean/fast

# Prepare targets for installation.
preinstall: all
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall

# Prepare targets for installation.
preinstall/fast:
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall/fast

# clear depends
depend:
	$(CMAKE_COMMAND) -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 1
.PHONY : depend

#=============================================================================
# Target rules for targets named proxy

# Build rule for target.
proxy: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 proxy
.PHONY : proxy

# fast build rule for target.
proxy/fast:
	$(MAKE) -f CMakeFiles/proxy.dir/build.make CMakeFiles/proxy.dir/build
.PHONY : proxy/fast

client.o: client.cpp.o
.PHONY : client.o

# target to build an object file
client.cpp.o:
	$(MAKE) -f CMakeFiles/proxy.dir/build.make CMakeFiles/proxy.dir/client.cpp.o
.PHONY : client.cpp.o

client.i: client.cpp.i
.PHONY : client.i

# target to preprocess a source file
client.cpp.i:
	$(MAKE) -f CMakeFiles/proxy.dir/build.make CMakeFiles/proxy.dir/client.cpp.i
.PHONY : client.cpp.i

client.s: client.cpp.s
.PHONY : client.s

# target to generate assembly for a file
client.cpp.s:
	$(MAKE) -f CMakeFiles/proxy.dir/build.make CMakeFiles/proxy.dir/client.cpp.s
.PHONY : client.cpp.s

epoll_io.o: epoll_io.cpp.o
.PHONY : epoll_io.o

# target to build an object file
epoll_io.cpp.o:
	$(MAKE) -f CMakeFiles/proxy.dir/build.make CMakeFiles/proxy.dir/epoll_io.cpp.o
.PHONY : epoll_io.cpp.o

epoll_io.i: epoll_io.cpp.i
.PHONY : epoll_io.i

# target to preprocess a source file
epoll_io.cpp.i:
	$(MAKE) -f CMakeFiles/proxy.dir/build.make CMakeFiles/proxy.dir/epoll_io.cpp.i
.PHONY : epoll_io.cpp.i

epoll_io.s: epoll_io.cpp.s
.PHONY : epoll_io.s

# target to generate assembly for a file
epoll_io.cpp.s:
	$(MAKE) -f CMakeFiles/proxy.dir/build.make CMakeFiles/proxy.dir/epoll_io.cpp.s
.PHONY : epoll_io.cpp.s

file_descriptor.o: file_descriptor.cpp.o
.PHONY : file_descriptor.o

# target to build an object file
file_descriptor.cpp.o:
	$(MAKE) -f CMakeFiles/proxy.dir/build.make CMakeFiles/proxy.dir/file_descriptor.cpp.o
.PHONY : file_descriptor.cpp.o

file_descriptor.i: file_descriptor.cpp.i
.PHONY : file_descriptor.i

# target to preprocess a source file
file_descriptor.cpp.i:
	$(MAKE) -f CMakeFiles/proxy.dir/build.make CMakeFiles/proxy.dir/file_descriptor.cpp.i
.PHONY : file_descriptor.cpp.i

file_descriptor.s: file_descriptor.cpp.s
.PHONY : file_descriptor.s

# target to generate assembly for a file
file_descriptor.cpp.s:
	$(MAKE) -f CMakeFiles/proxy.dir/build.make CMakeFiles/proxy.dir/file_descriptor.cpp.s
.PHONY : file_descriptor.cpp.s

http.o: http.cpp.o
.PHONY : http.o

# target to build an object file
http.cpp.o:
	$(MAKE) -f CMakeFiles/proxy.dir/build.make CMakeFiles/proxy.dir/http.cpp.o
.PHONY : http.cpp.o

http.i: http.cpp.i
.PHONY : http.i

# target to preprocess a source file
http.cpp.i:
	$(MAKE) -f CMakeFiles/proxy.dir/build.make CMakeFiles/proxy.dir/http.cpp.i
.PHONY : http.cpp.i

http.s: http.cpp.s
.PHONY : http.s

# target to generate assembly for a file
http.cpp.s:
	$(MAKE) -f CMakeFiles/proxy.dir/build.make CMakeFiles/proxy.dir/http.cpp.s
.PHONY : http.cpp.s

io_event.o: io_event.cpp.o
.PHONY : io_event.o

# target to build an object file
io_event.cpp.o:
	$(MAKE) -f CMakeFiles/proxy.dir/build.make CMakeFiles/proxy.dir/io_event.cpp.o
.PHONY : io_event.cpp.o

io_event.i: io_event.cpp.i
.PHONY : io_event.i

# target to preprocess a source file
io_event.cpp.i:
	$(MAKE) -f CMakeFiles/proxy.dir/build.make CMakeFiles/proxy.dir/io_event.cpp.i
.PHONY : io_event.cpp.i

io_event.s: io_event.cpp.s
.PHONY : io_event.s

# target to generate assembly for a file
io_event.cpp.s:
	$(MAKE) -f CMakeFiles/proxy.dir/build.make CMakeFiles/proxy.dir/io_event.cpp.s
.PHONY : io_event.cpp.s

linux_socket.o: linux_socket.cpp.o
.PHONY : linux_socket.o

# target to build an object file
linux_socket.cpp.o:
	$(MAKE) -f CMakeFiles/proxy.dir/build.make CMakeFiles/proxy.dir/linux_socket.cpp.o
.PHONY : linux_socket.cpp.o

linux_socket.i: linux_socket.cpp.i
.PHONY : linux_socket.i

# target to preprocess a source file
linux_socket.cpp.i:
	$(MAKE) -f CMakeFiles/proxy.dir/build.make CMakeFiles/proxy.dir/linux_socket.cpp.i
.PHONY : linux_socket.cpp.i

linux_socket.s: linux_socket.cpp.s
.PHONY : linux_socket.s

# target to generate assembly for a file
linux_socket.cpp.s:
	$(MAKE) -f CMakeFiles/proxy.dir/build.make CMakeFiles/proxy.dir/linux_socket.cpp.s
.PHONY : linux_socket.cpp.s

main.o: main.cpp.o
.PHONY : main.o

# target to build an object file
main.cpp.o:
	$(MAKE) -f CMakeFiles/proxy.dir/build.make CMakeFiles/proxy.dir/main.cpp.o
.PHONY : main.cpp.o

main.i: main.cpp.i
.PHONY : main.i

# target to preprocess a source file
main.cpp.i:
	$(MAKE) -f CMakeFiles/proxy.dir/build.make CMakeFiles/proxy.dir/main.cpp.i
.PHONY : main.cpp.i

main.s: main.cpp.s
.PHONY : main.s

# target to generate assembly for a file
main.cpp.s:
	$(MAKE) -f CMakeFiles/proxy.dir/build.make CMakeFiles/proxy.dir/main.cpp.s
.PHONY : main.cpp.s

proxy_server.o: proxy_server.cpp.o
.PHONY : proxy_server.o

# target to build an object file
proxy_server.cpp.o:
	$(MAKE) -f CMakeFiles/proxy.dir/build.make CMakeFiles/proxy.dir/proxy_server.cpp.o
.PHONY : proxy_server.cpp.o

proxy_server.i: proxy_server.cpp.i
.PHONY : proxy_server.i

# target to preprocess a source file
proxy_server.cpp.i:
	$(MAKE) -f CMakeFiles/proxy.dir/build.make CMakeFiles/proxy.dir/proxy_server.cpp.i
.PHONY : proxy_server.cpp.i

proxy_server.s: proxy_server.cpp.s
.PHONY : proxy_server.s

# target to generate assembly for a file
proxy_server.cpp.s:
	$(MAKE) -f CMakeFiles/proxy.dir/build.make CMakeFiles/proxy.dir/proxy_server.cpp.s
.PHONY : proxy_server.cpp.s

resolver.o: resolver.cpp.o
.PHONY : resolver.o

# target to build an object file
resolver.cpp.o:
	$(MAKE) -f CMakeFiles/proxy.dir/build.make CMakeFiles/proxy.dir/resolver.cpp.o
.PHONY : resolver.cpp.o

resolver.i: resolver.cpp.i
.PHONY : resolver.i

# target to preprocess a source file
resolver.cpp.i:
	$(MAKE) -f CMakeFiles/proxy.dir/build.make CMakeFiles/proxy.dir/resolver.cpp.i
.PHONY : resolver.cpp.i

resolver.s: resolver.cpp.s
.PHONY : resolver.s

# target to generate assembly for a file
resolver.cpp.s:
	$(MAKE) -f CMakeFiles/proxy.dir/build.make CMakeFiles/proxy.dir/resolver.cpp.s
.PHONY : resolver.cpp.s

server.o: server.cpp.o
.PHONY : server.o

# target to build an object file
server.cpp.o:
	$(MAKE) -f CMakeFiles/proxy.dir/build.make CMakeFiles/proxy.dir/server.cpp.o
.PHONY : server.cpp.o

server.i: server.cpp.i
.PHONY : server.i

# target to preprocess a source file
server.cpp.i:
	$(MAKE) -f CMakeFiles/proxy.dir/build.make CMakeFiles/proxy.dir/server.cpp.i
.PHONY : server.cpp.i

server.s: server.cpp.s
.PHONY : server.s

# target to generate assembly for a file
server.cpp.s:
	$(MAKE) -f CMakeFiles/proxy.dir/build.make CMakeFiles/proxy.dir/server.cpp.s
.PHONY : server.cpp.s

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... edit_cache"
	@echo "... proxy"
	@echo "... rebuild_cache"
	@echo "... client.o"
	@echo "... client.i"
	@echo "... client.s"
	@echo "... epoll_io.o"
	@echo "... epoll_io.i"
	@echo "... epoll_io.s"
	@echo "... file_descriptor.o"
	@echo "... file_descriptor.i"
	@echo "... file_descriptor.s"
	@echo "... http.o"
	@echo "... http.i"
	@echo "... http.s"
	@echo "... io_event.o"
	@echo "... io_event.i"
	@echo "... io_event.s"
	@echo "... linux_socket.o"
	@echo "... linux_socket.i"
	@echo "... linux_socket.s"
	@echo "... main.o"
	@echo "... main.i"
	@echo "... main.s"
	@echo "... proxy_server.o"
	@echo "... proxy_server.i"
	@echo "... proxy_server.s"
	@echo "... resolver.o"
	@echo "... resolver.i"
	@echo "... resolver.s"
	@echo "... server.o"
	@echo "... server.i"
	@echo "... server.s"
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	$(CMAKE_COMMAND) -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 0
.PHONY : cmake_check_build_system

