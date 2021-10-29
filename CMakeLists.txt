cmake_minimum_required(VERSION 3.16.5)
project(hw3d_my)

set(target hw3d_my)



file(GLOB_RECURSE src_files ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

add_executable(${target} WIN32 ${src_files})
