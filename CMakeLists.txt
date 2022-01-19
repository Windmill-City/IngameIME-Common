cmake_minimum_required(VERSION 3.21.1)
project(IngameIME-Common VERSION 3.0.0)

include(CTest)
enable_testing()

file(GLOB_RECURSE SOURCES ./src/**.cpp ./src/**.c)

# Target
add_library(IngameIME STATIC ./src/IngameIME.cpp ./src/InputContext.cpp)
target_include_directories(IngameIME PUBLIC ./src/include)
target_include_directories(IngameIME PRIVATE ./src)
target_sources(IngameIME PRIVATE ${SOURCES})
