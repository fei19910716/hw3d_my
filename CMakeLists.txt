cmake_minimum_required(VERSION 3.16.5)
project(hw3d_my)

set(target hw3d_my)
set(CMAKE_CXX_STANDARD 17)

set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/assets/shaders/VertexShader.hlsl PROPERTIES VS_SHADER_FLAGS "/Fh \"$(OutDir)%(Filename).cso\"" VS_SHADER_TYPE Vertex VS_SHADER_MODEL 5.0 VS_SHADER_ENTRYPOINT main)


file(GLOB_RECURSE src_files ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

add_executable(${target} WIN32 ${src_files})
target_include_directories(${target} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
