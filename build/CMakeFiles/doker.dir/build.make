# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.5

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
CMAKE_SOURCE_DIR = /home/wuxd/ClionProjects/doker

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/wuxd/ClionProjects/doker/build

# Include any dependencies generated for this target.
include CMakeFiles/doker.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/doker.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/doker.dir/flags.make

CMakeFiles/doker.dir/main.cpp.o: CMakeFiles/doker.dir/flags.make
CMakeFiles/doker.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/wuxd/ClionProjects/doker/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/doker.dir/main.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/doker.dir/main.cpp.o -c /home/wuxd/ClionProjects/doker/main.cpp

CMakeFiles/doker.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/doker.dir/main.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/wuxd/ClionProjects/doker/main.cpp > CMakeFiles/doker.dir/main.cpp.i

CMakeFiles/doker.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/doker.dir/main.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/wuxd/ClionProjects/doker/main.cpp -o CMakeFiles/doker.dir/main.cpp.s

CMakeFiles/doker.dir/main.cpp.o.requires:

.PHONY : CMakeFiles/doker.dir/main.cpp.o.requires

CMakeFiles/doker.dir/main.cpp.o.provides: CMakeFiles/doker.dir/main.cpp.o.requires
	$(MAKE) -f CMakeFiles/doker.dir/build.make CMakeFiles/doker.dir/main.cpp.o.provides.build
.PHONY : CMakeFiles/doker.dir/main.cpp.o.provides

CMakeFiles/doker.dir/main.cpp.o.provides.build: CMakeFiles/doker.dir/main.cpp.o


CMakeFiles/doker.dir/base/Thread.cpp.o: CMakeFiles/doker.dir/flags.make
CMakeFiles/doker.dir/base/Thread.cpp.o: ../base/Thread.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/wuxd/ClionProjects/doker/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/doker.dir/base/Thread.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/doker.dir/base/Thread.cpp.o -c /home/wuxd/ClionProjects/doker/base/Thread.cpp

CMakeFiles/doker.dir/base/Thread.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/doker.dir/base/Thread.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/wuxd/ClionProjects/doker/base/Thread.cpp > CMakeFiles/doker.dir/base/Thread.cpp.i

CMakeFiles/doker.dir/base/Thread.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/doker.dir/base/Thread.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/wuxd/ClionProjects/doker/base/Thread.cpp -o CMakeFiles/doker.dir/base/Thread.cpp.s

CMakeFiles/doker.dir/base/Thread.cpp.o.requires:

.PHONY : CMakeFiles/doker.dir/base/Thread.cpp.o.requires

CMakeFiles/doker.dir/base/Thread.cpp.o.provides: CMakeFiles/doker.dir/base/Thread.cpp.o.requires
	$(MAKE) -f CMakeFiles/doker.dir/build.make CMakeFiles/doker.dir/base/Thread.cpp.o.provides.build
.PHONY : CMakeFiles/doker.dir/base/Thread.cpp.o.provides

CMakeFiles/doker.dir/base/Thread.cpp.o.provides.build: CMakeFiles/doker.dir/base/Thread.cpp.o


CMakeFiles/doker.dir/base/TimeUnit.cpp.o: CMakeFiles/doker.dir/flags.make
CMakeFiles/doker.dir/base/TimeUnit.cpp.o: ../base/TimeUnit.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/wuxd/ClionProjects/doker/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/doker.dir/base/TimeUnit.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/doker.dir/base/TimeUnit.cpp.o -c /home/wuxd/ClionProjects/doker/base/TimeUnit.cpp

CMakeFiles/doker.dir/base/TimeUnit.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/doker.dir/base/TimeUnit.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/wuxd/ClionProjects/doker/base/TimeUnit.cpp > CMakeFiles/doker.dir/base/TimeUnit.cpp.i

CMakeFiles/doker.dir/base/TimeUnit.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/doker.dir/base/TimeUnit.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/wuxd/ClionProjects/doker/base/TimeUnit.cpp -o CMakeFiles/doker.dir/base/TimeUnit.cpp.s

CMakeFiles/doker.dir/base/TimeUnit.cpp.o.requires:

.PHONY : CMakeFiles/doker.dir/base/TimeUnit.cpp.o.requires

CMakeFiles/doker.dir/base/TimeUnit.cpp.o.provides: CMakeFiles/doker.dir/base/TimeUnit.cpp.o.requires
	$(MAKE) -f CMakeFiles/doker.dir/build.make CMakeFiles/doker.dir/base/TimeUnit.cpp.o.provides.build
.PHONY : CMakeFiles/doker.dir/base/TimeUnit.cpp.o.provides

CMakeFiles/doker.dir/base/TimeUnit.cpp.o.provides.build: CMakeFiles/doker.dir/base/TimeUnit.cpp.o


CMakeFiles/doker.dir/EventLoop.cpp.o: CMakeFiles/doker.dir/flags.make
CMakeFiles/doker.dir/EventLoop.cpp.o: ../EventLoop.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/wuxd/ClionProjects/doker/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/doker.dir/EventLoop.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/doker.dir/EventLoop.cpp.o -c /home/wuxd/ClionProjects/doker/EventLoop.cpp

CMakeFiles/doker.dir/EventLoop.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/doker.dir/EventLoop.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/wuxd/ClionProjects/doker/EventLoop.cpp > CMakeFiles/doker.dir/EventLoop.cpp.i

CMakeFiles/doker.dir/EventLoop.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/doker.dir/EventLoop.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/wuxd/ClionProjects/doker/EventLoop.cpp -o CMakeFiles/doker.dir/EventLoop.cpp.s

CMakeFiles/doker.dir/EventLoop.cpp.o.requires:

.PHONY : CMakeFiles/doker.dir/EventLoop.cpp.o.requires

CMakeFiles/doker.dir/EventLoop.cpp.o.provides: CMakeFiles/doker.dir/EventLoop.cpp.o.requires
	$(MAKE) -f CMakeFiles/doker.dir/build.make CMakeFiles/doker.dir/EventLoop.cpp.o.provides.build
.PHONY : CMakeFiles/doker.dir/EventLoop.cpp.o.provides

CMakeFiles/doker.dir/EventLoop.cpp.o.provides.build: CMakeFiles/doker.dir/EventLoop.cpp.o


CMakeFiles/doker.dir/Channel.cpp.o: CMakeFiles/doker.dir/flags.make
CMakeFiles/doker.dir/Channel.cpp.o: ../Channel.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/wuxd/ClionProjects/doker/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/doker.dir/Channel.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/doker.dir/Channel.cpp.o -c /home/wuxd/ClionProjects/doker/Channel.cpp

CMakeFiles/doker.dir/Channel.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/doker.dir/Channel.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/wuxd/ClionProjects/doker/Channel.cpp > CMakeFiles/doker.dir/Channel.cpp.i

CMakeFiles/doker.dir/Channel.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/doker.dir/Channel.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/wuxd/ClionProjects/doker/Channel.cpp -o CMakeFiles/doker.dir/Channel.cpp.s

CMakeFiles/doker.dir/Channel.cpp.o.requires:

.PHONY : CMakeFiles/doker.dir/Channel.cpp.o.requires

CMakeFiles/doker.dir/Channel.cpp.o.provides: CMakeFiles/doker.dir/Channel.cpp.o.requires
	$(MAKE) -f CMakeFiles/doker.dir/build.make CMakeFiles/doker.dir/Channel.cpp.o.provides.build
.PHONY : CMakeFiles/doker.dir/Channel.cpp.o.provides

CMakeFiles/doker.dir/Channel.cpp.o.provides.build: CMakeFiles/doker.dir/Channel.cpp.o


CMakeFiles/doker.dir/Poller.cpp.o: CMakeFiles/doker.dir/flags.make
CMakeFiles/doker.dir/Poller.cpp.o: ../Poller.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/wuxd/ClionProjects/doker/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/doker.dir/Poller.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/doker.dir/Poller.cpp.o -c /home/wuxd/ClionProjects/doker/Poller.cpp

CMakeFiles/doker.dir/Poller.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/doker.dir/Poller.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/wuxd/ClionProjects/doker/Poller.cpp > CMakeFiles/doker.dir/Poller.cpp.i

CMakeFiles/doker.dir/Poller.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/doker.dir/Poller.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/wuxd/ClionProjects/doker/Poller.cpp -o CMakeFiles/doker.dir/Poller.cpp.s

CMakeFiles/doker.dir/Poller.cpp.o.requires:

.PHONY : CMakeFiles/doker.dir/Poller.cpp.o.requires

CMakeFiles/doker.dir/Poller.cpp.o.provides: CMakeFiles/doker.dir/Poller.cpp.o.requires
	$(MAKE) -f CMakeFiles/doker.dir/build.make CMakeFiles/doker.dir/Poller.cpp.o.provides.build
.PHONY : CMakeFiles/doker.dir/Poller.cpp.o.provides

CMakeFiles/doker.dir/Poller.cpp.o.provides.build: CMakeFiles/doker.dir/Poller.cpp.o


CMakeFiles/doker.dir/TimerQueue.cpp.o: CMakeFiles/doker.dir/flags.make
CMakeFiles/doker.dir/TimerQueue.cpp.o: ../TimerQueue.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/wuxd/ClionProjects/doker/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/doker.dir/TimerQueue.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/doker.dir/TimerQueue.cpp.o -c /home/wuxd/ClionProjects/doker/TimerQueue.cpp

CMakeFiles/doker.dir/TimerQueue.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/doker.dir/TimerQueue.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/wuxd/ClionProjects/doker/TimerQueue.cpp > CMakeFiles/doker.dir/TimerQueue.cpp.i

CMakeFiles/doker.dir/TimerQueue.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/doker.dir/TimerQueue.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/wuxd/ClionProjects/doker/TimerQueue.cpp -o CMakeFiles/doker.dir/TimerQueue.cpp.s

CMakeFiles/doker.dir/TimerQueue.cpp.o.requires:

.PHONY : CMakeFiles/doker.dir/TimerQueue.cpp.o.requires

CMakeFiles/doker.dir/TimerQueue.cpp.o.provides: CMakeFiles/doker.dir/TimerQueue.cpp.o.requires
	$(MAKE) -f CMakeFiles/doker.dir/build.make CMakeFiles/doker.dir/TimerQueue.cpp.o.provides.build
.PHONY : CMakeFiles/doker.dir/TimerQueue.cpp.o.provides

CMakeFiles/doker.dir/TimerQueue.cpp.o.provides.build: CMakeFiles/doker.dir/TimerQueue.cpp.o


CMakeFiles/doker.dir/EventLoopThread.cpp.o: CMakeFiles/doker.dir/flags.make
CMakeFiles/doker.dir/EventLoopThread.cpp.o: ../EventLoopThread.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/wuxd/ClionProjects/doker/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/doker.dir/EventLoopThread.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/doker.dir/EventLoopThread.cpp.o -c /home/wuxd/ClionProjects/doker/EventLoopThread.cpp

CMakeFiles/doker.dir/EventLoopThread.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/doker.dir/EventLoopThread.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/wuxd/ClionProjects/doker/EventLoopThread.cpp > CMakeFiles/doker.dir/EventLoopThread.cpp.i

CMakeFiles/doker.dir/EventLoopThread.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/doker.dir/EventLoopThread.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/wuxd/ClionProjects/doker/EventLoopThread.cpp -o CMakeFiles/doker.dir/EventLoopThread.cpp.s

CMakeFiles/doker.dir/EventLoopThread.cpp.o.requires:

.PHONY : CMakeFiles/doker.dir/EventLoopThread.cpp.o.requires

CMakeFiles/doker.dir/EventLoopThread.cpp.o.provides: CMakeFiles/doker.dir/EventLoopThread.cpp.o.requires
	$(MAKE) -f CMakeFiles/doker.dir/build.make CMakeFiles/doker.dir/EventLoopThread.cpp.o.provides.build
.PHONY : CMakeFiles/doker.dir/EventLoopThread.cpp.o.provides

CMakeFiles/doker.dir/EventLoopThread.cpp.o.provides.build: CMakeFiles/doker.dir/EventLoopThread.cpp.o


CMakeFiles/doker.dir/SocketHand.cpp.o: CMakeFiles/doker.dir/flags.make
CMakeFiles/doker.dir/SocketHand.cpp.o: ../SocketHand.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/wuxd/ClionProjects/doker/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object CMakeFiles/doker.dir/SocketHand.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/doker.dir/SocketHand.cpp.o -c /home/wuxd/ClionProjects/doker/SocketHand.cpp

CMakeFiles/doker.dir/SocketHand.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/doker.dir/SocketHand.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/wuxd/ClionProjects/doker/SocketHand.cpp > CMakeFiles/doker.dir/SocketHand.cpp.i

CMakeFiles/doker.dir/SocketHand.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/doker.dir/SocketHand.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/wuxd/ClionProjects/doker/SocketHand.cpp -o CMakeFiles/doker.dir/SocketHand.cpp.s

CMakeFiles/doker.dir/SocketHand.cpp.o.requires:

.PHONY : CMakeFiles/doker.dir/SocketHand.cpp.o.requires

CMakeFiles/doker.dir/SocketHand.cpp.o.provides: CMakeFiles/doker.dir/SocketHand.cpp.o.requires
	$(MAKE) -f CMakeFiles/doker.dir/build.make CMakeFiles/doker.dir/SocketHand.cpp.o.provides.build
.PHONY : CMakeFiles/doker.dir/SocketHand.cpp.o.provides

CMakeFiles/doker.dir/SocketHand.cpp.o.provides.build: CMakeFiles/doker.dir/SocketHand.cpp.o


CMakeFiles/doker.dir/Socket.cpp.o: CMakeFiles/doker.dir/flags.make
CMakeFiles/doker.dir/Socket.cpp.o: ../Socket.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/wuxd/ClionProjects/doker/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building CXX object CMakeFiles/doker.dir/Socket.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/doker.dir/Socket.cpp.o -c /home/wuxd/ClionProjects/doker/Socket.cpp

CMakeFiles/doker.dir/Socket.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/doker.dir/Socket.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/wuxd/ClionProjects/doker/Socket.cpp > CMakeFiles/doker.dir/Socket.cpp.i

CMakeFiles/doker.dir/Socket.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/doker.dir/Socket.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/wuxd/ClionProjects/doker/Socket.cpp -o CMakeFiles/doker.dir/Socket.cpp.s

CMakeFiles/doker.dir/Socket.cpp.o.requires:

.PHONY : CMakeFiles/doker.dir/Socket.cpp.o.requires

CMakeFiles/doker.dir/Socket.cpp.o.provides: CMakeFiles/doker.dir/Socket.cpp.o.requires
	$(MAKE) -f CMakeFiles/doker.dir/build.make CMakeFiles/doker.dir/Socket.cpp.o.provides.build
.PHONY : CMakeFiles/doker.dir/Socket.cpp.o.provides

CMakeFiles/doker.dir/Socket.cpp.o.provides.build: CMakeFiles/doker.dir/Socket.cpp.o


CMakeFiles/doker.dir/InetAddr.cpp.o: CMakeFiles/doker.dir/flags.make
CMakeFiles/doker.dir/InetAddr.cpp.o: ../InetAddr.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/wuxd/ClionProjects/doker/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Building CXX object CMakeFiles/doker.dir/InetAddr.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/doker.dir/InetAddr.cpp.o -c /home/wuxd/ClionProjects/doker/InetAddr.cpp

CMakeFiles/doker.dir/InetAddr.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/doker.dir/InetAddr.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/wuxd/ClionProjects/doker/InetAddr.cpp > CMakeFiles/doker.dir/InetAddr.cpp.i

CMakeFiles/doker.dir/InetAddr.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/doker.dir/InetAddr.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/wuxd/ClionProjects/doker/InetAddr.cpp -o CMakeFiles/doker.dir/InetAddr.cpp.s

CMakeFiles/doker.dir/InetAddr.cpp.o.requires:

.PHONY : CMakeFiles/doker.dir/InetAddr.cpp.o.requires

CMakeFiles/doker.dir/InetAddr.cpp.o.provides: CMakeFiles/doker.dir/InetAddr.cpp.o.requires
	$(MAKE) -f CMakeFiles/doker.dir/build.make CMakeFiles/doker.dir/InetAddr.cpp.o.provides.build
.PHONY : CMakeFiles/doker.dir/InetAddr.cpp.o.provides

CMakeFiles/doker.dir/InetAddr.cpp.o.provides.build: CMakeFiles/doker.dir/InetAddr.cpp.o


CMakeFiles/doker.dir/Acceptor.cpp.o: CMakeFiles/doker.dir/flags.make
CMakeFiles/doker.dir/Acceptor.cpp.o: ../Acceptor.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/wuxd/ClionProjects/doker/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_12) "Building CXX object CMakeFiles/doker.dir/Acceptor.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/doker.dir/Acceptor.cpp.o -c /home/wuxd/ClionProjects/doker/Acceptor.cpp

CMakeFiles/doker.dir/Acceptor.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/doker.dir/Acceptor.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/wuxd/ClionProjects/doker/Acceptor.cpp > CMakeFiles/doker.dir/Acceptor.cpp.i

CMakeFiles/doker.dir/Acceptor.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/doker.dir/Acceptor.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/wuxd/ClionProjects/doker/Acceptor.cpp -o CMakeFiles/doker.dir/Acceptor.cpp.s

CMakeFiles/doker.dir/Acceptor.cpp.o.requires:

.PHONY : CMakeFiles/doker.dir/Acceptor.cpp.o.requires

CMakeFiles/doker.dir/Acceptor.cpp.o.provides: CMakeFiles/doker.dir/Acceptor.cpp.o.requires
	$(MAKE) -f CMakeFiles/doker.dir/build.make CMakeFiles/doker.dir/Acceptor.cpp.o.provides.build
.PHONY : CMakeFiles/doker.dir/Acceptor.cpp.o.provides

CMakeFiles/doker.dir/Acceptor.cpp.o.provides.build: CMakeFiles/doker.dir/Acceptor.cpp.o


# Object files for target doker
doker_OBJECTS = \
"CMakeFiles/doker.dir/main.cpp.o" \
"CMakeFiles/doker.dir/base/Thread.cpp.o" \
"CMakeFiles/doker.dir/base/TimeUnit.cpp.o" \
"CMakeFiles/doker.dir/EventLoop.cpp.o" \
"CMakeFiles/doker.dir/Channel.cpp.o" \
"CMakeFiles/doker.dir/Poller.cpp.o" \
"CMakeFiles/doker.dir/TimerQueue.cpp.o" \
"CMakeFiles/doker.dir/EventLoopThread.cpp.o" \
"CMakeFiles/doker.dir/SocketHand.cpp.o" \
"CMakeFiles/doker.dir/Socket.cpp.o" \
"CMakeFiles/doker.dir/InetAddr.cpp.o" \
"CMakeFiles/doker.dir/Acceptor.cpp.o"

# External object files for target doker
doker_EXTERNAL_OBJECTS =

doker: CMakeFiles/doker.dir/main.cpp.o
doker: CMakeFiles/doker.dir/base/Thread.cpp.o
doker: CMakeFiles/doker.dir/base/TimeUnit.cpp.o
doker: CMakeFiles/doker.dir/EventLoop.cpp.o
doker: CMakeFiles/doker.dir/Channel.cpp.o
doker: CMakeFiles/doker.dir/Poller.cpp.o
doker: CMakeFiles/doker.dir/TimerQueue.cpp.o
doker: CMakeFiles/doker.dir/EventLoopThread.cpp.o
doker: CMakeFiles/doker.dir/SocketHand.cpp.o
doker: CMakeFiles/doker.dir/Socket.cpp.o
doker: CMakeFiles/doker.dir/InetAddr.cpp.o
doker: CMakeFiles/doker.dir/Acceptor.cpp.o
doker: CMakeFiles/doker.dir/build.make
doker: CMakeFiles/doker.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/wuxd/ClionProjects/doker/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_13) "Linking CXX executable doker"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/doker.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/doker.dir/build: doker

.PHONY : CMakeFiles/doker.dir/build

CMakeFiles/doker.dir/requires: CMakeFiles/doker.dir/main.cpp.o.requires
CMakeFiles/doker.dir/requires: CMakeFiles/doker.dir/base/Thread.cpp.o.requires
CMakeFiles/doker.dir/requires: CMakeFiles/doker.dir/base/TimeUnit.cpp.o.requires
CMakeFiles/doker.dir/requires: CMakeFiles/doker.dir/EventLoop.cpp.o.requires
CMakeFiles/doker.dir/requires: CMakeFiles/doker.dir/Channel.cpp.o.requires
CMakeFiles/doker.dir/requires: CMakeFiles/doker.dir/Poller.cpp.o.requires
CMakeFiles/doker.dir/requires: CMakeFiles/doker.dir/TimerQueue.cpp.o.requires
CMakeFiles/doker.dir/requires: CMakeFiles/doker.dir/EventLoopThread.cpp.o.requires
CMakeFiles/doker.dir/requires: CMakeFiles/doker.dir/SocketHand.cpp.o.requires
CMakeFiles/doker.dir/requires: CMakeFiles/doker.dir/Socket.cpp.o.requires
CMakeFiles/doker.dir/requires: CMakeFiles/doker.dir/InetAddr.cpp.o.requires
CMakeFiles/doker.dir/requires: CMakeFiles/doker.dir/Acceptor.cpp.o.requires

.PHONY : CMakeFiles/doker.dir/requires

CMakeFiles/doker.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/doker.dir/cmake_clean.cmake
.PHONY : CMakeFiles/doker.dir/clean

CMakeFiles/doker.dir/depend:
	cd /home/wuxd/ClionProjects/doker/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/wuxd/ClionProjects/doker /home/wuxd/ClionProjects/doker /home/wuxd/ClionProjects/doker/build /home/wuxd/ClionProjects/doker/build /home/wuxd/ClionProjects/doker/build/CMakeFiles/doker.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/doker.dir/depend

