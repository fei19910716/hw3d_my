
add_library(assimp INTERFACE)

file(GLOB lib_files ${CMAKE_CURRENT_SOURCE_DIR}/lib/*.lib)

target_link_libraries(assimp INTERFACE ${lib_files})

target_include_directories(assimp INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)