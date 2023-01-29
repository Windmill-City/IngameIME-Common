cmake_minimum_required(VERSION 3.18)
project(testWnd)

# ##############################################################################
# Compiler flags
# ##############################################################################

set(CMAKE_CXX_STANDARD 17)

if(MSVC)
  # Use Static Runtime Library
  set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

# ##############################################################################
# Find Packages
# ##############################################################################

add_subdirectory(glfw EXCLUDE_FROM_ALL)
add_subdirectory(glad EXCLUDE_FROM_ALL)
add_subdirectory(nuklear EXCLUDE_FROM_ALL)

# ##############################################################################
# Targets
# ##############################################################################

add_executable(testWnd src/main.cpp)
target_include_directories(testWnd PRIVATE include)
target_link_libraries(testWnd PRIVATE glfw glad nuklear)

# ##############################################################################
# Packaging
# ##############################################################################

# Install
install(TARGETS testWnd)
