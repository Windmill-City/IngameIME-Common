cmake_minimum_required(VERSION 3.4)
project(IngameIME-Test)

# ####################################
# Compile
# ####################################
set(CMAKE_CXX_STANDARD 20)

# Use Static Runtime Library
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

# ####################################
# Dependencies
# ####################################
add_subdirectory(glad EXCLUDE_FROM_ALL)
add_subdirectory(glfw EXCLUDE_FROM_ALL)

# IngameIME
add_subdirectory(../../ build)

# Library Target - ImGUI
aux_source_directory(imgui IMGUI_SRC_LIST)
add_library(imgui STATIC ${IMGUI_SRC_LIST} imgui/backends/imgui_impl_glfw.cpp imgui/backends/imgui_impl_opengl3.cpp)
target_include_directories(imgui PUBLIC imgui imgui/backends)
target_link_libraries(imgui PUBLIC glfw glad)

# ####################################
# Source Files
# ####################################
aux_source_directory(src SRC_LIST)

# ####################################
# Embed Assets Build Tools
# ####################################
add_executable(ResourceBuilder EXCLUDE_FROM_ALL builder/ResourceBuilder.cpp)
target_include_directories(ResourceBuilder PRIVATE include)

# Build embed assets - Rebuild manually when folder 'embed' has changed
add_executable(bin2c builder/bin2c.c)
add_custom_command(
    OUTPUT ${CMAKE_BINARY_DIR}/indexAssets.c ${CMAKE_BINARY_DIR}/blockAssets.c
    COMMAND $<TARGET_FILE:ResourceBuilder> -e ${CMAKE_CURRENT_SOURCE_DIR}/embed -i indexAssets -b blockAssets
    COMMAND $<TARGET_FILE:bin2c> -i indexAssets -o indexAssets.c -a _embed_indexAssets
    COMMAND $<TARGET_FILE:bin2c> -i blockAssets -o blockAssets.c -a _embed_blockAssets
    DEPENDS ${CMAKE_BINARY_DIR}/indexAssets ${CMAKE_BINARY_DIR}/blockAssets
)

# ####################################
# Target
# ####################################
add_executable(IngameIME-Test ${SRC_LIST} ${CMAKE_BINARY_DIR}/indexAssets.c ${CMAKE_BINARY_DIR}/blockAssets.c)
target_include_directories(IngameIME-Test PRIVATE include)
target_link_libraries(IngameIME-Test PRIVATE glfw glad imgui IngameIME_Static)

# Copy file assets
add_custom_command(TARGET IngameIME-Test POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/assets $<TARGET_FILE_DIR:IngameIME-Test>/assets
)

# Remove console window on Release
if(WIN32)
    set_target_properties(IngameIME-Test
        PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:CONSOLE"
        LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup"
    )
endif(WIN32)

# ####################################
# Test
# ####################################
enable_testing()

# GTest
set(GTEST ctest/gtest/googletest)
add_library(gtest STATIC EXCLUDE_FROM_ALL ${GTEST}/src/gtest-all.cc)
target_include_directories(gtest PUBLIC ${GTEST}/include ${GTEST})

# Unicode test
add_executable(T_Unicode ctest/T_Unicode.cpp)
target_include_directories(T_Unicode PRIVATE include)
target_link_libraries(T_Unicode PRIVATE gtest)