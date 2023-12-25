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

# ####################################
# Source Files
# ####################################
aux_source_directory(src SRC_LIST)

# ####################################
# Target
# ####################################
add_executable(IngameIME-Test ${SRC_LIST})
target_include_directories(IngameIME-Test PRIVATE include)
target_link_libraries(IngameIME-Test PRIVATE glfw glad)

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
# Embed Assets Build Tools
# ####################################
add_executable(ResourceBuilder EXCLUDE_FROM_ALL builder/ResourceBuilder.cpp)
target_include_directories(ResourceBuilder PRIVATE include)

# Build embed assets
if(MSVC)
    add_executable(MSVC_bin2obj builder/MSVC_bin2obj.cpp)
    target_include_directories(MSVC_bin2obj PRIVATE include)

    add_custom_target(BuildEmbed
        COMMAND $<TARGET_FILE:ResourceBuilder> -e ${CMAKE_CURRENT_SOURCE_DIR}/embed -i indexAssets -b blockAssets
        COMMAND $<TARGET_FILE:MSVC_bin2obj> -b indexAssets -o indexAssets.obj -s _embed_indexAssets
        COMMAND $<TARGET_FILE:MSVC_bin2obj> -b blockAssets -o blockAssets.obj -s _embed_blockAssets
    )
    add_dependencies(BuildEmbed ResourceBuilder MSVC_bin2obj)
else(MSVC)
    add_custom_target(BuildEmbed
        COMMAND $<TARGET_FILE:ResourceBuilder> -e ${CMAKE_CURRENT_SOURCE_DIR}/embed -i indexAssets -b blockAssets

        # objcopy .... todo
    )
    add_dependencies(BuildEmbed ResourceBuilder)
endif(MSVC)

target_link_libraries(IngameIME-Test PRIVATE ${CMAKE_BINARY_DIR}/indexAssets ${CMAKE_BINARY_DIR}/blockAssets)
add_dependencies(IngameIME-Test BuildEmbed)

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