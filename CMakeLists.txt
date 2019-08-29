cmake_minimum_required(VERSION 2.8)

project(lane-detection-tfcc)

SET(LDFLAGS "-std=c++11 -msse4.1 -fPIC -O3 -ggdb -Wall -finline-functions -Wl,--allow-multiple-definition -Wl,--no-whole-archive")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${LDFLAGS}")

# Enable C++11
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

message(STATUS "CMAKE_CXX_COMPILER: ${CMAKE_CXX_COMPILER}")
message(STATUS "CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")

# Add Subdirectory
add_subdirectory(src)
