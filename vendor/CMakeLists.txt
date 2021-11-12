add_library(libvendor INTERFACE)

set(vendor_libs
    imgui
    assimp
)

foreach(vendor_lib ${vendor_libs})
    add_subdirectory(${vendor_lib})
    target_link_libraries(libvendor INTERFACE ${vendor_lib})
endforeach(vendor_lib ${})


target_include_directories(libvendor INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

