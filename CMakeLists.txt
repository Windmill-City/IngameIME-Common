cmake_minimum_required(VERSION 3.18)
project(IngameIME-Common)

# ##############################################################################
# Build files
# ##############################################################################

set(include_dir ${CMAKE_CURRENT_SOURCE_DIR}/include)
file(GLOB_RECURSE public_headers
     ${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp)

# ##############################################################################
# Targets
# ##############################################################################

# Library Target
add_library(IngameIME_Interface INTERFACE "${public_headers}")
target_include_directories(IngameIME_Interface INTERFACE "${include_dir}")
set_target_properties(IngameIME_Interface PROPERTIES PUBLIC_HEADER
                                                     "${public_headers}")
